#include <Arduino.h>
#include "H2O_Rocket.h"
#include "logging.h"


void setup()
{
	uint8_t vid, pid;
	uint8_t temp;
	Wire.begin();
	Serial.begin(115200);
	Serial.setDebugOutput(false);
	while (!Serial && !Serial.available());
	delay(300);

	spiffs_recording = true;

	// Init file system
	if (SPIFFS.begin())
		logger(LOG_INFO, "SPIFFS opened!");
	else
		logger(LOG_ERROR, "SPIFFS error!");
	fid_log = SPIFFS.open("/logger.txt", "a");

	state = STATE_PWR_ON;
	last_state = state;

	//read config from SPIFFS config.txt
	if (read_config(&config))
	{
		logger(LOG_WARNING, "config read error, using default values");
		default_config(&config);
	}
	else
		logger(LOG_INFO, "config read successful");

	config.reset_cnt++;

	if(write_config(&config))
		logger(LOG_WARNING, "config write error");
	else
		logger(LOG_INFO, "config write successful");

	print_config(&config);

	led.setup(PIN_LED_BLUE);
	button.setup(PIN_BUTTON);

	servo.attach(PIN_SERVO, config.servo_open, config.servo_close);
	servo.writeMicroseconds(config.servo_middle);

	pinMode(PIN_CAMERA_RECORD, OUTPUT);
	digitalWrite(PIN_CAMERA_RECORD, LOW);
	cam_capture = false;

	if(mpu.setup())
	{
		state = STATE_ERROR;
		logger(LOG_ERROR, "Could not connect to MPU9250");
	}
	else
		logger(LOG_INFO, "MPU9250 detected");

	if (!bmp.begin())
		logger(LOG_ERROR, "BMP280 not found!");
	else
		logger(LOG_INFO, "BMP280 detected.");

	/* Default settings from datasheet. */
	bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
					Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
					Adafruit_BMP280::SAMPLING_X2,    /* Pressure oversampling */
					Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
					Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */

	fid_dat = SPIFFS.open("/dat.hdr", "w");
	fid_dat.print(F("time_ms\taccx\taccy\taccz\tgyrox\tgyroy\tgyroz\tbx\tby\tbz\troll\tpitch\tyaw\taltitude"));
	fid_dat.close();

	WiFi.mode( WIFI_OFF );
	WiFi.forceSleepBegin();
	delay(1);

	launch_cnt = 0;

	t_next_mpu_update = millis() + config.period_mpu_update;

	logger(LOG_INFO, "Setup() finished, starting loop()");
}

void loop()
{
	button.update();
	if (state != STATE_ERROR)
		mpu.update();

	if (millis() > t_next_spiffs_check)
	{
		SPIFFS.info(spiffs_info);
		if (spiffs_info.totalBytes - spiffs_info.usedBytes < config.spiffs_free_min)
		{
			logger(LOG_WARNING, "free space is down to %d bytes, recording disabled");
			spiffs_recording = false;
		}
		t_next_spiffs_check = millis() + config.period_spiffs_check;
	}


	// State Transitions

	// from all states
	if (button.is_long_press() && button.get_duration() > 3000)
		state = STATE_IDLE;
	// from specific state
	switch (state)
	{
		case STATE_PWR_ON:
			state = STATE_IDLE;
			break;
		case STATE_IDLE:
			if (button.is_single_click())
				state = STATE_WAIT_LAUNCH;
			if (button.is_multi_click())
			{
				switch(button.get_click_count())
				{
					case 3:
						state = STATE_DWNLOAD;
						break;
					case 4:
						state = STATE_MPU_CAL;
						break;
					default:
						break;
				}
			}
			break;
		case STATE_WAIT_LAUNCH:
			if (mpu.getAcc(1) > config.accy_lauch_detect_thresh || button.is_single_click())
				state = STATE_UPWARDS;
			break;
		case STATE_UPWARDS:
			if ( (mpu.getRoll()*mpu.getRoll() + mpu.getPitch()*mpu.getPitch()) > config.release_detect_thresh || button.is_single_click()) // replaced by magnetometer apogee detection
				state = STATE_PARACHUTE_RELEASE;
			break;
		case STATE_PARACHUTE_RELEASE:
			state = STATE_DOWNWARDS;
			break;
		case STATE_DOWNWARDS:
			if (altitude < config.alt_landed_detect_thres || button.is_single_click())
				state = STATE_LANDED;
			break;
		case STATE_LANDED:
			if (button.is_single_click())
				state = STATE_IDLE;
			break;
		case STATE_DWNLOAD:
			if (button.is_single_click())
				state = STATE_IDLE;
			break;
		case STATE_MPU_CAL:
			state = STATE_IDLE;
			break;
		case STATE_ERROR:
			//This state is not meant to be left. If we end up here, something serious went wrong. The state can only be left to Idle by button long press.
			break;
		default:
			state = STATE_IDLE;
	}

	// State Exit Actions
	switch (last_state)
	{
		case STATE_PWR_ON:
			// State Exit Actions
			if (last_state != state)
			{
				//logger("Exited state PWR_ON");
			}
			break;
		case STATE_IDLE:
			// State Exit Actions
			if (last_state != state)
			{
				//logger("Exited state IDLE");
			}
			break;
		case STATE_WAIT_LAUNCH:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state WAIT_LAUNCH");
			}
			break;
		case STATE_UPWARDS:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state UPWARDS");
			}
			break;
		case STATE_PARACHUTE_RELEASE:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state PARACHUTE_RELEASE");
			}
			break;
		case STATE_DOWNWARDS:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state DOWNWARDS");
			}
			break;
		case STATE_LANDED:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state LANDED");
			}
			break;
		case STATE_DWNLOAD:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state DWNLOAD");

				//read config from SPIFFS config.txt
				if (read_config(&config))
				{
					logger(LOG_WARNING, "config read error, using default values");
					default_config(&config);
				}
				else
					logger(LOG_INFO, "config read successful");
				print_config(&config);
			}
			break;
		case STATE_MPU_CAL:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state MPU_CAL");
			}
			break;
		case STATE_ERROR:
			// State Exit Actions
			if (last_state != state) // transition
			{
				//logger("Exited state ERROR");
			}
			break;
		default:
			break;
	}

	// State Entry and In-State Actions
	switch (state)
	{
		case STATE_PWR_ON:
			// In-State Actions
			break;
		case STATE_IDLE:
			// State Entry Actions
			if (last_state != state)
			{
				// Stop recording
				if (cam_capture)
				{
					digitalWrite(PIN_CAMERA_RECORD, HIGH);
					delay(CAM_RECORD_PULS_HIGH_TIME);
					pinMode(PIN_CAMERA_RECORD, INPUT);
					cam_capture = false;
					logger(LOG_INFO, "HD camera recording stopped");
				}

				if (fid_log)
					fid_log.close();

				fid_log = SPIFFS.open("/logger.txt", "a");
				if (!fid_log)
					logger(LOG_ERROR, "open /logger.txt failed!");

				logger(LOG_INFO, "Entered state IDLE");
				led.set_blinks(1);
				servo.writeMicroseconds(config.servo_middle);

				SPIFFS.info(spiffs_info);
				logger(LOG_INFO, "SPIFFS space - used: %d (%d%%), total: %d, free: %d (%d%%)",
									spiffs_info.usedBytes, (100*spiffs_info.usedBytes)/spiffs_info.totalBytes,
									spiffs_info.totalBytes,
									spiffs_info.totalBytes-spiffs_info.usedBytes, (100*(spiffs_info.totalBytes-spiffs_info.usedBytes))/spiffs_info.totalBytes);
			}
			// In-State Actions
			else
			{
				if (button.is_double_click())
				{
					if (servo.readMicroseconds() == config.servo_close)
						servo.writeMicroseconds(config.servo_middle);
					else
						servo.writeMicroseconds(config.servo_close);
				}
			}
			break;
		case STATE_WAIT_LAUNCH:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state WAIT_LAUNCH");
				led.set_blinks(2);
				servo.writeMicroseconds(config.servo_close);

				launch_cnt++;

				if (fid_dat)
					fid_dat.close();
				sprintf(str, "/%03d_%03d.dat", config.reset_cnt, launch_cnt);
				fid_dat = SPIFFS.open(str, "w");
				if (!fid_dat)
					logger(LOG_ERROR, "open %s failed!", str);

				// Start recording
				if (!cam_capture)
				{
					digitalWrite(PIN_CAMERA_RECORD, HIGH);
					delay(CAM_RECORD_PULS_HIGH_TIME);
					digitalWrite(PIN_CAMERA_RECORD, LOW);
					cam_capture = true;
					logger(LOG_INFO, "HD camera recording started");
				}
			}
			// In-State Actions
			else
			{

			}
			break;
		case STATE_UPWARDS:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state UPWARDS");
				led.set_blinks(3);
				launch_altitude = bmp.readAltitude(config.hpa_at_sealevel);
			}
			// In-State Actions
			else
			{
				// There are actions for this state below the case statement
			}
			break;
		case STATE_PARACHUTE_RELEASE:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state PARACHUTE_RELEASE");
				servo.writeMicroseconds(config.servo_open);
			}
			// In-State Actions
			else
			{
				// There are actions for this state below the case statement
			}
			break;
		case STATE_DOWNWARDS:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state DOWNWARDS");
				led.set_blinks(4);
			}
			// In-State Actions
			else
			{
				// There are actions for this state below the case statement
			}
			break;
		case STATE_LANDED:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state LANDED");
				led.set_blinks(5);

				fid_dat.close();

				// Stop recording
				if (cam_capture)
				{
					digitalWrite(PIN_CAMERA_RECORD, HIGH);
					delay(CAM_RECORD_PULS_HIGH_TIME);
					digitalWrite(PIN_CAMERA_RECORD, LOW);
					cam_capture = false;
					logger(LOG_INFO, "HD camera recording stopped");
				}
			}
			// In-State Actions
			else
			{

			}
			break;
		case STATE_DWNLOAD:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state DWNLOAD");
				led.set_blinks(6);

				WiFi.forceSleepWake();
				delay(1);
				// Disable the WiFi persistence.  The ESP8266 will not load and save WiFi settings in the flash memory.
				WiFi.persistent( false );
				WiFi.mode( WIFI_STA );

				//Init Wifi
				WiFi.config(config.ip, config.dns, config.gateway, config.netmask);
				WiFi.begin(config.ssid, config.password);

				logger(LOG_INFO, "connecting to %s", config.ssid);

				strncpy(str, config.ssid, sizeof(str));
				wifi_connect = true;

				fid_log.close();
				fid_dat.close();

				t_wifi_timeout = millis() + WIFI_TIMEOUT;
				t_next_wifi_check = millis() + PERIOD_WIFI_CHECK;
			}
			// In-State Actions
			else
			{
				if (wifi_connect == false)
				{
					//connected
					ftpSrv.handleFTP();
					//Serial.print(".");
				}
				else
				{
					// connecting
					if(WiFi.status() == WL_CONNECTED)
					{
						if (wifi_connect)
						{
							//entered once after connection established
							wifi_connect = false;
							logger(LOG_INFO, "Wifi connected to: %s, IP address: %s", str, WiFi.localIP().toString().c_str());
							ftpSrv.begin(ftp_user, ftp_password);    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
						}
					}
					else
					{
						if (millis() >= t_next_wifi_check)
						{
							Serial.print(".");
							t_next_wifi_check = millis() + PERIOD_WIFI_CHECK;

							if(millis() > t_wifi_timeout)
							{
								// when the configured connection times out, use the backup (my phone)
								logger(LOG_WARNING, "couln't establish connection to %s, trying %s", str, backup_ssid);
								strncpy(str, backup_ssid, sizeof(str));
								WiFi.persistent(false);
								WiFi.mode(WIFI_OFF);   // this is a temporary line, to be removed after SDK update to 1.5.4
								WiFi.mode(WIFI_STA);
								WiFi.config(0u, 0u, 0u);
								WiFi.begin(backup_ssid, backup_password);

								t_wifi_timeout = millis() + WIFI_TIMEOUT;
							}
						}
					}
				}
			}
			break;
		case STATE_MPU_CAL:
			if (last_state != state) // transition
			{
				logger(LOG_INFO, "Entered state MPU_CAL");
				led.set_blinks(7);
				mpu.printCalibration();
				logger(LOG_INFO, "starting accelerometer and gyro calibration");
				mpu.calibrateAccelGyro();
				logger(LOG_INFO, "starting magnetometer calibration");
				mpu.calibrateMag();
				mpu.printCalibration();
			}
			// In-State Actions
			else
			{

			}
			break;
		case STATE_ERROR:
			if (last_state != state) // transition
			{
				logger(LOG_ERROR, "Entered state ERROR");
				led.set_blinks(65535);
			}
			// In-State Actions
			else
			{

			}
			break;
		default:
			logger(LOG_ERROR, "Unknown state (%d), returning to IDLE.", state);
	}


	//Actions which are taken in more than one state
	if (state==STATE_UPWARDS || state==STATE_PARACHUTE_RELEASE || state==STATE_DOWNWARDS )
	{
		if (millis() >= t_next_mpu_update)
		{

			/*** no valid magnetometer data workaround **************************************/
			float bx_tmp = mpu.getMag(0);
			float by_tmp = mpu.getMag(1);
			float bz_tmp = mpu.getMag(2);
			if (bx_tmp != 0.0 || by_tmp != 0.0|| bz_tmp != 0.0)
			{
				bx = bx_tmp;
				by = by_tmp;
				bz = bz_tmp;
			}
			/*******************************************************************************/

			altitude = bmp.readAltitude(config.hpa_at_sealevel) - launch_altitude;
			//temperature = bmp.readTemperature();

			if(fid_dat && spiffs_recording)
			{
				fid_dat.printf(	"%ld\t%6.3f\t%6.3f\t%6.3f\t"
								"%7.2f\t%7.2f\t%7.2f\t"
								"%7.1f\t%7.1f\t%7.1f\t"
								"%7.2f\t%7.2f\t%7.2f\t"
								"%7.2f\r\n",
								millis(), mpu.getAcc(0), mpu.getAcc(1), mpu.getAcc(2),
								mpu.getGyro(0), mpu.getGyro(1), mpu.getGyro(2),
								bx, by, bz,
								mpu.getRoll(), mpu.getPitch(), mpu.getYaw(),
								altitude);
			}
			Serial.printf(	"%ld\t%6.3f\t%6.3f\t%6.3f\t"
							"%7.2f\t%7.2f\t%7.2f\t"
							"%7.1f\t%7.1f\t%7.1f\t"
							"%7.2f\t%7.2f\t%7.2f\t"
							"%7.2f\r\n",
							millis(), mpu.getAcc(0), mpu.getAcc(1), mpu.getAcc(2),
							mpu.getGyro(0), mpu.getGyro(1), mpu.getGyro(2),
							bx, by, bz,
							mpu.getRoll(), mpu.getPitch(), mpu.getYaw(),
							altitude);

			t_next_mpu_update = millis() + config.period_mpu_update;
		}
	}

	last_state = state;

	led.update();
}
