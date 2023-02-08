#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include "dipDetector.h"
#include "sampler.h"
#include "shutdownManager.h"
#include "periodTimer.h"
#include "ledDisplay.h"

// voltage difference and hysteresis for dips
#define VOLTAGE_DIP_DIFF 0.1
#define VOLTAGE_HYSTERESIS 0.03

// dip detection thread
static const int dipSleepMs = 100;
static pthread_t dipThread;
static void* Dip_threadFunction(void* args);

// Curent amount of dips
static int dipCount;

// Calculate dips on current history
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
        Timer_sleepForMs(dipSleepMs);
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
            if (sample >= average - VOLTAGE_DIP_DIFF + VOLTAGE_HYSTERESIS) {
                isDip = false;
                dipCount++;
            } 
        } else if (sample <= average - VOLTAGE_DIP_DIFF) {
            isDip = true;
        }
    }
    free(history);
}