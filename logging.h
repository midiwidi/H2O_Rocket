#ifndef LOGGING_H_
#define LOGGING_H_

#include <FS.h>
extern File fid_log;

#define OFF		0
#define ERROR	1
#define WARNING	2
#define INFO	3
#define TRACE	4

/*** Change these settings to control the Log level or switch logging off ********************/
#define LOGLEVEL		INFO
#define SERIAL_LOGGING	true
/*********************************************************************************************/

#if LOGLEVEL != OFF
	#define logger(lev, fmt, ...) \
			do { logfunc(lev, fmt, ##__VA_ARGS__); } while (0)
#else
	#define logger(lev, fmt, ...)	(void)0
#endif

typedef enum loglevel_t
{
	LOG_OFF = OFF,
	LOG_ERROR = ERROR,
	LOG_WARNING = WARNING,
	LOG_INFO = INFO,
	LOG_TRACE = TRACE
};

const char loglvl_str[] = {' ', 'E', 'W', 'I', 'T'};

extern bool spiffs_recording;

void logfunc(loglevel_t level, char *format,...);

#endif /* LOGGING_H_ */
