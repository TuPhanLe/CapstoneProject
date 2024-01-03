#ifndef _BLUETOOTH
#define _BLUETOOTH




#define BLE_START_BIT           (1 << 0)
#define BLE_STOP_BIT            (1 << 1)
#define BLE_RECEIVE_BIT         (1 << 2)

#define CMD_SET_GET_WIFI  		0x01
#define CMD_SAVE_EXIT        	0x02
#define CMD_GET_DEV_INFO     	0x03
#define CMD_SET_TIME        	0x04
#define CMD_GPS_INFO    		0x05
#define CMD_GET_STATUS    		0x06
#define CMD_ACK_NAK     		0xFF

bool on_parseData(uint8_t *data, uint32_t len);
void set_ble_start_stop_bit(uint32_t status);
bool send_data_to_mobile(uint8_t *data, uint16_t length_data);
void ble_handle(void);
void start_ble_task(void);

#endif