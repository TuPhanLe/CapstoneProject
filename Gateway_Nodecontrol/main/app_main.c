
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "esp_log.h"
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp32/rom/rtc.h"
#include "esp32/rom/crc.h"
#include "sys/time.h"
#include "sdkconfig.h"


#include "app_main.h"
#include "util.h"
#include "bluetooth.h"
#include "mqtt.h"
#include "spiffs.h"
//#include "rtc_pcf8563.h"
#include "rtc_internal.h"
#include "schedule.h"
#include "mlsOnboard.h"
#include "outdoor_farm_config.h"
#include "GPIO.h"
#include "app_uart.h"
#include "lora_e32.h"
#include "wifi.h"
#include "min.h"


#if 1 /* GLobal varible */
  
    /* test alarm*/
    unsigned char first_Initialization = 1;
    unsigned char test = 1;
    unsigned char suspend = 0;
    TaskHandle_t xHandle;
    
#endif

#if 1
    void operate();

    void setup(void);
#endif



///////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////




void init_defaut_param(void)
{
#ifdef FIRMWARE_CONTROL_NODE
    struct pump value = get_pump_1_info();

    value.GPIO_NUM = GPIO_PUMP_1;
    output_io_create(GPIO_PUMP_1);
    set_pump_1_info(value);
#endif

#ifdef FIRMWARE_GATEWAY
    struct info dev_info = get_dev_info();
    dev_info.is_connected_server = false;
    dev_info.is_connected_control_node = false;
    dev_info.is_connected_sensor_node1 = false;
    dev_info.is_connected_sensor_node2 = false;
    dev_info.get_connect_all_node_flag = false;
    set_dev_info(dev_info);
#endif
}

void setup(void)
{
    /* Set gpio output for control relay */
    //gpio_pad_select_gpio(23);
    //gpio_set_direction(23,GPIO_MODE_OUTPUT);

    // output_io_create(GPIO_NUM_12);
    // output_io_create(GPIO_NUM_13);
    // output_io_create(GPIO_NUM_14);
    // output_io_create(GPIO_NUM_27);
}





void operate(void)
{
    
#ifdef USE_EXTERNAL_RTC
    test_rtc();
    test_rtc();
    printf("hi\r\n");

    Rtc = rtc_pcf_read();

    ESP_LOGW("Test RTC", "%02d - %02d/%02d/%02d %02d:%02d:%02d", Rtc.tm_wday, Rtc.tm_mday, Rtc.tm_mon, Rtc.tm_year, Rtc.tm_hour, Rtc.tm_min, Rtc.tm_sec);
    //printf("time: %d",gtime.tm_hour);
#else /* USE_INTERNAL_RTC */
    // struct tm time_sync;
    // time_sync.tm_year = 23 + 100;
    // time_sync.tm_mon = 10 - 1;
    // time_sync.tm_mday = 7;
    // time_sync.tm_hour = 22;
    // time_sync.tm_min = 26;
    // time_sync.tm_sec = 10;
    // RTC_set_Time(time_sync);
    
#endif


#if 0 // test flash
    char file_name[30] = "wifi.txt";
    // char file_name1[30] = "schedule.txt";
    //char write_buffer[100] = "SSID: windpink PASSWORD: 11111111 \r\n";
    // char write_buffer1[100] = "SSID: windPink PASSWORD: 11111111 \r\n";
    // char write_buffer2[100] = "SSID: Windpink PASSWORD: 11111111 \r\n";
    // char write_buffer3[100] = "SSID: Windpink PASSWORD: 11111111 \r\n";
    char read_buffer[100];

    //save_file_flash(file_name, sizeof(file_name), write_buffer, strlen(write_buffer), WRITE_ONLY);

    memset(read_buffer,NULL,sizeof(read_buffer));
    
    bool ret = read_file_flash(file_name, sizeof(file_name), read_buffer, sizeof(read_buffer), READ_ONLY);
    if(ret == true)
    {
        ESP_LOGW("READ WIFI FROM FLASH", " %s",read_buffer);
    }
    
    //printf("%s",read_buffer);
#endif
}



void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    //ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_loop_create_default());

    init_defaut_param();
    
    // operate();
   
    init_timer_1_second();
    read_info_esp_after_on_board();
    
    setup();

    uart_init();
    loraInit();

    
    //uint8_t buffTestMin[6] = {0x00, 0x01, 0x07, 0x00, 0x07, 0x02};
    
    //switch_channel(&control_node);

#ifdef FIRMWARE_GATEWAY
    init_spiffs();
    read_param_memory();
    internet_init();
    start_ble_task(); 
    mqtt_init();
#endif

//     while(1){
// #ifdef FIRMWARE_GATEWAY
//     //sendFixedMessage(&control_node, buffTestMin, sizeof(buffTestMin)); 
// #endif
    
//         vTaskDelay(2000/portTICK_PERIOD_MS);
//     }
    

}


