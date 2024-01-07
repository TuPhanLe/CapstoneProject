#ifndef SPIFFS_H_
#define SPIFFS_H_

#define PATH_SCHEDULE       "/schedule"
#define PATH_WIFI           "/spiffs/wifi"


#define READ_ONLY            "r"    //Open a file as read only.The file must exist.

#define READ_AND_WRITE       "r+"   //Open a file as read and write. The file must exist.

#define WRITE_ONLY           "w"    //Open a file as write only.The file must exist. 
                                    //If a file with the same name already exists, 
                                    //its contents are discarded and the file is treated as a new empty file.

#define CREATE               "w+"   //Create an empty file and open it for update
                                    //If a file with the same name already exists
                                    //its contents are discarded and the file is treated as a new empty file.
  
#define APPEND               "a"    //Move to end of file on every write.
#define APPEND_AND_READ      "a+"   //Move to end of file on every write and can be read. 


void init_spiffs(void);
bool file_exist(const char* path);

bool read_file_flash(char *filename, uint8_t filename_length, char *pbuf, uint16_t length, char *mode);
bool save_file_flash(char *filename, uint8_t filename_length, char *pbuf, uint16_t length, char *mode);
void read_param_memory();
void save_flash_info_wifi(void); 
void spiffs(void);
#endif