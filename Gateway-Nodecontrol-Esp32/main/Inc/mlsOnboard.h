#ifndef MLSONBOARD_H
#define MLSONBOARD_H


#define MAX_MODEL_CHAR      32
#define MAX_HWID_CHAR       48

typedef struct
{
	char hwId[MAX_HWID_CHAR];    /*!< Device's Hardware-ID */
	char model[MAX_MODEL_CHAR];  /*!< Model of device */
//	char simId[MAX_SIMID_CHAR];
//	char imei[MAX_IMEI_CHAR];
} deviceInfo_t;

void read_info_esp_after_on_board(void);
#endif