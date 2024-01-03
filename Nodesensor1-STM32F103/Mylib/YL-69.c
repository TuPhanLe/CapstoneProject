#include "YL-69.h"
uint32_t map(long x, long in_min, long in_max, long out_min, long out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float get_YL_value(YL69 *yl)
{
	float adc;
		// Config channel // 
		ADC_ChannelConfTypeDef sConfig = {0};
		sConfig.Channel = yl->channel;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

		// Config adc value // 	
		HAL_ADC_Start(&yl->hadc);
		HAL_ADC_PollForConversion(&yl->hadc, 1000);
		adc = HAL_ADC_GetValue(&yl->hadc);
		HAL_ADC_Stop(&yl->hadc);
//		adc = map(adc, 900.00, 3200.00, 0.00, 100.00);
		adc = 100 - (adc-1100)/20;
		return adc;
}	
void YL69_init(YL69 *yl,uint32_t channel,ADC_HandleTypeDef hadc)
{
	yl->channel = channel;
	yl->hadc = hadc;
//	HAL_ADCEx_Calibration_Start(&yl->hadc);
}
