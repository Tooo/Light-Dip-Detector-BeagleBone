#include "ledDisplay.h"
#include "system.h"

void Display_init(void)
{
    System_runCommand("config-pin P9_18 i2c");
    System_runCommand("config-pin P9_17 i2c");
}

void Display_cleanup(void)
{

}

void Display_setDigit(int digit)
{
    if (digit > 99) {
        digit = 99;
    }

}