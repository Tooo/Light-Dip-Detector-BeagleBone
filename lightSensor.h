// lightSensor.h
// Module to manage light sensor
#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

// Initialize/cleanup the module's data structures.
void LightSensor_init(void);
void LightSensor_cleanup(void);

// Get voltage from light sensor
double LightSensor_getVoltage(void);

#endif