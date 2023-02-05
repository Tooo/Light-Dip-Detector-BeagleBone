#include <stdlib.h>
#include <string.h>

#include "potentiometer.h"
#include "system.h"

static char* voltageFile = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
static double voltageRef = 1.8;
static int voltageMax = 4095;

void Pot_init(void)
{
    // nothing
}

void Pot_cleanup(void)
{
    // nothing
}

double Pot_getVoltage(void)
{
    char buffer[BUFFER_MAX_LENGTH];
    System_readFile(voltageFile, buffer);
    
    int value = atoi(buffer);
    return ((double)value / voltageMax) * voltageRef;
}