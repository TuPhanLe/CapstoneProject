/********** Include section ***************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "wifi.h"
#include "esp_smartconfig.h"
#include "mqtt.h"
#include "schedule.h"
#include "outdoor_farm_config.h"

#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

/********** Local Constant and compile switch definition section **************/
/* FreeRTOS event group to signal when we are connected & ready to make a request */
//static EventGroupHandle_t s_wifi_event_group;
/********** Local Type definition section *************************************/

/********** Local Macro definition section ************************************/
#define EXAMPLE_ESP_WIFI_SSID      "Wifi"
#define EXAMPLE_ESP_WIFI_PASS      "bbbbbbbbb"
#define EXAMPLE_ESP_MAXIMUM_RETRY  (0xFFFFFFFF)
static int s_retry_num = 0;


//static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "wifi station";

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT 			BIT0
#define WIFI_FAIL_BIT      			BIT1
#define ESPTOUCH_START_BIT 			BIT2
#define ESPTOUCH_GET_SSID_PASS_BIT 	BIT3
#define ESPTOUCH_DONE_BIT 			BIT4


/********** Local (static) function declaration section ***********************/
//static bool internet_connected = false;
/********** Local function definition section *********************************/

/********** Global function definition section ********************************/
// void set_InternetStatus(uint32_t status)
// {
// 	if(status)
// 		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
// 	else
// 		xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
// }

// bool get_InternetStatus(void)
// {
// 	EventBits_t uxBits = xEventGroupGetBits(s_wifi_event_group);// xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, false, 1/portTICK_RATE_MS);
// 	return (uxBits & WIFI_CONNECTED_BIT);
// }

esp_err_t get_rssi_wifi(void)
{
	wifi_ap_record_t wifidata;
	struct setting setting = get_setting_value();

	if(esp_wifi_sta_get_ap_info(&wifidata) == ESP_OK){
		ESP_LOGI(TAG, "rssi: %d", wifidata.rssi);
		setting.rssi = wifidata.rssi;
		set_setting_value(setting);
		return ESP_OK;
	}
	return ESP_FAIL;
}

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    struct info dev_info = get_dev_info();

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    	ESP_LOGI(TAG, "Connecting to the AP....");

    	esp_wifi_disconnect();
    	vTaskDelay(5000 / portTICK_PERIOD_MS);
        esp_wifi_connect();
//        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_event_sta_disconnected_t *disconnected = (wifi_event_sta_disconnected_t *) event_data;
        ESP_LOGE(TAG,"Disconnect reason: %d", disconnected->reason);
//        ESP_LOGE(TAG,"connect to the AP fail");
        dev_info.is_connected_sta_wifi = false;
		s_retry_num++;
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
        	//ESP_LOGE(TAG, "esp_wifi_scan_start: %d", esp_wifi_scan_start(&wifi_config, true));
			esp_wifi_connect();

			//xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
			ESP_LOGI(TAG, "Retry to connect to the AP %d", s_retry_num);
//            vTaskDelay(15000 / portTICK_PERIOD_MS);

        }
        else {
        	ESP_LOGI(TAG, "No retry to connect to the AP %d", s_retry_num);
        
        }

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        get_rssi_wifi();
        ESP_LOGI(TAG, "WIFI got ip: " IPSTR, IP2STR(&event->ip_info.ip));
        dev_info.is_connected_sta_wifi = true;
        s_retry_num = 0;
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
            ESP_LOGI(TAG, "Scan done");

	} else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
		ESP_LOGI(TAG, "Found channel");
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
		ESP_LOGI(TAG, "Got SSID and password");

		smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
		wifi_config_t wifi_config;
		//uint8_t ssid[33] = { 0 };
		//uint8_t password[65] = { 0 };
		//struct setting_t setting = get_setting_value();

		bzero(&wifi_config, sizeof(wifi_config_t));
		memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
		memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
		wifi_config.sta.bssid_set = evt->bssid_set;
		if (wifi_config.sta.bssid_set == true) {
			memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
		}

		//memcpy(setting.wifi_ssid, evt->ssid, sizeof(evt->ssid));
		//memcpy(setting.wifi_pass, evt->password, sizeof(evt->password));
		//ESP_LOGI(TAG, "SSID: %s", setting.wifi_ssid);
		//ESP_LOGI(TAG, "PASSWORD: %s", setting.wifi_pass);
		//set_setting_value(setting);
		//save_misc_region();


		ESP_ERROR_CHECK( esp_wifi_disconnect() );
		ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
		ESP_ERROR_CHECK( esp_wifi_connect() );
		//xEventGroupClearBits(s_wifi_event_group, ESPTOUCH_START_BIT);
		//xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_GET_SSID_PASS_BIT);
	}
	else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
		//xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);

		ESP_LOGE(TAG, "SC_EVENT_SEND_ACK_DONE<=====");
		esp_smartconfig_stop();
	}
    set_dev_info(dev_info);
}

void reconnect_wifi_sta(void)
{
	struct setting info = get_setting_value();

	//s_wifi_event_group = xEventGroupCreate();


//	vTaskDelay(5000 / portTICK_PERIOD_MS);
	//client_request_disconnect_mqtt();

//	esp_wifi_disconnect();
//	esp_wifi_stop();
//	esp_wifi_set_mode(WIFI_MODE_NULL);
//	vTaskDelay(5000 / portTICK_PERIOD_MS);
//	esp_event_handler_instance_t instance_any_id;
//	esp_event_handler_instance_t instance_got_ip;
//	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
//															ESP_EVENT_ANY_ID,
//															&event_handler,
//															NULL,
//															&instance_any_id));
//	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
//															IP_EVENT_STA_GOT_IP,
//															&event_handler,
//															NULL,
//															&instance_got_ip));

	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = WIFI_SSID_DEFAULT,
			.password = WIFI_PASS_DEFAULT,
//			.listen_interval = 3,
//			/* Setting a password implies station will connect to all security modes including WEP/WPA.
//			 * However these modes are deprecated and not advisable to be used. Incase your Access point
//			 * doesn't support WPA2, these mode can be enabled by commenting below line */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
//			.pmf_cfg = {
//				.capable = true,
//				.required = false
//			},
			.sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
		},
	};
	//wifi_config.sta.listen_interval = 3;
	//Assign ssid + password
	sprintf((char *)wifi_config.sta.ssid, "%s", info.wifi_ssid);
	sprintf((char *)wifi_config.sta.password, "%s", info.wifi_pass);

	ESP_LOGI(TAG, "Reconnect NEW WIFI AP: %s, %s<==", wifi_config.sta.ssid, wifi_config.sta.password);
	esp_wifi_set_mode(WIFI_MODE_NULL);
	// Stop the current WiFi connection
	esp_wifi_stop();

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//	ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));
	ESP_ERROR_CHECK(esp_wifi_start());
	//ESP_LOGE(TAG, "esp_wifi_scan_start: %d", esp_wifi_scan_start(&wifi_config, true));
	ESP_ERROR_CHECK(esp_wifi_connect());

	ESP_LOGI(TAG, "wifi_init_sta finished");

}

void wifi_scan_specific(const char *ssid)
{
    uint16_t ap_count = 0;
    wifi_ap_record_t ap_records[10];
    wifi_scan_config_t config;
	memset(&config, 0, sizeof(config));
	config.ssid = NULL;
	config.bssid = NULL;
	config.channel = 0; // all channels
	config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
	config.show_hidden = true;

    ESP_ERROR_CHECK(esp_wifi_scan_start(&config, true));
//    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_records));
    TickType_t tick_start = xTaskGetTickCount();
	TickType_t tick_end = tick_start + pdMS_TO_TICKS(5000); // Wait a maximum of 5 seconds for scan to complete
	while (xTaskGetTickCount() < tick_end) {
		vTaskDelay(pdMS_TO_TICKS(100)); // Wait 100ms before checking again

		ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
		if (ap_count > 0) {
			ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, ap_records));
			ESP_LOGI(TAG, "Found %d access points:", ap_count);

			for (int i = 0; i < ap_count; i++) {
				if (strcmp(ssid, (const char *)ap_records[i].ssid) == 0) {
					ESP_LOGI(TAG, "Found SSID \"%s\"", ssid);
					goto scan_done;
				}
			}
		}
	}

	ESP_LOGI(TAG, "Scan timed out before finding SSID \"%s\"", ssid);

    scan_done:

    ESP_ERROR_CHECK(esp_wifi_scan_stop());
}

void wifi_init_sta(void)
{
	struct setting info = get_setting_value();

	//s_wifi_event_group = xEventGroupCreate();

	esp_wifi_disconnect();
	esp_wifi_stop();
	esp_wifi_set_mode(WIFI_MODE_NULL);

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));


	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
	                                                        ESP_EVENT_ANY_ID,
	                                                        &event_handler,
	                                                        NULL,
	                                                        &instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
	                                                        IP_EVENT_STA_GOT_IP,
	                                                        &event_handler,
	                                                        NULL,
	                                                        &instance_got_ip));


	//ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	wifi_config_t wifi_config = {
		.sta = {
			.ssid = WIFI_SSID_DEFAULT,
			.password = WIFI_PASS_DEFAULT,
//			.listen_interval = 3,
//			/* Setting a password implies station will connect to all security modes including WEP/WPA.
//			 * However these modes are deprecated and not advisable to be used. Incase your Access point
//			 * doesn't support WPA2, these mode can be enabled by commenting below line */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
//			.pmf_cfg = {
//				.capable = true,
//				.required = false
//			},
			.sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
		},
	};
	//wifi_config.sta.listen_interval = 3;
	//Assign ssid + password
    
	sprintf((char *)wifi_config.sta.ssid, "%s", info.wifi_ssid);
	sprintf((char *)wifi_config.sta.password, "%s", info.wifi_pass);
    //printf("UUID: %s PASS:  %s\r\n",UUID_WIFI,PASS_WIFI);
	
	// sprintf((char *)wifi_config.sta.ssid, "%s",WIFI_SSID_DEFAULT);
	// sprintf((char *)wifi_config.sta.password, "%s", WIFI_PASS_DEFAULT);



	ESP_LOGW(TAG, "Connecting to AP: %s, %s", (char *)wifi_config.sta.ssid, (char *)wifi_config.sta.password);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//	ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));
	ESP_ERROR_CHECK(esp_wifi_start());


//	ESP_LOGE(TAG, "esp_wifi_scan_start: %d", esp_wifi_scan_start(&wifi_config, true));
	ESP_ERROR_CHECK(esp_wifi_connect());

	ESP_LOGI(TAG, "wifi_init_sta finished");
}

void smartconfig_example_task(void * parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    uint32_t retry = 0;
    //struct setting_t setting = get_setting_value();




    //esp_esptouch_set_timeout(5);
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    //xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_START_BIT);

    while (1) 
    {
//         uxBits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, 300/portTICK_RATE_MS/*portMAX_DELAY*/);
//         if(uxBits & ESPTOUCH_START_BIT) {
//         	//ESP_LOGI(TAG, "ESPTOUCH_START_BIT<=====");
//         	//led_b_toogle();
// 		}
//         if(uxBits & ESPTOUCH_GET_SSID_PASS_BIT) {
//         	//ESP_LOGI(TAG, "ESPTOUCH_GET_SSID_PASS_BIT<=====");
// 			//led_b(0);
// 		}
//         if(uxBits & WIFI_CONNECTED_BIT) {
//             ESP_LOGI(TAG, "WiFi Connected to ap<=====");
//             vTaskDelay(5000 / portTICK_RATE_MS);
//             esp_smartconfig_stop();
//             break;
//         }
//         if(uxBits & ESPTOUCH_DONE_BIT) {
//             ESP_LOGI(TAG, "smartconfig over<=====");
// //            esp_smartconfig_stop();
//             //vTaskDelete(NULL);
//             //vTaskDelay(3000/portTICK_PERIOD_MS);
//             //esp_restart();
//             break;
    }

    
}

void internet_init(void)
{
	//EventBits_t uxBits;

    wifi_init_sta();
    //ESP_LOGI(TAG, "Waiting for connect to ap: %d <====", get_InternetStatus());

    // uxBits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, false, 60000/portTICK_PERIOD_MS);
	// if(uxBits & WIFI_CONNECTED_BIT) {
	// 	//sync_time_server();
	// }
	// else{
	// 	ESP_LOGE(TAG, "Internet_init error, pls check wifi network");
	// }
}