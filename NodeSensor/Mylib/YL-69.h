#ifndef YL_69_H
#define YL_69_H
#include "main.h"
typedef struct
{
	uint32_t channel;
	ADC_HandleTypeDef hadc;
}YL69;
	
float get_YL_value(YL69 *yl);	// Function Get Back YL69 Value // 																
void YL69_init(YL69 *yl,uint32_t channel,ADC_HandleTypeDef hadc);	// Function YL69 INIT // 				
#endif

