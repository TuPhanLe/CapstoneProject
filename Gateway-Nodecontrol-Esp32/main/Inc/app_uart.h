#ifndef __APP_UART_H
#define __APP_UART_H
#include "stdint.h"



#define EX_UART_NUM UART_NUM_2
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
#define M0 GPIO_NUM_21                  // Define for lora connection // 
#define M1 GPIO_NUM_23                 // Define for lora connection // 
#define AUX GPIO_NUM_15               // Define for lora connection // 
struct min_context GATEWAY;
struct min_context SENSOR_NODE_1;
struct min_context SENSOR_NODE_2;
struct min_context CONTROL_NODE;
typedef void (*uart_handle_t) (uint16_t *data, uint16_t length);
void uart_init(void);
void uart_set_callback(void *cb);
void uart_put(uint8_t *data, uint16_t length);

#endif 