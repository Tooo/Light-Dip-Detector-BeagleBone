#include <stdlib.h>
#include <string.h>

#include "potentiometer.h"
#include "system.h"

static char* voltageFile = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";

void Pot_init(void)
{
    // nothing
}

void Pot_cleanup(void)
{
    // nothing
}

int Pot_getValue(void)
{
    char buffer[BUFFER_MAX_LENGTH];
    System_readFile(voltageFile, buffer);
    
    return atoi(buffer);
}