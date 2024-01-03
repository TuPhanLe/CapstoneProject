#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "esp32/rom/rtc.h"
#include "esp32/rom/crc.h"

#include "sys/time.h"
#include "sdkconfig.h"
#include <time.h>

#include "wifi.h"
#include "mqtt.h"
#include "util.h"
#include "schedule.h"
#include "lora_e32.h"


static const char *TAG = "MQTTS_PROTOCOL";
static esp_mqtt_client_handle_t client;
#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipseprojects_io_pem_start[]  = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipseprojects_io_pem_start[]   asm("_binary_mqtt_eclipseprojects_io_pem_start");
#endif
extern const uint8_t mqtt_eclipseprojects_io_pem_end[]   asm("_binary_mqtt_eclipseprojects_io_pem_end");
//
// Note: this function is for testing purposes only publishing part of the active partition
//       (to be checked against the original binary)
//

#define BIT_PENDING_SERVER                   (1 << 0)
#define BIT_PENDING1_SERVER                  (1 << 1)

static uint8_t buff_data[100];
static uint8_t length;
static EventGroupHandle_t s_mqtt_event_group;
TaskHandle_t mqtt_handle_task;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    struct info dev_info = get_dev_info();
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");  
            dev_info.is_connected_server = true;
            //msg_id = esp_mqtt_client_subscribe(client, gateway_to_server_topic, 0);   
            //ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, server_to_gateway_topic, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            set_dev_info(dev_info);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            dev_info.is_connected_server = false;
            set_dev_info(dev_info);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            //msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            if (strncmp(event->topic, server_to_gateway_topic, event->topic_len) == 0) {
                char *data = strndup(event->data, event->data_len);
			    printf("Message from server: %swith length data = %d\n", data,event->data_len);
                parse_data_from_server(data,event->data_len);
                free(data);
            }
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_ESP_TLS) {
                ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
                ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
            } else {
                ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

bool parse_data_from_server(char *buf, uint16_t length_data)
{
    //char buffer[];
    uint8_t length_data_hex;
    int index;
    char str[3];
    uint8_t buffer_hex[50];
    memset(buffer_hex,NULL,sizeof(buffer_hex));

    //printf("buf: %s\r\n",buf);
    for (index = 1; index < length_data; index++){
        if(SplitnData(buf,' ',index,' ',sizeof(str),str) != 0){
            buffer_hex[index-1] = strtol(str,NULL,16);
        }
        else{
            
            break;
        }
       
    }
    length_data_hex = index - 1;
    printf("Show data hex after parsing\r\n");
    for (int j = 0; j < length_data_hex; j++)
    {
        printf("%02x ",buffer_hex[j]);
    }

    if(length_data_hex == 1){
        return false;
    }
    else{
        if(process_data_from_server(buffer_hex,length_data_hex) == true){
            printf("Process data from server successfully \r\n");   
        }
    }
    return true;

}


bool process_data_from_server(uint8_t *pbuf, uint8_t length)
{
    uint8_t command;
    uint8_t data_length;
    
    command = *(pbuf++);
    data_length = *(pbuf++);
    
    printf("\r\nCommand = %02x",command);
    printf(" length data = %02x\r\n",data_length);
    switch (command)
    {
        case GET_STATUS_ALL_NODE: /* Get status of all node */
        {
            if(data_length == 0x01){
                if(*pbuf == 0xFF){
                    stop_timer_1_second();
                    uint8_t buffTestMin[6];
                    uint8_t data[3];
                    struct info value = get_dev_info();

                    value.get_connect_all_node_flag = true;
                    set_dev_info(value);

                    memset(buffTestMin, NULL, sizeof(buffTestMin));
                    send_cmd_wait_resp(&control_node, buffTestMin, sizeof(buffTestMin), 10);
                    send_cmd_wait_resp(&sensor_node1, buffTestMin, sizeof(buffTestMin), 10);
                    send_cmd_wait_resp(&sensor_node2, buffTestMin, sizeof(buffTestMin), 10);
                    value = get_dev_info();
                    //memset(data,NULL,sizeof(data));
                    data[0] = value.is_connected_control_node;
                    data[1] = value.is_connected_sensor_node1;
                    data[2] = value.is_connected_sensor_node2;

                    if(send_data_to_server(gateway_to_server_topic, GET_STATUS_ALL_NODE, data, 3)== false)
                    {
                        ESP_LOGE("GET_STATUS_ALL_NODE", " Publish fail");
                    }

                    value.get_connect_all_node_flag = false;
                    value.is_connected_control_node = false;
                    value.is_connected_sensor_node1 = false;
                    value.is_connected_sensor_node2 = false;
                    set_dev_info(value);
                    restart_timer_1_second();
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }  
        }
        break;
        case SET_SENSOR_GET_PERIOD: /* Set period to get data from sensor node */
        {
            
            uint8_t buffer[4];
            uint8_t data_return[1] = {0x01};
            struct info value = get_dev_info();

            if(data_length >= 0){
                for(int i = 0; i < data_length;i++)
                {
                    buffer[i] = *(pbuf++);
                }
                value.get_data_sensor_period = Util_buildUint32(buffer[3],buffer[2],buffer[1],buffer[0]);
                printf("value.get_data_sensor_period = %d seconds\r\n",value.get_data_sensor_period);
                set_dev_info(value);

                send_data_to_server(gateway_to_server_topic,0xFF,data_return,1);
            }
            else{
                data_return[0] = 0x00;
                send_data_to_server(gateway_to_server_topic,0xFF,data_return,1);
                return false;
            }
        }
        break;   
        case SEND_NODE_CONTROL: /* send data to node control */
        {
            uint8_t buffer[6];
            bool cmd_off_pump = true;
            uint8_t data_return[1] = {0x01};

            if(data_length == 6){
                for(int i = 0; i < data_length;i++)
                {
                    buffer[i] = *(pbuf++);
                    printf("%d ",buffer[i]);
                    if(buffer[i] != 0)
                    {
                        cmd_off_pump = false;
                    }
                    
                } 
                if(cmd_off_pump == true)
                {
                    restart_timer_1_second();    
                }
                else{
                    stop_timer_1_second();
                }
                sendFixedMessage(&control_node, buffer, sizeof(buffer));
            }
            else{
                data_return[0] = 0x00;
                send_data_to_server(gateway_to_server_topic,0xFF,data_return,1);
                return false;
            }
            
        } 
        break;
        default:
            return false;
        break;
    }
    return true;
}

bool send_data_to_server(char *topic, uint8_t command, uint8_t *pdata, uint8_t length_data)
{
    char data[100];
    char str[2];
    char space[] = " ";
    int ret;

    struct info dev_info = get_dev_info();
    
    sprintf(data, "%02x%02x",command,length_data);
    
    //printf("%s \r\n", data);
    for(int i = 0; i < length_data; i++){
        sprintf(str,"%02x",*pdata);
        strcat(data,str);
        pdata++;
    }

    //printf("%s \r\n", data);
    if(dev_info.is_connected_server == false){
        ESP_LOGE("MQTT_PROTOCOL", "No connect to server");
        return false;
    }
    get_rssi_wifi();
    
    ESP_LOGI("send_data_to_server","Topic: %s and payload: %s",topic,data);
    ret = publishMQTT(topic,data);
    if(ret == -1 || ret == -2){
        return false;
    }
    return true;
}
void min_application_handler(uint8_t min_id, uint8_t *min_payload, 
															uint8_t len_payload, uint8_t port)
{
    uint8_t buffer_data[100];
    struct info dev_info = get_dev_info();
	
	memset(buffer_data, NULL, sizeof(buffer_data));
	buffer_data[0] = min_id;
	memcpy(&buffer_data[1],min_payload,len_payload);
    ESP_LOG_BUFFER_HEX("min_payload",min_payload,len_payload);

    if(dev_info.get_connect_all_node_flag == true)
    {
        switch (min_id)
        {
            case ADDR_LOW_SENSOR1: //ADDR_LOW_SENSOR1
            {	
                sensor_node1.is_responded = true;
                dev_info.is_connected_sensor_node1 = true;
            }    
            break;
            case ADDR_LOW_SENSOR2:
            {
                sensor_node2.is_responded = true;
                dev_info.is_connected_sensor_node2 = true;
            }    
            break;
            case ADDR_LOW_CONTROl:
            {
                control_node.is_responded = true;
                dev_info.is_connected_control_node = true;
            }    
            break;
            default:
            break;
        }
        set_dev_info(dev_info);
    }
    else
    {
        process_data_from_node(buffer_data,len_payload);
    }
}

void process_data_from_node(uint8_t *pbuf, uint8_t len_payload) 
{
	uint8_t buffer[100];
    struct info dev_info = get_dev_info();

	memset(buffer,NULL,sizeof(buffer));
    buffer[0] = *(pbuf++);

	memcpy(&buffer[1],pbuf,len_payload);

    switch (buffer[0])
    {
        case ADDR_LOW_GATEWAY:
        {

        }    
        break;
        case ADDR_LOW_SENSOR1: //ADDR_LOW_SENSOR1
        {	
            sensor_node1.is_responded = true;
            ESP_LOGI(SENSOR_TAG,"Data from sensor 1");
            if(len_payload == SENSOR_DATA_MAX_LENGTH)
            {
                memset(buff_data,NULL,sizeof(buff_data));
                memcpy(buff_data,buffer,len_payload);
                //dev_info.is_connected_sensor_node1 = true;
                
                if(dev_info.is_connected_sta_wifi && dev_info.is_connected_server)
                {
                    set_bit_pending_server();
                }
				else
                {
                    return false;
                }
                //set_dev_info(dev_info);
            }
            else
            {
                ESP_LOGE("process_data_from_node","Data format wrong, length receive data = %d",len_payload);
            }

        }    
        break;
        case ADDR_LOW_SENSOR2:
        {
            sensor_node2.is_responded = true;
            ESP_LOGI(SENSOR_TAG,"Data from sensor 2");
            if(len_payload == SENSOR_DATA_MAX_LENGTH)
            {
                memset(buff_data,NULL,sizeof(buff_data));
                memcpy(buff_data,buffer,len_payload);
                //dev_info.is_connected_sensor_node2 = true;
                //set_dev_info(dev_info);
				if(dev_info.is_connected_sta_wifi && dev_info.is_connected_server)
                {
                    set_bit_pending_server();
                }
				else
                {
                    return false;
                }
                
            }
            else
            {
                ESP_LOGE("process_data_from_node","Data format wrong, length receive data = %d",len_payload);
            }
            
        }    
        break;
        case ADDR_LOW_CONTROl:
        {
            control_node.is_responded = true;
            ESP_LOGI("CONTROL_NODE","Data from control 1");
            if(len_payload == 2)
            {
                memset(buff_data,NULL,sizeof(buff_data));
                if(buffer[1] == 0 && buffer[2] == 0)
                {
                    restart_timer_1_second();
                }
                //printf("buff1 %d ",buffer[1]);
                //printf("buff2 %d\r\n",buffer[2]);
                memcpy(buff_data,&buffer[1],len_payload);
                //dev_info.is_connected_control_node = true;
                //set_dev_info(dev_info);  
				if(dev_info.is_connected_sta_wifi && dev_info.is_connected_server)
                {
                    set_bit_pending1_server();
                }
				else
                {
                    return false;
                }
            }
            else
            {
                ESP_LOGE("process_data_from_node","Data format wrong, length receive data = %d",len_payload);
            }
            
        }    
	    break;
        default:
    	break;
    }
}

void set_bit_pending_server(void)
{
    xEventGroupSetBits(s_mqtt_event_group, BIT_PENDING_SERVER);
}

void set_bit_pending1_server(void)
{
    xEventGroupSetBits(s_mqtt_event_group, BIT_PENDING1_SERVER);
}


void mqtt_task(void *pvParameters) {
    EventBits_t bits;
    s_mqtt_event_group = xEventGroupCreate();
    while (1)
    {
        bits = xEventGroupWaitBits(s_mqtt_event_group, BIT_PENDING_SERVER | BIT_PENDING1_SERVER, pdTRUE, pdFALSE, portMAX_DELAY);
        //EventBits_t currentBits = xEventGroupGetBits(s_mqtt_event_group);
        if ((bits & BIT_PENDING_SERVER) != 0) {
            printf("BIT_PENDING_SERVER has been set\n");
	        vTaskDelay(5000/ portTICK_PERIOD_MS);
            
            ESP_LOG_BUFFER_HEX("PUBLISH DATA SENSOR",buff_data,11);
            if(send_data_to_server(gateway_to_server_topic_v1,0x02,buff_data,11) == false)
            {
                ESP_LOGE(SEND_SERVER_TAG,"Publish data node sensor fail\r\n");
                //printf("Send data node sensor fail\r\n");
            }
            else
            {
                ESP_LOGI(SEND_SERVER_TAG,"Publish data node sensor successfully\r\n");
            }
            //xEventGroupClearBits(s_mqtt_event_group, EVENT_BIT_2);
        }
        else if ((bits & BIT_PENDING1_SERVER) != 0) {
            ESP_LOG_BUFFER_HEX("PUBLISH DATA NODE CONTROL",buff_data,2);  //03020100
            if(send_data_to_server(gateway_to_server_topic,0x03,buff_data,2) == false)
            {
                ESP_LOGE(SEND_SERVER_TAG,"Publish data control fail\r\n");
                //printf("Send data node sensor fail\r\n");
            }
            else
            {
                ESP_LOGI(SEND_SERVER_TAG,"Publish data control successfully\r\n");
                //printf("Send data node sensor 1 successfully\r\n");
            }
        }
    }

    //vTaskDelete(NULL);
    //vTaskDelay(100/portTICK_PERIOD_MS);
}

void mqtt_create_task(void)
{

    xTaskCreate(mqtt_task, "mqtt_handle_task", 8192, NULL, 10, NULL);
}

static void mqtt_app_start(void)
{

    printf("\r\n");
    ESP_LOGI("BROKER INFO", "Host broker: %s",CONFIG_BROKER_URI);
    ESP_LOGI("BROKER INFO", "Port: 8883");
    ESP_LOGI("BROKER INFO", "Username: %s",Username);
    ESP_LOGI("BROKER INFO", "Password: %s",Password);
    printf("\r\n");

    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URI,
        .cert_pem = (const char *)mqtt_eclipseprojects_io_pem_start,
        .username = Username,
        .password = Password,
        // .session.disable_clean_session = true,
        // .session.keepalive = 5000,
        // .network.reconnect_timeout_ms = 15000,
        .event_handle = mqtt_event_handler,
    };

    //ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

int publishMQTT(char *topic,char *data)
{
    int ret;
    //default qos = 1 and retain = 0;
    ret = esp_mqtt_client_publish(client, topic, data, strlen(data), 0, 0);
    printf("Get more delay message id %d\r\n",ret);
    // vTaskDelay(2000/portTICK_PERIOD_MS);
    return ret;
}


void mqtt_init(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    //ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
    
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    //ESP_ERROR_CHECK(example_connect());

    mqtt_app_start();
    mqtt_create_task();
}

