// main.c
// Main Thread of Light Sampler program

#include <stdio.h>

#include "shutdownManager.h"
#include "circularBuffer.h"
#include "lightSensor.h"
#include "potentiometer.h"
#include "ledDisplay.h"

static void main_init(void);
static void main_cleanup(void);

int main(void)
{
    main_init();

    Shutdown_wait();
    main_cleanup();
    return 0;
}

static void main_init(void)
{
    Buffer_init();
    LightSensor_init();
    Pot_init();
    Display_init();
    Shutdown_init();
}

static void main_cleanup(void)
{
    Shutdown_cleanup();
    printf("cleanup\n");
    Display_cleanup();
    Pot_cleanup();
    LightSensor_cleanup();
    Buffer_cleanup();
}