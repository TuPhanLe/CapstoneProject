#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
//#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

//#include "esp_log.h"
//#include "esp_sntp.h"
#include "mqtt_client.h"




#ifndef MAIN_INC_WIFI_H_
#define MAIN_INC_WIFI_H_

#ifdef __cplusplus
extern "C"
{
#endif

/********** Include section ***************************************************/
#include <stdbool.h>
#include <stdint.h>
/********** Constant  and compile switch definition section *******************/
#define SSID_LEN                    32
#define PASS_LEN                    64
/********** Type definition section *******************************************/


/********** Function declaration section **************************************/
void set_InternetStatus(uint32_t status);
bool get_InternetStatus(void);
void wifi_init_sta(void);
void reconnect_wifi_sta(void);
void internet_init(void);
esp_err_t get_rssi_wifi(void);
#ifdef __cplusplus
}
#endif


#endif /* MAIN_INC_WIFI_H_ */