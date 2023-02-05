#include <stdlib.h>

#include "lightSensor.h"
#include "system.h"

static char* voltageFile = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";
static double voltageRef = 1.8;
static int voltageMax = 4095;

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
    return ((double)value / voltageMax) * voltageRef;
}