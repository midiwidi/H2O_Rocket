#include <Arduino.h>
#include "config.h"
#include "logging.h"

bool read_config(config_t* config)
{
	File fid_cfg;
	char buff[64];
	int ip[4];
	int n;

	if(!(fid_cfg = SPIFFS.open("/config.txt", "r")))  return true;

	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->reset_cnt = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->accy_lauch_detect_thresh = atof(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->alt_landed_detect_thres = atof(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->hpa_at_sealevel = atof(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->period_mpu_update = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->period_spiffs_check = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->release_detect_thresh = atof(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->servo_close = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->servo_middle = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->servo_open = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	config->spiffs_free_min = atoi(buff);
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; fid_cfg.readBytes(buff,1); n=fid_cfg.readBytesUntil('\n', config->ssid, sizeof(config->ssid)); if (!n) goto error; config->ssid[n]='\0';
	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; fid_cfg.readBytes(buff,1); n=fid_cfg.readBytesUntil('\n', config->password, sizeof(config->password)); if (!n) goto error; config->password[n]='\0';

	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[0] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[1] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[2] = atoi(buff);
	n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[3] = atoi(buff);
	config->ip = IPAddress(ip[0], ip[1], ip[2], ip[3]);

	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[0] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[1] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[2] = atoi(buff);
	n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[3] = atoi(buff);
	config->netmask = IPAddress(ip[0], ip[1], ip[2], ip[3]);

	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[0] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[1] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[2] = atoi(buff);
	n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[3] = atoi(buff);
	config->gateway = IPAddress(ip[0], ip[1], ip[2], ip[3]);

	n=fid_cfg.readBytesUntil('=', buff, sizeof(buff)); if (!n) goto error; n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[0] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[1] = atoi(buff);
	n=fid_cfg.readBytesUntil('.', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[2] = atoi(buff);
	n=fid_cfg.readBytesUntil('\n', buff, sizeof(buff)); if (!n) goto error; buff[n]='\0';
	ip[3] = atoi(buff);
	config->dns = IPAddress(ip[0], ip[1], ip[2], ip[3]);

	fid_cfg.close();
	return false;

	error:
		fid_cfg.close();
		return true;
}

bool write_config(config_t* config)
{
	File fid_cfg;

	if(!(fid_cfg = SPIFFS.open("/config.txt", "w")))  return true;

	fid_cfg.printf("reset_cnt = %d\n", config->reset_cnt);
	fid_cfg.printf("accy_lauch_detect_thresh = %.2f\n", config->accy_lauch_detect_thresh);
	fid_cfg.printf("alt_landed_detect_thres = %.2f\n", config->alt_landed_detect_thres);
	fid_cfg.printf("hpa_at_sealevel = %.2f\n", config->hpa_at_sealevel);
	fid_cfg.printf("period_mpu_update = %d\n", config->period_mpu_update);
	fid_cfg.printf("period_spiffs_check = %d\n", config->period_spiffs_check);
	fid_cfg.printf("release_detect_thresh = %.2f\n", config->release_detect_thresh);
	fid_cfg.printf("servo_close = %d\n", config->servo_close);
	fid_cfg.printf("servo_middle = %d\n", config->servo_middle);
	fid_cfg.printf("servo_open = %d\n", config->servo_open);
	fid_cfg.printf("spiffs_free_min = %d\n", config->spiffs_free_min);
	fid_cfg.printf("ssid = %s\n", config->ssid);
	fid_cfg.printf("password = %s\n", config->password);

	fid_cfg.printf("ip = %d.%d.%d.%d\n", config->ip[0],config->ip[1],config->ip[2],config->ip[3]);
	fid_cfg.printf("netmask = %d.%d.%d.%d\n", config->netmask[0],config->netmask[1],config->netmask[2],config->netmask[3]);
	fid_cfg.printf("gateway = %d.%d.%d.%d\n", config->gateway[0],config->gateway[1],config->gateway[2],config->gateway[3]);
	fid_cfg.printf("dns = %d.%d.%d.%d\r\n", config->dns[0],config->dns[1],config->dns[2],config->dns[3]);

	fid_cfg.close();
	return false;
}

bool default_config(config_t* config)
{
	config->accy_lauch_detect_thresh = DEFAULT_ACCY_LAUNCH_DETECT_THRESHOLD;
	config->alt_landed_detect_thres = DEFAULT_LANDED_DETECT_ALT_THRESHOLD;
	config->hpa_at_sealevel = DEFAULT_HPA_AT_SEALEVEL;
	config->period_mpu_update = DEFAULT_PERIOD_MPU_UPDATE;
	config->period_spiffs_check = DEFAULT_PERIOD_SPIFFS_CHECK;
	config->release_detect_thresh = DEFAULT_RELEASE_DETECT_THRESHOLD;
	config->servo_close = DEFAULT_SERVO_CLOSE;
	config->servo_middle = DEFAULT_SERVO_MIDDLE;
	config->servo_open = DEFAULT_SERVO_OPEN;
	config->spiffs_free_min = DEFAULT_SPIFFS_FREE_MIN;
	strncpy(config->ssid, backup_ssid, sizeof(config->ssid));
	strncpy(config->password, backup_password, sizeof(config->password));
	config->ip = IPAddress(0,0,0,0);
	config->netmask = IPAddress(0,0,0,0);
	config->gateway = IPAddress(0,0,0,0);
	config->dns = IPAddress(0,0,0,0);
	return false;
}

void print_config(config_t* config)
{
	Serial.printf(	"reset counter = %d\r\n"
					"accy_lauch_detect_thresh = %f\r\n"
					"alt_landed_detect_thres = %f\r\n"
					"hpa_at_sealevel = %f\r\n"
					"period_mpu_update = %d\r\n"
					"period_spiffs_check = %d\r\n"
					"release_detect_thresh = %f\r\n"
					"servo_close = %d\r\n"
					"servo_middle = %d\r\n"
					"servo_open = %d\r\n"
					"spiffs_free_min = %d\r\n"
					"ssid = %s\r\n"
					"password = %s\r\n"
					"ip = %d.%d.%d.%d\r\n"
					"netmask = %d.%d.%d.%d\r\n"
					"gateway = %d.%d.%d.%d\r\n"
					"dns = %d.%d.%d.%d\r\n",
					config->reset_cnt,
					config->accy_lauch_detect_thresh,
					config->alt_landed_detect_thres,
					config->hpa_at_sealevel,
					config->period_mpu_update,
					config->period_spiffs_check,
					config->release_detect_thresh,
					config->servo_close,
					config->servo_middle,
					config->servo_open,
					config->spiffs_free_min,
					config->ssid,
					config->password,
					config->ip[0],config->ip[1],config->ip[2],config->ip[3],
					config->netmask[0],config->netmask[1],config->netmask[2],config->netmask[3],
					config->gateway[0],config->gateway[1],config->gateway[2],config->gateway[3],
					config->dns[0],config->dns[1],config->dns[2],config->dns[3]);
}
