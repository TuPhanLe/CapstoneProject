#ifndef _MQTT
#define _MQTT

#define Username                "client01"
#define Password                "123123Aa"


#define gateway_to_server_topic         "gateway/to/server"
#define gateway_to_server_topic_v1      "data_gateway/to/server"
#define server_to_gateway_topic         "server/to/gateway"

/* Define command gateway communicate server */
#define ACK             0x01
#define NACK            0x00




#define GET_STATUS_ALL_NODE             0x00
#define SET_SENSOR_GET_PERIOD           0x01
#define SEND_DATA_SENSOR_TO_SERVER      0x02
#define SEND_NODE_CONTROL               0x03

void set_bit_pending_server(void);
void set_bit_pending1_server(void);
bool parse_data_from_server(char *buf, uint16_t length_data);
bool process_data_from_server(uint8_t *pbuf, uint8_t length);
bool send_data_to_server(char *topic, uint8_t command, uint8_t *pdata, uint8_t length_data);

void process_data_from_node(uint8_t *pbuf, uint8_t len_payload);

int publishMQTT(char *topic,char *data);
void mqtt_init();
void mqtt_create_task(void);
#endif