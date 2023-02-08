#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include "dipDetector.h"
#include "sampler.h"
#include "shutdownManager.h"
#include "periodTimer.h"
#include "ledDisplay.h"

static const double voltageDifference = 0.1;
static const double voltageHysteresis = 0.03;

static pthread_t dipThread;
static void* Dip_threadFunction(void* args);

static int dipCount;

static void Dip_calculateDip();

void Dip_startDetecting(void)
{
    pthread_create(&dipThread, NULL, &Dip_threadFunction, NULL);
}

void Dip_stopDetecting(void)
{
    pthread_join(dipThread, NULL);
}

int Dip_getDipCount(void)
{
    return dipCount;
}

static void* Dip_threadFunction(void* args)
{
    while(!Shutdown_isShuttingDown()) {
        Dip_calculateDip();
        Display_setDigit(dipCount);
        Timer_sleepForMs(100);
    }
    return NULL;
}

static void Dip_calculateDip()
{
    int length = Sampler_getNumSamplesInHistory();
    double average = Sampler_getAverageReading();
    double* history = Sampler_getHistory(&length);

    bool isDip = false;
    dipCount = 0;

    for (int i = 0; i<length; i++) {
        double sample = history[i];

        if (isDip) {
            if (sample >= average - voltageDifference + voltageHysteresis) {
                isDip = false;
                dipCount++;
            } 
        } else if (sample <= average - voltageDifference) {
            isDip = true;
        }
    }
    free(history);
}