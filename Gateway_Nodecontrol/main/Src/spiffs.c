#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "dirent.h"

#include "spiffs.h"
#include "schedule.h"
#include "outdoor_farm_config.h"

static const char *TAG = "spiffs";
#define CONFIG_EXAMPLE_SPIFFS_CHECK_ON_START

void init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

#ifdef CONFIG_EXAMPLE_SPIFFS_CHECK_ON_START
    ESP_LOGI(TAG, "Performing SPIFFS_check().");
    ret = esp_spiffs_check(conf.partition_label);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
        return;
    } else {
        ESP_LOGI(TAG, "SPIFFS_check() successful");
    }
#endif

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }


    // Check consistency of reported partiton size info.
    if (used > total) {
        ESP_LOGW(TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        // Could be also used to mend broken files, to clean unreferenced pages, etc.
        // More info at https://github.com/pellepl/spiffs/wiki/FAQ#powerlosses-contd-when-should-i-run-spiffs_check
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return;
        } else {
            ESP_LOGI(TAG, "SPIFFS_check() successful");
        }
    }
}

bool file_exists(const char *path)
{
    struct stat st;
    if(stat(path, &st) == 0){
        ESP_LOGI(TAG, "Found file: %s",path);
        return true;
    }
    else{
        ESP_LOGI(TAG, "Not found file: %s",path);
    }
    return false;
}

void list_all_file(char *path)
{
    DIR* dir = opendir(path);
    if(dir == NULL){
        return;
    }
    while(true){
        struct dirent* de = readdir(dir);
        if(!de){
            break;
        }

        struct stat st;
        char full_path[512];
        sprintf(full_path, "%s/%s", PATH_SCHEDULE, de->d_name);
        if(stat(full_path, &st)==0){
            printf("File %s has size %ld bytes.\n", full_path, st.st_size);
        }
        else{
            printf("Could not get file size. \n");
        }
    }
}

bool read_file_flash(char *filename, uint8_t filename_length, char *pbuf, uint16_t length, char *mode)
{
    char buffer[30];
    char path[50];

    sprintf(path, "/spiffs/");

    for(int i = 0; i < filename_length; i++){
        buffer[i] = *filename;
        filename++;
    }
    strcat(path,buffer);
    
    ESP_LOGI(TAG, "Reading file: %s",path);
    FILE* ptrFile = fopen(path, mode);
    if (ptrFile == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return false;
    }
   
    fgets(pbuf, length, ptrFile);
    //printf("read file successfull \r\n");
    fclose(ptrFile);
    
    return true;
}

bool save_file_flash(char *filename, uint8_t filename_length, char *pbuf, uint16_t length, char *mode)
{
    int total_elements_successfully_written = 0;
    char buffer[30];
    char path[50];

    sprintf(path, "/spiffs/");

    for(int i = 0; i < filename_length; i++){
        buffer[i] = *filename;
        filename++;
    }
    strcat(path,buffer);

    ESP_LOGW(TAG, "Opening file: %s",path);
    FILE* ptrFile = fopen(path, mode);
    if (ptrFile == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return false;
    }
    //ESP_LOGI(TAG, "Opening file %s successful",filename);

    total_elements_successfully_written = fwrite(pbuf, 1, length, ptrFile);
    //printf("total_elements_successfully_written: %d\r\n",total_elements_successfully_written);

    fclose(ptrFile);

    if(total_elements_successfully_written == 0 || total_elements_successfully_written != length){
        ESP_LOGE(TAG, "write file fail");
        
        return false;
    }

    ESP_LOGI(TAG, "Save file flash successfully");
    return true;
}

void read_param_memory()
{
    bool ret;
    struct setting value = get_setting_value();

    char read_buffer[100];

    memset(read_buffer,NULL,sizeof(read_buffer));
    ret = read_file_flash(PATH_SSID, sizeof(PATH_SSID), read_buffer, sizeof(read_buffer), READ_ONLY);
    if(ret == true)
    {
        ESP_LOGW(TAG,"READ SSID WIFI FROM FLASH: %s",read_buffer);
        memcpy(value.wifi_ssid,read_buffer,strlen(read_buffer));
    }
    else{
        memcpy(value.wifi_ssid,WIFI_SSID_DEFAULT,strlen(WIFI_SSID_DEFAULT));
        ESP_LOGE(TAG,"Read ssid wifi fail and then set default ssid: %s",value.wifi_ssid);
    }


    memset(read_buffer,NULL,sizeof(read_buffer));
    ret = read_file_flash(PATH_PASS, sizeof(PATH_PASS), read_buffer, sizeof(read_buffer), READ_ONLY);
    if(ret == true)
    {
        ESP_LOGW(TAG,"READ PASS WIFI FROM FLASH: %s",read_buffer);
        memcpy(value.wifi_pass,read_buffer,strlen(read_buffer));
    }
    else{
        memcpy(value.wifi_pass,WIFI_PASS_DEFAULT,strlen(WIFI_PASS_DEFAULT));
        ESP_LOGE(TAG,"Read pass wifi fail and then set default pass: %s",value.wifi_pass);
    }

    set_setting_value(value);
}

void save_flash_info_wifi(void)
{
    bool ret;
    struct setting value = get_setting_value();

    ret = save_file_flash(PATH_SSID, sizeof(PATH_SSID), value.wifi_ssid, strlen(value.wifi_ssid), WRITE_ONLY);

    ret = save_file_flash(PATH_PASS, sizeof(PATH_PASS), value.wifi_pass, strlen(value.wifi_pass), WRITE_ONLY);
   
}


void spiffs(void)
{
    
    // Use POSIX and C standard library functions to work with files.
    // First create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/wifi.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "windpink 11111111\n");
    
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) {
        // Delete it if it exists
        unlink("/spiffs/foo.txt");
    }

    // Rename original file
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/spiffs/wifi.txt", "/spiffs/foo.txt") != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // Open renamed file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    // All done, unmount partition and disable SPIFFS

    //esp_vfs_spiffs_unregister(conf.partition_label);
    //ESP_LOGI(TAG, "SPIFFS unmounted");
}