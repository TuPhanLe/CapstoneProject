#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"


#include "util.h"
#include "rtc_pcf8563.h"


#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif


static const char *TAG = "RTC_PCF8563";
struct tm Rtc;

// struct tm RTC_get_time(void)
// {
//     return Rtc;
// }

static esp_err_t i2c_master_init(void)
{
    esp_err_t ret;
    int i2c_master_port = I2C_MASTER_NUM;
    static bool i2c_init_driver = false;

    if(i2c_init_driver == true){
        i2c_driver_delete(i2c_master_port);
    }

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    
    ret = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    i2c_init_driver = true;
    return ret;
}


void i2c_scan()
{
    printf("i2c scan: \n");
    for(uint8_t i = 1; i < 127;i++)
    {
        int ret;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100/portTICK_RATE_MS);

        if(ret == ESP_OK){
            printf("Found device at address: %02X \n", i);
        }


    }
}


static esp_err_t rtc_pcf_read_reg_nbyte(uint8_t i2c_reg, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // first, send device address (indicating write) & register to be read
    i2c_master_write_byte(cmd, ( RTC_PCF85263_ADDR << 1 ), ACK_CHECK_EN);
    //i2c_master_write_byte(cmd, 0xA3, ACK_CHECK_EN);
    // send register we want
    i2c_master_write_byte(cmd, i2c_reg, ACK_CHECK_EN);
    // Send repeated start
    i2c_master_start(cmd);
    // now send device address (indicating read) & read data
    i2c_master_write_byte(cmd, ( RTC_PCF85263_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


esp_err_t rtc_pcf_write_reg_byte(uint8_t address, uint8_t regVal)
{
    esp_err_t ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (RTC_PCF85263_ADDR << 1) | I2C_MASTER_WRITE, 0x1);
    //i2c_master_write_byte(cmd, 0xA2, 0x1);

    i2c_master_write_byte(cmd, address, 0x1);
    i2c_master_write_byte(cmd, regVal, 0x1);

    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100/ portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}



void WriteTimeOn(void)
{
//	Wire.beginTransmission(RTC_Address);
//	Wire.write(0x10);//Set the address for writing as 10H
//	Wire.write(0x80);//Set WRTC1=1
//	Wire.endTransmission();
	rtc_pcf_write_reg_byte(0x10, 0x80);

//	Wire.beginTransmission(RTC_Address);
//	Wire.write(0x0F);//Set the address for writing as OFH
//	Wire.write(0x84);//Set WRTC2=1,WRTC3=1
//	Wire.endTransmission();
	rtc_pcf_write_reg_byte(0x0F, 0x84);
}

void WriteTimeOff(void)
{
//	Wire.beginTransmission(RTC_Address);
//	Wire.write(0x0F);   //Set the address for writing as OFH
//	Wire.write(0);//Set WRTC2=0,WRTC3=0
//	Wire.write(0);//Set WRTC1=0
//	Wire.endTransmission();
	rtc_pcf_write_reg_byte(0x0F, 0x00);
	rtc_pcf_write_reg_byte(0x10, 0x00);
}


void rtc_pcf_set(struct tm value)
{
	// if(is_time_valid(value) == false) 
    // {   
    //     return;
    // }
	if(value.tm_year > 100)
		value.tm_year -= 100;
	ESP_LOGW(TAG, "Set RTC: %02d - %02d/%02d/%02d %02d:%02d:%02d", value.tm_wday, value.tm_mday, value.tm_mon+1, value.tm_year + 2000, value.tm_hour, value.tm_min, value.tm_sec);



	rtc_pcf_write_reg_byte(SECOND_ADDR, dec_to_bcd(value.tm_sec));
	rtc_pcf_write_reg_byte(MINUTE_ADDR, dec_to_bcd(value.tm_min));
	rtc_pcf_write_reg_byte(HOUR_ADDR, dec_to_bcd(value.tm_hour));
	rtc_pcf_write_reg_byte(WEEK_ADDR, dec_to_bcd(value.tm_wday));
	rtc_pcf_write_reg_byte(DAY_ADDR, dec_to_bcd(value.tm_mday));
	rtc_pcf_write_reg_byte(MONTH_ADDR, dec_to_bcd(value.tm_mon));
	rtc_pcf_write_reg_byte(YEAR_ADDR, dec_to_bcd(value.tm_year));
	
}

struct tm rtc_pcf_read(void)
{
    static uint8_t data[7];

    memset(data,NULL,sizeof(data));

    rtc_pcf_read_reg_nbyte(SECOND_ADDR,data,7);

//	ESP_LOGW(TAG, "TES_BIT_OS: %02x", data[0]);
	Rtc.tm_sec = bcd_to_dec(data[0] & 0x7F);
	Rtc.tm_min = bcd_to_dec(data[1] & 0x7F);
	Rtc.tm_hour = bcd_to_dec(data[2] & 0x3F);
	Rtc.tm_mday = bcd_to_dec(data[3] & 0x3F); 
	Rtc.tm_wday = bcd_to_dec(data[4] & 0x07) + 1;  
	Rtc.tm_mon = bcd_to_dec(data[5] & 0x1F);
	Rtc.tm_year = bcd_to_dec(data[6])+ 2000;
	ESP_LOGW(TAG, "%02d - %02d/%02d/%02d %02d:%02d:%02d", Rtc.tm_wday, Rtc.tm_mday, Rtc.tm_mon, Rtc.tm_year, Rtc.tm_hour, Rtc.tm_min, Rtc.tm_sec);

	return Rtc;
}

void test_rtc(void)
{
    struct tm value;

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    i2c_scan();

    value.tm_sec = 0x00;
    value.tm_min = 20;
    value.tm_hour = 7;
    value.tm_wday = 5;
    value.tm_mday = 1;
    value.tm_mon = 1;
    value.tm_year = 23;
    rtc_pcf_set(value);

    // while(1){
    //     rtc_pcf_read();
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
    int i = 10;
    while(i--){
        rtc_pcf_read();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

