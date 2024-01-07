#ifndef GPIO_H
#define GPIO_H
#include "esp_err.h"
#include "hal/gpio_types.h"

#define BUTTON0 GPIO_NUM_0

typedef void (*input_callback_t) (int,uint64_t);            // function pointer // 
typedef void (*input_timeout_callback_t) (int);            // function pointer // 
typedef enum{
    LO_TO_HI = 1,
    HI_TO_LO = 2,
    ANY_EDLE = 3
}   interrupt_type_edle_t;

void input_io_create(gpio_num_t gpio_num, interrupt_type_edle_t type);
uint8_t input_io_get_level(gpio_num_t gpio_num);
void input_set_callback(void * cb);
void input_set_timeout_callback(void * cb);
void input_io_create_without_interrupt(gpio_num_t gpio_num); 

void output_io_create(gpio_num_t gpio_num);
void output_io_set_level(gpio_num_t gpio_num,int level);
void output_io_toggle(gpio_num_t gpio_num);


void power_control_pump(gpio_num_t gpio_num, uint8_t mode);
#endif