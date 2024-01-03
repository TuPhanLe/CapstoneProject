#ifndef SCHEDULE_H
#define SCHEDULE_H

#define TAG_ALARM       "PENDING_ALARM"

#define HIGH_LEVEL      1
#define LOW_LEVEL       0

struct status{
    bool pending_alarm;
    bool pending_pump;
    uint8_t GPIO_OUTPUT_23;
};

struct pump{
    uint8_t GPIO_NUM;
    uint8_t GPIO_STATE;
    bool pending_alarm;
    bool pending_pump;
    bool return_data_flag;

    uint8_t pump;
    uint8_t alarm_pump;

    uint8_t start_hour;
    uint8_t start_minute;
    uint8_t start_second;

    uint8_t end_hour;
    uint8_t end_minute;
    uint8_t end_second;
};

struct info{
    bool is_connected_sta_wifi;
    bool is_connected_server;
    bool is_connected_control_node;
    bool is_connected_sensor_node1;
    bool is_connected_sensor_node2;

    bool get_connect_all_node_flag;
    uint32_t count_time;
    uint32_t get_data_sensor_period;
};

struct setting{
    char wifi_ssid[32];
    char wifi_pass[64];
    int8_t rssi;
};

//void show_hex(uint8_t *pbuf, uint16_t length);

struct info get_dev_info(void);
void set_dev_info(struct info value);
struct setting get_setting_value(void);
void set_setting_value(struct setting value);
struct pump get_pump_1_info(void);
void set_pump_1_info(struct pump value);
struct status get_dev_stt(void);
void set_dev_stt(struct status value);
void check_all_schedule(void);
void check_each_schedule(struct pump param_pump);
void clear_param_pump(struct pump param);

void stop_timer_1_second();
void restart_timer_1_second();
void init_timer_1_second();

#endif