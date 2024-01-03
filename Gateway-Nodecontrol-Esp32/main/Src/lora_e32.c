#include "lora_e32.h"

#define PIN_RECOVER 50
#define TIMEOUT	5000


const char *TAG1 = "LORA";

LoraParameter_t LoraParameter;
// User modify properly function subject to their MCU // 
uint32_t timeMs(void)			
{
#ifdef STM32F103		
	return HAL_GetTick();
#endif 	
#ifdef ESP32		
	return esp_timer_get_time();
#endif 	
}
void delay(uint16_t time)
{
#ifdef STM32F103	
	DELAY_MS_RUN(time);
#endif 	
#ifdef ESP32		
	vTaskDelay(time/ portTICK_PERIOD_MS);
#endif 	
}
void WriteM0(uint8_t state)
{
#ifdef STM32F103
	HAL_GPIO_WritePin(GPIOB, M0_Pin, (GPIO_PinState) state);
#endif 	
#ifdef ESP32		
	output_io_set_level(M0,state);
#endif	
}
void WriteM1(uint8_t state)
{
#ifdef STM32F103
	HAL_GPIO_WritePin(GPIOB, M1_Pin, (GPIO_PinState) state);
#endif 	
#ifdef ESP32		
	output_io_set_level(M1,state);
#endif
}
uint8_t GetAux(void)
{
#ifdef STM32F103
	uint8_t state = HAL_GPIO_ReadPin(GPIOB,AUX_Pin);
	return state;
#endif 	
#ifdef ESP32		
	uint8_t state = input_io_get_level(AUX);
	return state;
#endif
}
uint8_t LORA_GetByte(void) 
{
#ifdef STM32F103
		uint8_t byte;
		if(uart_available() > 0)
		{
			byte = uart_read();
		}
		return byte; 
#endif 
#ifdef ESP32
		// uint8_t byte[];
		// syn = uart_read_bytes(EX_UART_NUM,byte,1,50);
		// return byte[syn+1]; 
		uint8_t data[128]={};
		int length = 0;
		ESP_ERROR_CHECK(uart_get_buffered_data_len(EX_UART_NUM, (size_t*)&length));
		length = uart_read_bytes(EX_UART_NUM, data, length, 100);	
		return data[length];
#endif 
}

// Send byte to Lora module // 
void LORA_SendByte(uint8_t byte) 				 
{
#ifdef STM32F103
	HAL_UART_Transmit(&huart1,&byte,1,100);
#endif 
#ifdef ESP32
	
	uart_put(&byte, 1);
#endif 
}
// Send frame to Lora module // 
void LORA_SendFrame (uint8_t *pData, uint8_t size) 				
{
#ifdef STM32F103
	HAL_UART_Transmit(&huart1,pData,size,100);
#endif 
#ifdef ESP32
	uart_put(pData, size);
#endif 
}
// End of modify-time // 
void SetMode(uint8_t mode)
{
	delay(PIN_RECOVER);	
	if (mode == MODE_0_NORMAL) 
	{
        WriteM0(0);
        WriteM1(0);
	}
	else if (mode == MODE_1_WAKE_UP) 
	{
		WriteM0(1);
		WriteM1(0);
	}
	else if (mode == MODE_2_POWER_SAVING) {
		WriteM0(0);
		WriteM1(1);
	}
	else if (mode == MODE_3_PROGRAM) {
		WriteM0(1);
		WriteM1(1);
	}
	delay(PIN_RECOVER);	
}
// make sure AUX gets high after config or start off // 
void CompleteTask(uint16_t timeout) 		
{
	unsigned long t = 0u;
	if (GetAux() == 0 ) 
	{	
		while (GetAux() == 0) {
			delay(1);
			t++;
		    delay(PIN_RECOVER);
			if (t > timeout){
				break;
			}
		}
	}
	else 
	{
			delay(100);
	}
}
// After configuration parameter, you need to call this function to modify modle // 
void SaveParameters(uint8_t command) 				
{
	LoraParameter._SavePara = command;
	SetMode(MODE_3_PROGRAM);

	LORA_SendByte(LoraParameter._SavePara);
	LORA_SendByte(LoraParameter._AddressHigh);
	LORA_SendByte(LoraParameter._AddressLow);
	LORA_SendByte(LoraParameter._Speed);
	LORA_SendByte(LoraParameter._Channel);
	LORA_SendByte(LoraParameter._Options);

	delay(PIN_RECOVER);
	CompleteTask(200);
	SetMode(MODE_0_NORMAL);
	ESP_LOGI(TAG1, "Save done");
}

LoraParameter_t GetInforLora(void)
{
	return LoraParameter;
}
	// Reading model of Lora Master// 
uint8_t ReadInformationOfLora (uint8_t PROGRAM_COMMAND) 					
{
	SetMode(MODE_3_PROGRAM);
	
	if( PROGRAM_COMMAND == ReadConfigPara)
	{
		LORA_SendByte(ReadConfigPara);
		LORA_SendByte(ReadConfigPara);
		LORA_SendByte(ReadConfigPara);
	}

	else if(PROGRAM_COMMAND == ReadModuleVersion )
	{	
		LORA_SendByte(ReadModuleVersion);
		LORA_SendByte(ReadModuleVersion);
		LORA_SendByte(ReadModuleVersion);
	}
		SetMode(MODE_0_NORMAL);
return 1; 
}
void LoraConfigPara (uint8_t AddressHigh, uint8_t AddressLow, uint8_t Speed, uint8_t Channel, uint8_t Options)
{
	LoraParameter._AddressHigh = AddressHigh;
	LoraParameter._AddressLow = AddressLow;
	LoraParameter._Speed = Speed;
	LoraParameter._Channel = Channel;
	LoraParameter._Options = Options;
}
void sendFixedMessage(NodeParameters  *NodeLora, uint8_t *message, uint8_t size)
{
	SetMode(MODE_3_PROGRAM);
	//LORA_SendByte(WriteConfigAndSave);
	LORA_SendByte(WriteConfigAndNoSave);
	LORA_SendByte(NodeLora->_AddressHigh);
	LORA_SendByte(NodeLora->_AddressLow);
	LORA_SendByte(LoraParameter._Speed);
	LORA_SendByte(NodeLora->_Channel);
	LORA_SendByte(LoraParameter._Options);
	CompleteTask(500);
	SetMode(MODE_0_NORMAL);
	// send data // 
	uart_flush_input(EX_UART_NUM);
	min_send_frame(&(NodeLora->_min),NodeLora->_idNode,message,size);
	ESP_LOG_BUFFER_HEX("DATA SEND",message,size);
	ESP_LOGI(TAG1, "Send fixed done");
	delay(1000);
	// //back to default mode // 
	
	// LoraConfigPara(LoraParameter._AddressHigh,LoraParameter._AddressLow,LoraParameter._Speed,
	// 				LoraParameter._Channel,LoraParameter._Options);
	// // LoraConfigPara(0x00,0x01,0x1a,0x01,0x40);
	// SaveParameters(WriteConfigAndSave);
	// ESP_LOGI(TAG1, "BAck to default para done");
}
// Checkking lora version successfull // 
void switch_channel(NodeParameters  *NodeLora)
{
	printf("Switch chanel %d with frequency = %d MHz\r\n",NodeLora->_Channel,(410+NodeLora->_Channel));
    SetMode(MODE_3_PROGRAM);
	LORA_SendByte(WriteConfigAndSave);
	LORA_SendByte(NodeLora->_AddressHigh);
	LORA_SendByte(NodeLora->_AddressLow);
	LORA_SendByte(LoraParameter._Speed);
	LORA_SendByte(NodeLora->_Channel);
	LORA_SendByte(LoraParameter._Options);
	CompleteTask(500);
	SetMode(MODE_0_NORMAL);
}


/*
	Function send_cmd_wait_resp
	note: param timeout (second unit)
*/
void send_cmd_wait_resp(NodeParameters  *NodeLora, uint8_t *message, uint8_t size, uint16_t timeout)
{
	sendFixedMessage(NodeLora,message,size);
	while(timeout-- > 0)
	{	
		if(NodeLora->is_responded == true)
		{
			ESP_LOGW("send_cmd_wait_resp"," Receive response from node and break the loop");
			break;
		}
		ESP_LOGW("send_cmd_wait_resp"," Time out = %d",timeout);
		vTaskDelay(1000/ portTICK_PERIOD_MS);
	}
	NodeLora->is_responded = false;
}


void WakeUpLora(void)	
{
	for(uint8_t i = 0; i < 3; i++)
	{
		if(ReadInformationOfLora(ReadModuleVersion))
		{
			break;
		}
	}
}

// Node config // 
void nodeParaInit(NodeParameters *NodeLora,uint8_t AddressHigh, 
					uint8_t AddressLow, uint8_t Channel, 
					struct min_context min, uint8_t idNode)
{
	NodeLora->_AddressHigh = AddressHigh;
	NodeLora->_AddressLow = AddressLow;
	NodeLora->_Channel = Channel;
	NodeLora->_min = min;
	NodeLora->_idNode = idNode;

	NodeLora->is_responded = false;
}

#ifdef FIRMWARE_CONTROL_NODE
void min_application_handler(uint8_t min_id, 
							uint8_t *min_payload, 
							uint8_t len_payload,
							uint8_t port)
{
    uint8_t buffer_data[100];
    //ESP_LOGE(SENSOR_TAG,"at here");

	if(min_id == 0x03)
	{
		memset(buffer_data, NULL, sizeof(buffer_data));
		memcpy(buffer_data,min_payload,len_payload);
		process_data_from_gateway(buffer_data,len_payload);
	}   
}

void process_data_from_gateway(uint8_t *pbuf, uint8_t len_payload) 
{
	uint8_t buffer[6];
	uint8_t buffer_ret[2];
	struct pump pump1 = get_pump_1_info();


	memcpy(buffer,pbuf,len_payload);
	ESP_LOG_BUFFER_HEX("DATA RECEIVE",buffer,len_payload);
 
	pump1.pump = buffer[0];
	pump1.alarm_pump = buffer[1];
    pump1.start_hour = buffer[2];
    pump1.start_minute = buffer[3];
    pump1.start_second = 0;
    pump1.end_hour = buffer[4];
    pump1.end_minute = buffer[5];
    pump1.end_second = 0;

    pump1.GPIO_NUM = GPIO_PUMP_1;

	/* off led */
	if(pump1.pump == 0 && pump1.alarm_pump == 0){
		power_control_pump(pump1.GPIO_NUM,OFF_PUMP);
		pump1.pending_pump = 0;
		pump1.pending_alarm = 0;
		pump1.GPIO_STATE = LOW_LEVEL;
		
		ESP_LOGI("LORA","Pump 1 off");	
  	}
	else if(pump1.pump == 0 && pump1.alarm_pump == 1 && pump1.pending_alarm == 0){
    	power_control_pump(pump1.GPIO_NUM,ON_PUMP);
		pump1.pending_pump = 0;
		pump1.pending_alarm = 1;
		pump1.GPIO_STATE = HIGH_LEVEL;
		
		// Start alarm
		setTimeRTC(pump1.start_hour, pump1.start_minute);
       	ESP_LOGI("LORA","Pump 1 is alarming");	
	}
	else if(pump1.pump == 0 && pump1.alarm_pump == 1 && pump1.pending_pump == 1){
    	power_control_pump(pump1.GPIO_NUM,ON_PUMP);
		pump1.pending_pump = 0;
		pump1.pending_alarm = 1;
		pump1.GPIO_STATE = HIGH_LEVEL;
		
		// Start alarm
		setTimeRTC(pump1.start_hour, pump1.start_minute);
       	ESP_LOGI("LORA","Pump 1 switch from on normally to alarm");	
	}
	else if(pump1.pump == 1 && pump1.alarm_pump == 0 && pump1.pending_alarm == 0){
		power_control_pump(pump1.GPIO_NUM,ON_PUMP);
		pump1.pending_pump = 1;
		pump1.pending_alarm = 0;
		pump1.GPIO_STATE = HIGH_LEVEL;
		
		ESP_LOGI("LORA","Pump 1 on");
	}
	// else if(led == 1 && alarmLed == 1 && statusAlarm == 0){
	// 	setPowerLed(onLed);
	// 	statusLed = 1;
	// 	//glResetTimerFlag = 0;

	// 	// Start alarm
	// 	setTimeRTC(startHour,startMinute);
	// 	statusAlarm = 1;
	// }
	pump1.return_data_flag = true;
	set_pump_1_info(pump1);
}

void send_state_back_to_gateway(uint8_t *pbuf, uint8_t len_payload)
{
	sendFixedMessage(&control_node, pbuf, len_payload); 
}
#endif


void loraReceive(uint8_t *data, uint16_t length)
{
#ifdef FIRMWARE_GATEWAY
    min_poll(&GATEWAY,data,length);
#else
    min_poll(&CONTROL_NODE,data,length);
#endif
	
    // for (size_t i = 0; i < length; i++)
    // {
	// 	if(*data == 0xC0 || *data == 0xC2 )
	// 	{
	// 		ESP_LOGI(TAG1, "Default parameter of lora in hex ");
	// 	}
	// 	if(*data == 0xC3)
	// 	{
	// 		ESP_LOGI(TAG1, "Version of lora in hex");
	// 	}	
    //     ESP_LOGI("loraReceive", "para %d :  %x ",i,*(data++));
    // }
}
void loraInit(void)
{
	uart_set_callback(loraReceive);

#ifdef FIRMWARE_GATEWAY

    /* Define other nodes */
    nodeParaInit(&sensor_node1, ADDR_HIGH_SENSOR1, ADDR_LOW_SENSOR1, CHANEL_1, SENSOR_NODE_1, 1);
	nodeParaInit(&sensor_node2, ADDR_HIGH_SENSOR2, ADDR_LOW_SENSOR2, CHANEL_2, SENSOR_NODE_2, 2);
	nodeParaInit(&control_node, ADDR_HIGH_CONTROL, ADDR_LOW_CONTROl, CHANEL_3, CONTROL_NODE, 3);

	//printf("Set param gateway\r\n");
	LoraConfigPara(ADDR_HIGH_GATEWAY,ADDR_LOW_GATEWAY,0x1a,CHANEL_0,0x40);       
    SaveParameters(WriteConfigAndSave); 
    ReadInformationOfLora(ReadConfigPara);
#else
	nodeParaInit(&gateway, ADDR_HIGH_GATEWAY, ADDR_LOW_GATEWAY, CHANEL_0, GATEWAY, 0);
	nodeParaInit(&control_node, ADDR_HIGH_CONTROL, ADDR_LOW_CONTROl, CHANEL_3, CONTROL_NODE, 3);	
	//nodeParaInit(&gateway, ADDR_HIGH_GATEWAY, 0x01, CHANEL_3, GATEWAY, 0);	

	printf("Set param node control\r\n");
    LoraConfigPara(ADDR_HIGH_CONTROL,ADDR_LOW_CONTROl,0x1a,CHANEL_3,0x40);       
    //LoraConfigPara(ADDR_HIGH_CONTROL,0x01,0x1a,CHANEL_3,0x40);       
    SaveParameters(WriteConfigAndSave);
    ReadInformationOfLora(ReadConfigPara);

    
#endif
	
}