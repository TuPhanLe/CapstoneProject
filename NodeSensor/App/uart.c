#include "uart.h"
#include "DHT.h"

#define max_len 64
static uint8_t buff[max_len];
static uint8_t bufSending[10];
static uint8_t buf_test[] = {0x00};
static uint8_t bufForSend[] = {0x11};

struct min_context node_1;
struct min_context node_2;

extern uint8_t uart_flag;
ringbuffer_t ring;

data_separated temp16bit;
data_separated humi16bit;
data_separated soil16bit;
data_separated light32bit;

sensor_t sensorTestData;


void receive_data(uint8_t data)
{
	ringbuffer_push(&ring,data);
}
uint16_t uart_available(void)
{
	return Is_ringbuf_available(&ring);
}
uint8_t uart_read(void)
{
	uint8_t data;
	ringbuffer_pop(&ring,&data);
	return data;
}
void uart_init(void)
{
	ringbuffer_init(&ring,buff,max_len);
	min_init_context(&node_1, 1); 
	min_init_context(&node_2, 2);	
}

uint32_t min_time_ms(void)
{
	return HAL_GetTick();
}
uint16_t min_tx_space(uint8_t port)
{
	return 512;
}
void min_tx_start(uint8_t port){}
void min_tx_finished(uint8_t port){}
void min_tx_byte(uint8_t port, uint8_t byte)
{
	USART1->DR = byte;
	while(!(USART1->SR & USART_SR_TC));	
}
void separate_16_to_8(uint16_t data16bit,data_separated *data)
{
    data->data_8bitLSB = data16bit & 0x00FF;
    data->data_8bitMSB = (data16bit & 0xFF00) >> 8;  
}
void separate_32_to_8(uint32_t data32Bit,data_separated *data)
{
    data->data_8bitLSB = data32Bit & 0x000000FF;
    data->data_8bit3 = (data32Bit & 0x0000FF00) >> 8;  
		data->data_8bit2 = (data32Bit & 0x00FF0000) >> 16;  
		data->data_8bitMSB = (data32Bit & 0xFF000000) >> 24;
}
//uint16_t merge8bit(uint8_t var8bitMSB , uint8_t var8bitLSB)
//{   
//   uint16_t data = 0x0000;
//   data = (data | var8bitLSB) | (var8bitMSB << 8);
//   return data;
//}
//---------------------------------------------------------------------------------------------------//


void uart_send_min(sensor_t sensor)
{
		separate_16_to_8(sensor.system_temperature,&temp16bit);
		separate_16_to_8(sensor.system_huminity,&humi16bit);
		separate_16_to_8(sensor.system_soil,&soil16bit);
		separate_32_to_8(sensor.system_light,&light32bit);
		
		bufSending[0] = temp16bit.data_8bitMSB;
		bufSending[1] = temp16bit.data_8bitLSB;
		bufSending[2] = humi16bit.data_8bitMSB;
		bufSending[3] = humi16bit.data_8bitLSB;	
		bufSending[4] = soil16bit.data_8bitMSB;
		bufSending[5] = soil16bit.data_8bitLSB;
		bufSending[6] = light32bit.data_8bitMSB;
		bufSending[7] = light32bit.data_8bit2;	
		bufSending[8] = light32bit.data_8bit3;
		bufSending[9] = light32bit.data_8bitLSB;	
		
		min_send_frame(&node_1, 1, bufSending, sizeof(bufSending));

}
void uart_sendFrame(void)
{
	min_send_frame(&node_1, 1 , buf_test, sizeof(buf_test));
}
void uart_test(void)
{
	min_send_frame(&node_1, 1 , bufForSend, sizeof(bufForSend));
}
void uart_receive_min()
{
		uint8_t data;
		uint8_t len = 0;
		if(uart_available()>0)
		{
		data = uart_read();
		len = 1; 
		}
		min_poll(&node_1,&data,len);
}
