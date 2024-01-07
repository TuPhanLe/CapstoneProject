#include "sensor.h"


#define CHECKING_SENSOR_TIME	1000

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;


dht11_t dht;
YL69 soil;
void readDataFromSensor(sensor_t *sensor)
{
	readDHT11(&dht);
	sensor->system_huminity =updateEstimate(dht.humidty,0);
	sensor->system_temperature = updateEstimate(dht.temperature,1);
	sensor->system_soil = updateEstimate(get_YL_value(&soil),2)*100;
	sensor->system_light = updateEstimate(BH1750_Read(),3)*100;
}
void sensor_handle(sensor_t *sensor)
{
		for(int i =0; i<= 25; i++)
		{
			readDataFromSensor(sensor);
		}

}

void sensor_init(sensor_t *_sensor,uint32_t *tickAtThis)
{
	SimpleKalmanFilter(200,200,0.01);
	YL69_init(&soil,ADC_CHANNEL_1,hadc1);
	BH1750_Init();
	init_dht11(&dht,&htim1,GPIOA,GPIO_PIN_0);
	uint32_t tick = HAL_GetTick(); 
	for(int i = 0; i <= 40; i++)
	{
		readDataFromSensor(_sensor);
	}
	*tickAtThis = (HAL_GetTick() - tick)*0.0013; 
	// sample 		sec
	// 20				  7
	// 10 				3
	// 40 				14
}


