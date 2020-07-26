#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <FS.h>
#include <IPAddress.h>

#define DEFAULT_SERVO_CLOSE 	2300
#define DEFAULT_SERVO_OPEN		600
#define DEFAULT_SERVO_MIDDLE	(DEFAULT_SERVO_OPEN+DEFAULT_SERVO_CLOSE)>>1
#define CAM_RECORD_PULS_HIGH_TIME	200		// time of camera recording pulse high time in ms
#define DEFAULT_PERIOD_MPU_UPDATE	200
#define DEFAULT_PERIOD_SPIFFS_CHECK	10000
#define DEFAULT_PERIOD_CAM_CAPTURE	1000
#define PERIOD_WIFI_CHECK			1000
#define WIFI_TIMEOUT				10000

#define DEFAULT_SPIFFS_FREE_MIN					92160	//90kB
#define DEFAULT_ACCY_LAUNCH_DETECT_THRESHOLD 	1.4  	//Threshold of AccY when launch is detected upwards acceleration results in a bigger value than 1.0g
#define DEFAULT_RELEASE_DETECT_THRESHOLD		6000.0	// Pitch^2 + Roll()^2
#define DEFAULT_LANDED_DETECT_ALT_THRESHOLD		0.2		//0.2 m
#define DEFAULT_HPA_AT_SEALEVEL 				1013.25

struct config_t
{
		uint16_t reset_cnt;
		uint16_t servo_close;
		uint16_t servo_middle;
		uint16_t servo_open;

		uint16_t period_mpu_update;
		uint16_t period_spiffs_check;

		uint32_t spiffs_free_min;
		float accy_lauch_detect_thresh;
		float release_detect_thresh;
		float alt_landed_detect_thres;
		float hpa_at_sealevel;

		char ssid[32];
		char password[32];

		IPAddress ip;
		IPAddress netmask;
		IPAddress gateway;
		IPAddress dns;
};

extern const char *backup_ssid;
extern const char *backup_password;

bool read_config(config_t* config);
bool write_config(config_t* config);
bool default_config(config_t* config);
void print_config(config_t* config);

#endif /* CONFIG_H_ */
