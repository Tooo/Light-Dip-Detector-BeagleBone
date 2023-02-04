#include "lightSensor.h"
#include "system.h"

static char* voltageFile = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";

void LightSensor_init(void)
{
    // nothing
}

void LightSensor_cleanup(void)
{
    // nothing
}

void LightSensor_getValue(void)
{
    char value[BUFFER_MAX_LENGTH];

    System_readFile(voltageFile, value);
    
    return atoi(value);
}