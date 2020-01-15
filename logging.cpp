#include <Arduino.h>
#include "logging.h"
#include "config.h"

extern struct config_t config;
extern int launch_cnt;

#if LOGLEVEL != OFF
void logfunc(loglevel_t level, char *format,...)
{
	char buff[128];
	unsigned long time = millis();
	size_t len;

	if (level <= LOGLEVEL && level != LOG_OFF)
	{
		snprintf(buff, sizeof(buff), "[%c] %d %d %d - ", loglvl_str[level], config.reset_cnt, launch_cnt, time);
		va_list args;
		va_start (args,format);

		len=strnlen(buff, sizeof(buff));
		vsnprintf(&buff[len], sizeof(buff)-len, format, args);
		va_end (args);
		strncat(buff , "\n", sizeof(buff));
		buff[sizeof(buff)/sizeof(buff[0])-1]='\0';
		if (fid_log && spiffs_recording)
		{
			fid_log.print(buff);
		}
		strncat(buff , "\r", sizeof(buff));
		buff[sizeof(buff)/sizeof(buff[0])-1]='\0';
		#if SERIAL_LOGGING
			Serial.print(buff);
		#endif
	}
}
#endif
