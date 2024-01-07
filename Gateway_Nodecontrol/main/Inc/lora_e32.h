#ifndef __LORA_E32_	
#define __LORA_E32_
#include "stdio.h"
#include <string.h>
#include <stdint.h>

#define ESP32


#ifdef STM32F103
	#include "stm32f1xx_hal.h"
	#include "uart.h"
	#include "delay_timer.h"
	extern UART_HandleTypeDef huart1;
#endif 

#ifdef ESP32
	#include "GPIO.h"
	#include "app_uart.h"
	#include "driver/uart.h"
	#include "freertos/FreeRTOS.h"
	#include "freertos/timers.h"
	#include "freertos/task.h"
	#include "freertos/event_groups.h"
	#include "esp_log.h"
	#include "min.h"
	#include "outdoor_farm_config.h"
	#include "schedule.h"
	#include "mqtt.h"
	#include "util.h"
	#include "rtc_internal.h"
	#include "GPIO.h"
	
#endif 

#define CHANEL_0				0x00
#define CHANEL_1				0x01
#define CHANEL_2				0x02
#define CHANEL_3				0x03
#define CHANEL_4				0x04
#define CHANEL_5				0x05
#define CHANEL_6				0x06



typedef enum MODE_TYPE
{
  MODE_0_NORMAL 		= 0,
  MODE_1_WAKE_UP 		= 1,
  MODE_2_POWER_SAVING 	= 2,
  MODE_3_SLEEP 			= 3,
  MODE_3_PROGRAM 		= 3,
}MODE_TYPE;

enum PROGRAM_COMMAND
{
  WriteConfigAndSave  	= 0xC0,
  ReadConfigPara		= 0xC1,
  WriteConfigAndNoSave 	= 0xC2,
  ReadModuleVersion   	= 0xC3,
  WriteResetModule     	= 0xC4
};

typedef struct LoraParameter_t{
    	uint8_t _Save;
    	uint8_t _SavePara;	
    	uint8_t _AddressHigh;
    	uint8_t _AddressLow;
    	uint8_t _Speed;
    	uint8_t _Channel;
    	uint8_t _Options;
    	uint8_t _Attempts;
    	
    	// individual variables for all the options
    	uint8_t _ParityBit;
    	uint8_t _UARTDataRate;
    	uint8_t _AirDataRate;
    	uint8_t _OptionTrans;
    	uint8_t _OptionPullup;
    	uint8_t _OptionWakeup;
    	uint8_t _OptionFEC;
    	uint8_t _OptionPower;
    	uint16_t _Address;
    	uint8_t _Model;							 
    	uint8_t _Version;
    	uint8_t _Features;
    	uint8_t _buf;
       
    }LoraParameter_t;

typedef struct NodeParameters
{
	bool 	is_responded;
	uint8_t _AddressHigh;
	uint8_t _AddressLow;
	uint8_t _Channel;
	uint8_t _idNode;
	struct min_context _min;
}NodeParameters;

NodeParameters gateway; 
NodeParameters sensor_node1; 
NodeParameters sensor_node2; 
NodeParameters control_node; 

	/* User customize for each mcu */ 
	void 			delay(uint16_t time);
	uint32_t 		timeMs(void);
	void 			WriteM0(uint8_t state);
	void 			WriteM1(uint8_t state);
	uint8_t 		GetAux(void);
	uint8_t 		LORA_GetByte(void);
	void 			LORA_SendByte( uint8_t byte);
	void 			LORA_SendFrame (uint8_t *pData, uint8_t size);

	/* Core function for module lora e32 */ 
	void 			SetMode(uint8_t mode);
	void 			CompleteTask(uint16_t timeout);
	void 			SaveParameters(uint8_t command);
	uint8_t 		ReadInformationOfLora (uint8_t PROGRAM_COMMAND);
	LoraParameter_t GetInforLora(void);
	void 			loraInit(void);
	void 			LoraConfigPara (uint8_t AddressHigh, uint8_t AddressLow, 
									uint8_t Speed, uint8_t Channel, uint8_t Options);
	void 			sendFixedMessage(NodeParameters *NodeLora, uint8_t *message, uint8_t size);
	void 			send_cmd_wait_resp(NodeParameters  *NodeLora, uint8_t *message, uint8_t size, uint16_t timeout);
	void			switch_channel(NodeParameters  *NodeLora);
	void 			nodeParaInit(NodeParameters *NodeLora,uint8_t AddressHigh, 
					uint8_t AddressLow, uint8_t Channel, 
					struct min_context min, uint8_t idNode);
	void 			process_data_from_gateway(uint8_t *pbuf, uint8_t len_payload);
	void 			send_state_back_to_gateway(uint8_t *pbuf, uint8_t len_payload);
#endif
