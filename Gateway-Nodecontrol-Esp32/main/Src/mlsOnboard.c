#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_crt_bundle.h"

#include "mlsOnboard.h"

esp_err_t getHardwareID(const char* model, char* hardwareID);
esp_err_t collectChipInfo(deviceInfo_t* argDeviceInfo);





esp_err_t getHardwareID(const char* model, char* hardwareID)
{
    uint8_t macAddr[6];
    int retVal = 0;
    esp_err_t errorCode = ESP_OK;

    if((errorCode = esp_efuse_mac_get_default(macAddr)) != ESP_OK){
        ESP_LOGE("ON_BOARD", "get base MAC Address failed");
        return errorCode;
    }

    retVal = snprintf(hardwareID, 48,
            "%s-%02X-%02X-%02X-%02X-%02X-%02X",
            model,
            macAddr[0],
            macAddr[1],
            macAddr[2],
            macAddr[3],
            macAddr[4],
            macAddr[5]);

    if((retVal < 0) || (retVal > 48)){
        ESP_LOGE("ON_BOARD", "get hardware ID failed %d", retVal);
        return ESP_FAIL;
    }

    return ESP_OK;
}


/*___________collectChipInfo __________*/
esp_err_t collectChipInfo(deviceInfo_t* argDeviceInfo)
{
    esp_chip_info_t tmp_info;
    esp_chip_info(&tmp_info);
    const char *ptr = NULL;

    //set model information
    switch (tmp_info.model)
    {
    case CHIP_ESP32:
    	ptr = "ESP32";
    	break;
    case CHIP_ESP32S2:
    	ptr = "ESP32-S2";
    	break;
    case CHIP_ESP32S3:
    	ptr = "ESP32-S3";
    	break;
    case CHIP_ESP32C3:
    	ptr = "ESP32-C3";
    	break;
    case CHIP_ESP32H2:
    	ptr = "ESP32-H2";
    	break;
    default:
    	ptr = "ESP32";
    	break;
    }

    //Ignore sim_id and imei
    //argDeviceInfo->simId = NULL;
    //argDeviceInfo->imei   = NULL;

    if (strlen(ptr) > MAX_MODEL_CHAR)
    {
    	strncpy(argDeviceInfo->model, ptr, MAX_MODEL_CHAR-1);
    	*(argDeviceInfo->model + MAX_MODEL_CHAR) = '\0';
    }
    else
    {
    	strcpy(argDeviceInfo->model, ptr);
    }

    return getHardwareID(argDeviceInfo->model, argDeviceInfo->hwId);
}


void read_info_esp_after_on_board(void)
{
    deviceInfo_t deviceInfo_t;
    collectChipInfo(&deviceInfo_t);

    printf("Model: %s\r\n",deviceInfo_t.model);
    printf("Hardware ID: %s\r\n",deviceInfo_t.hwId);
}