#ifndef UART_H
#define UART_H
#include <stdint.h>
#include "ringbuff.h"
#include "min.h"
#include "DHT.h"
#include "util.h"
#include "sensor.h"
#include "stm32f1xx_hal.h"
typedef struct
{
	uint8_t data_8bitLSB;
	uint8_t data_8bitMSB;
	uint8_t data_8bit2;
	uint8_t data_8bit3;
}data_separated;


void 			receive_data(uint8_t data);
uint16_t 	uart_available(void);
uint8_t 	uart_read(void);
void 			uart_init(void);
void 			uart_send_min(sensor_t sensor);
void 			uart_receive_min(void);
void 			uart_sendFrame(void);
#endif

