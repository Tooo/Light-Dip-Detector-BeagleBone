#include <stdlib.h>

#include "lightSensor.h"
#include "system.h"

// Voltage file
static char* voltageFile = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";

// Voltage values
#define VOLTAGE_REF 1.8
#define VOLTAGE_MAX 4095

void LightSensor_init(void)
{
    // nothing
}

void LightSensor_cleanup(void)
{
    // nothing
}

double LightSensor_getVoltage(void)
{
    char buffer[BUFFER_MAX_LENGTH];
    System_readFile(voltageFile, buffer);
    
    int value = atoi(buffer);
    return ((double)value / VOLTAGE_MAX) * VOLTAGE_REF;
}