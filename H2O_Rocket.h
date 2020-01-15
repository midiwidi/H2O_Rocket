#ifndef H2O_ROCKET_H_
#define H2O_ROCKET_H_

#include <ESP8266WiFi.h>
#include "ESP8266FtpServer.h"
#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include "memorysaver.h"
#include "wemos_d1_mini.h"
#include "led_state_display.h"
#include "button_input.h"
#include <MPU9250.h>
#include <Adafruit_BMP280.h>
#include "config.h"

#define PIN_BUTTON	PIN_D3
#define PIN_SERVO	PIN_D8
#define RGBCAM_SPI_CS PIN_D0

typedef enum
{
	STATE_PWR_ON = 0,
	STATE_IDLE = 1,
	STATE_WAIT_LAUNCH = 2,
	STATE_UPWARDS = 3,
	STATE_PARACHUTE_RELEASE = 4,
	STATE_DOWNWARDS = 5,
	STATE_LANDED = 6,
	STATE_DWNLOAD = 7,
	STATE_MPU_CAL = 8,
	STATE_ERROR = 9
}   state_t;

const char *backup_ssid = "WLAN_midiwidi_mobile";
const char *backup_password = "electronic123";
const char *ftp_user = "RocketFTP";
const char *ftp_password = "flybabyfly";

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
ArduCAM rgbcam(OV2640, RGBCAM_SPI_CS);
Servo servo;
LED_state_display led;
button_input button;
MPU9250 mpu;
Adafruit_BMP280 bmp;
state_t state, last_state;
File fid_log, fid_dat;
FSInfo spiffs_info;
bool spiffs_recording;
int launch_cnt;
char str[32];
bool arducam_capture_started, arducam_capture;
unsigned long t_next_mpu_update;
unsigned long t_next_spiffs_check;
unsigned long t_next_pic;
unsigned long t_next_wifi_check;
unsigned long t_wifi_timeout;
float bx, by, bz;
float altitude, launch_altitude;
struct config_t config;
bool wifi_connect;

#endif /* H2O_ROCKET_H_ */
