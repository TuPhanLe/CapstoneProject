#include <stdio.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "GPIO.h"

TimerHandle_t xTimers;

input_callback_t            input_callback = NULL;
input_timeout_callback_t    input_timeout_callback = NULL;          

static uint64_t _start, _stop, _pressTick; 
static void IRAM_ATTR gpio_input_handler(void* arg)                 // Hàm ngắt //
{
    int gpio_num = (uint32_t) arg;
    uint32_t rtc = xTaskGetTickCountFromISR();          //ms// 
    if(gpio_get_level(gpio_num) == 0)
    {
        _start = rtc;
        xTimerStart(xTimers,0);
    }
    else 
    {
        _stop = rtc;
        _pressTick = _stop - _start;
        xTimerStop(xTimers,0);
    }

    input_callback(gpio_num,_pressTick);
}
void vTimerCallback( TimerHandle_t xTimer )
 {

    uint32_t ID;
    configASSERT( xTimer );
    ID = ( uint32_t ) pvTimerGetTimerID( xTimer );
    if( ID == 0 )
    {
        input_timeout_callback(BUTTON0);
    }

 }

void input_io_create(gpio_num_t gpio_num, interrupt_type_edle_t type)   // Tạo GPIO dạng input với định dạng ngắt cạnh lên hoặc xuống // 
{
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
    gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(gpio_num, type);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(gpio_num, gpio_input_handler, (void*) gpio_num);  
     xTimers = xTimerCreate
                   ( /* Just a text name, not used by the RTOS
                     kernel. */
                     "TimerForTimeOut",
                     /* The timer period in ticks, must be
                     greater than 0. */
                     5000/portTICK_RATE_MS,
                     /* The timers will auto-reload themselves
                     when they expire. */
                     pdFALSE,
                     /* The ID is used to store a count of the
                     number of times the timer has expired, which
                     is initialised to 0. */
                     ( void * ) 0,
                     /* Each timer calls the same callback when
                     it expires. */
                     vTimerCallback
                   );
}
void input_io_create_without_interrupt(gpio_num_t gpio_num)   // Tạo GPIO dạng input với định dạng ngắt cạnh lên hoặc xuống // 
{
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
    gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);
    gpio_intr_disable(gpio_num);
}

uint8_t input_io_get_level(gpio_num_t gpio_num)                // Đọc giá trị ngõ vào // 
{  
    return gpio_get_level(gpio_num);
}

void input_set_callback(void * cb)                          // Hàm gọi ra ngoài main khi xảy ra ngắt // 
{
    input_callback = cb;
}
void input_set_timeout_callback(void * cb)                          // Hàm gọi ra ngoài main khi xảy ra ngắt // 
{
    input_timeout_callback = cb;
}


void output_io_create(gpio_num_t gpio_num)                  // Tạo GPIO // 
{
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num , GPIO_MODE_INPUT_OUTPUT);

}

void output_io_set_level(gpio_num_t gpio_num,int level)     // Set giá trị cho GPIO // 
{  
    gpio_set_level(gpio_num,level);
}

void output_io_toggle(gpio_num_t gpio_num)                  // Đảo giá trị cho GPIO // 
{
    int old_level = gpio_get_level(gpio_num);
    gpio_set_level(gpio_num, 1 - old_level);

}





void power_control_pump(gpio_num_t gpio_num, uint8_t mode)
{
    output_io_set_level(gpio_num,mode);
}