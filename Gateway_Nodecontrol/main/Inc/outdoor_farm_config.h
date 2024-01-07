#ifndef OUTDOOR_FARM_CONFIG_H
#define OUTDOOR_FARM_CONFIG_H

#include <stdlib.h>
#include <stdbool.h>

//#define USE_EXTERNAL_RTC
#define FIRMWARE_GATEWAY
//#define FIRMWARE_CONTROL_NODE

#define SENSOR_DATA_MAX_LENGTH      10





#define ON_PUMP                      1
#define OFF_PUMP                     0

#define GPIO_PUMP_1                 GPIO_NUM_13

#define ADDR_HIGH_GATEWAY           0x00
#define ADDR_LOW_GATEWAY            0x00

#define ADDR_HIGH_SENSOR1           0x00
#define ADDR_LOW_SENSOR1            0x01

#define ADDR_HIGH_SENSOR2           0x00
#define ADDR_LOW_SENSOR2            0x02

#define ADDR_HIGH_CONTROL           0x00
#define ADDR_LOW_CONTROl            0x03

#define PATH_SSID               "ssid.txt"
#define PATH_PASS               "pass.txt"

#define WIFI_SSID_DEFAULT       "windpink"
#define WIFI_PASS_DEFAULT       "11111111"

#define SENSOR_TAG              "SENSOR_NODE"
#define SEND_SERVER_TAG         "PENDING_SERVER"

#define NAME_BLE_DEVICE         "Khối trung tâm-BLE"
#endif