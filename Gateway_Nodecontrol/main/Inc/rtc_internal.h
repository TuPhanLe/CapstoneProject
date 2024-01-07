#ifndef _RTC_INTERNAL
#define _RTC_INTERNAL
#include "time.h"

struct set_RTC{
    uint8_t year;
    uint8_t month;
    uint8_t mday;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct timeAlarm {
	unsigned char Hour;
	unsigned char Minute;
    unsigned char Second;
};

struct timeAlarm timeNow;



struct tm getTime();
void setTimeRTC(int hour, int minute);
unsigned char alarmRTC(int hour, int minute);
void RTC_set_Time(struct tm ltm);
time_t RTC_get_timeStamp();
struct tm RTC_get_time(void);
#endif