#include <stdlib.h>
#include <string.h>


#include "potentiometer.h"
#include "system.h"

static char* voltageFile = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";

void Potentiometer_init(void)
{
    // nothing
}

void Potentiometer_cleanup(void)
{
    // nothing
}

int Potentiometer_getValue(void)
{
    char value[BUFFER_MAX_LENGTH];

    System_readFile(voltageFile, value);
    
    return atoi(value);
}