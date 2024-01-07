#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_log.h"

#include "schedule.h"
#include "outdoor_farm_config.h"
#include "rtc_internal.h"
#include "GPIO.h"
#include "lora_e32.h"


esp_timer_handle_t timer;

struct info dev_info;
struct setting setting_value;
struct status dev_stt;
struct pump pump1;


struct info get_dev_info(void)
{
    return dev_info;
}
void set_dev_info(struct info value)
{
    dev_info = value;
}

struct setting get_setting_value(void)
{
    return setting_value;
}
void set_setting_value(struct setting value)
{
    setting_value = value;
}

struct pump get_pump_1_info(void)
{
    return pump1;
}
void set_pump_1_info(struct pump value)
{
    pump1 = value;
}

struct status get_dev_stt(void)
{
    return dev_stt;
}
void set_dev_stt(struct status value)
{
    dev_stt = value;
}

void period_timer_control(void *param)
{
    uint32_t time_stamp;
    struct tm gtime;

    struct info dev_info = get_dev_info();
    struct status dev_stt = get_dev_stt();
    uint8_t buffTestMin[1] = {0x11};
    
    

    dev_info.count_time++;
#ifdef FIRMWARE_GATEWAY           
    //time_stamp = RTC_get_timeStamp();
    //printf("Time stamp: %d",time_stamp);

    gtime = RTC_get_time();
    printf("Timer:%d/%d | Date:%d/%d/%d [%d:%d:%d]\r\n",
                                                    dev_info.count_time,
                                                    dev_info.get_data_sensor_period,
                                                    gtime.tm_year - 100 + 2000,
                                                    gtime.tm_mon + 1,
                                                    gtime.tm_mday,
                                                    gtime.tm_hour,
                                                    gtime.tm_min,
                                                    gtime.tm_sec);
    // if(dev_info.count_time == 20)
    // {
    //     ESP_LOGI(SENSOR_TAG,"Send command to Node sensor 1");
    //     sendFixedMessage(&sensor_node1, buffTestMin, sizeof(buffTestMin));
    // }
    // if(dev_info.count_time == 35)
    // {
    //     ESP_LOGI(SENSOR_TAG,"Send command to Node sensor 2");
    //     sendFixedMessage(&sensor_node2, buffTestMin, sizeof(buffTestMin));
    // }                                                      
    if(dev_info.count_time >= dev_info.get_data_sensor_period)
    {
        dev_info.count_time = 0;

        //ESP_LOGI(SENSOR_TAG,"Reset_timmer");
        //sendFixedMessage(&sensor_node1, buffTestMin, sizeof(buffTestMin));
        //send_cmd_wait_resp(&control_node, buffTestMin, sizeof(buffTestMin), 10);
        send_cmd_wait_resp(&sensor_node1, buffTestMin, sizeof(buffTestMin), 10);
        send_cmd_wait_resp(&sensor_node2, buffTestMin, sizeof(buffTestMin), 10);
       
    }
#endif

#ifdef FIRMWARE_CONTROL_NODE
    check_all_schedule();

    gtime = RTC_get_time();
    printf("Date:%d/%d/%d [%d:%d:%d]\r\n",            
                                        gtime.tm_year - 100 + 2000,
                                        gtime.tm_mon + 1,
                                        gtime.tm_mday,
                                        gtime.tm_hour,
                                        gtime.tm_min,
                                        gtime.tm_sec);
#endif
    set_dev_info(dev_info);
}

void check_all_schedule(void)
{
    struct pump pump1 = get_pump_1_info();
    check_each_schedule(pump1);
}


void check_each_schedule(struct pump param_pump)
{
    uint8_t data[2];
    struct tm gtime = getTime();
    
    if(param_pump.pending_alarm == true && param_pump.GPIO_STATE == HIGH_LEVEL)
    {
        ESP_LOGI(TAG_ALARM,"PUMP1 ON | Date:%d/%d/%d | [%d:%d:%d][%d:%d:%d]",
                                                                gtime.tm_mday,
                                                                gtime.tm_mon + 1,
                                                                gtime.tm_year - 100 + 2000,
                                                                param_pump.start_hour,
                                                                param_pump.start_minute,
                                                                param_pump.start_second,
                                                                param_pump.end_hour,
                                                                param_pump.end_minute,
                                                                param_pump.end_second); 
        if(alarmRTC(param_pump.end_hour,param_pump.end_minute) == 0)
        {
            ESP_LOGW(TAG_ALARM,"Finished pending pump1\r\n");
            output_io_set_level(param_pump.GPIO_NUM,LOW_LEVEL);
            memset(&param_pump,NULL,sizeof(struct pump));

            param_pump.return_data_flag = true;
            set_pump_1_info(param_pump);
        }
    }
    if(param_pump.return_data_flag == true)
    {
        param_pump.return_data_flag = false;
        data[0] = param_pump.pending_pump;
		data[1] = param_pump.pending_alarm;
        
        send_state_back_to_gateway(data,2);
        set_pump_1_info(param_pump);
    }
}

void stop_timer_1_second()
{
    esp_timer_stop(timer);
}

void restart_timer_1_second()
{
    esp_timer_start_periodic(timer, 1000000);
}

void init_timer_1_second()
{
    esp_timer_create_args_t timer_param ={
        .callback = period_timer_control,
        .name = "timer_1_second"
    };

    esp_timer_create(&timer_param, &timer);

    /* 1000000 us*/
    esp_timer_start_periodic(timer, 1000000);

    dev_info.get_data_sensor_period  = 60;
}

