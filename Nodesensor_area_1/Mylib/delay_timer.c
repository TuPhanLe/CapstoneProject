#include "delay_timer.h"

TIM_HandleTypeDef *delay_timer;

void DELAY_US_Init(TIM_HandleTypeDef *p_htim){
	delay_timer = p_htim;
}

void DELAY_US_Run(uint32_t p_delay_time){
	delay_timer->Instance->CNT = 0;
	HAL_TIM_Base_Start(delay_timer);
	while(delay_timer->Instance->CNT < p_delay_time);
	HAL_TIM_Base_Stop(delay_timer);
}
void DELAY_MS_RUN(uint32_t p_delay_time)
{
	for(int i = 0; i <= p_delay_time; i++)
	{
		DELAY_US_Run(1000);
	}
}
