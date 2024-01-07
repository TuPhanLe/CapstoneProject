#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "esp_log.h"
#include <sys/param.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rtc_io.h"
#include "esp_err.h"
#include "esp32/rom/rtc.h"

#include "sys/time.h"
#include "sdkconfig.h"


#include "rtc_internal.h"


/********** Local Constant and compile switch definition section **************/

/********** Local Type definition section *************************************/
 
/********** Local Macro definition section ************************************/

/********** Local (static) variable definition ********************************/
static time_t rtc_seconds;

/********** Local (static) function declaration section ***********************/

/********** Local function definition section *********************************/

/********** Global function definition section ********************************/



struct tm getTime()
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
	// get time now 
	timeNow.Hour = timeinfo.tm_hour;
	timeNow.Minute = timeinfo.tm_min;
	timeNow.Second = timeinfo.tm_sec;
    //printf("%d/%d/%d/%d \r\n", timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
    return timeinfo;
}

unsigned char alarmRTC(int hour, int minute)
{
	unsigned char flag;

	//Read Time from RTC
	getTime();
    //printf("%d/%d/%d \r\n", timeNow.Hours, timeNow.Minutes, timeNow.Second); 

	if(timeNow.Hour == hour){
		if(timeNow.Minute < minute){
			flag = 1;     // continnue alarm
		}
		else{
			flag = 0;     // stop alarm
		}
	}
	else{
		flag = 1;         // continnue alarm
	}
	return flag;
}
////////////////////////////////////////////////////////////


void setTimeRTC(int hour, int minute)
{   
    esp_err_t ret;
	struct tm default_time = {
        .tm_year = 2023 - 1900,
        .tm_mon = 10, // January
        .tm_mday = 10,
        .tm_hour = hour,
        .tm_min = minute,
        .tm_sec = 0
    };
    time_t default_time_secs = mktime(&default_time);
    struct timeval tv = {
        .tv_sec = default_time_secs,
        .tv_usec = 0
    };
    //printf("Setting default time to %ld\n", default_time_secs);
    settimeofday(&tv, NULL);

    // Initialize the RTC GPIO
    //printf("Initializing RTC GPIO\n");
    ret = rtc_gpio_init(GPIO_NUM_26);
    if (ret != ESP_OK) {
        printf("Error initializing RTC GPIO: %d\n", ret);
        return;
    }
    ret = rtc_gpio_set_direction(GPIO_NUM_26, RTC_GPIO_MODE_INPUT_ONLY);
    if (ret != ESP_OK) {
        printf("Error setting RTC GPIO direction: %d\n", ret);
        return;
    }
    printf("set alarm success \r\n");
}





void RTC_set_Time(struct tm ltm)
{
    esp_err_t ret;
    time_t time =  mktime(&ltm);

    struct timeval tv = {
        .tv_sec = time,
        .tv_usec = 0
    };
    //printf("Setting default time to %ld\n", default_time_secs);
    settimeofday(&tv, NULL);

    ret = rtc_gpio_init(GPIO_NUM_26);
    if (ret != ESP_OK) {
        ESP_LOGE("RTC","Error initializing RTC GPIO: %d", ret);
        return;
    }
    ret = rtc_gpio_set_direction(GPIO_NUM_26, RTC_GPIO_MODE_INPUT_ONLY);
    if (ret != ESP_OK) {
        ESP_LOGE("RTC","Error setting RTC GPIO direction: %d", ret);
        return;
    }
    ESP_LOGI("RTC","Set time for RTC successfully");
}


time_t RTC_get_timeStamp()
{
    //struct timeval tv_now;
    gettimeofday(&rtc_seconds, NULL);
    //int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
    //printf("%d \r\n",time_us);
    //printf("Tv_now: %d \r\n",tv_now);
    return rtc_seconds;
}

struct tm RTC_get_time(void)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);
	// get time now 
	// timeNow.Hours = timeinfo.tm_hour;
	// timeNow.Minutes = timeinfo.tm_min;
	// timeNow.Minutes = timeinfo.tm_sec;
    // printf("%d/%d/%d/%d \r\n", timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);

    return timeinfo;
}


