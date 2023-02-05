// ledDisplay.h
#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

void LightSensor_init(void);
void LightSensor_cleanup(void);

double LightSensor_getVoltage(void);

#endif