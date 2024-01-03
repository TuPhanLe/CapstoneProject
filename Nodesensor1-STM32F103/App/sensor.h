#ifndef __SENSOR_H
#define __SENSOR_H

#include "stdint.h"
#include "string.h"
#include "YL-69.h"
#include "bh1750.h"
#include "mk_dht11.h"
#include "DHT.h"
#include "SimpleKalmanFilter.h"
#include "DHT.h"
#include "delay_timer.h"
typedef struct 
{
	 uint16_t system_temperature;
	 uint16_t system_huminity;
	 uint16_t system_soil;
	 uint32_t system_light;	 
}sensor_t;

void sensor_handle(sensor_t *sensor);
void sensor_init(sensor_t *_sensor,float *tickAtThis);


#endif 


