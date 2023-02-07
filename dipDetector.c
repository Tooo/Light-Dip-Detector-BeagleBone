#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include "dipDetector.h"
#include "samplerThread.h"
#include "shutdownManager.h"
#include "periodTimer.h"
#include "ledDisplay.h"

static const double voltageDifference = 0.1;
static const double voltageHysteresis = 0.03;

static pthread_t dipThread;
static void* Dip_threadFunction(void* args);

static int Dip_calculateDip();

void Dip_startDetecting(void)
{
    pthread_create(&dipThread, NULL, &Dip_threadFunction, NULL);
}

void Dip_stopDetecting(void)
{
    pthread_join(dipThread, NULL);
}

static void* Dip_threadFunction(void* args)
{
    while(!Shutdown_isShuttingDown()) {
        int dipCount = Dip_calculateDip();
        Display_setDigit(dipCount);
        Timer_sleepForMs(100);
    }
    return NULL;
}

static int Dip_calculateDip()
{
    int length = Sampler_getNumSamplesInHistory();
    double* history = Sampler_getHistory(&length);
    double average = Sampler_getAverageReading();
    int dipCount = 0;

    bool isDipBelow = false;
    bool isDipAbove = false;

    for (int i = 0; i<length; i++) {
        double sample = history[i];

        double difference = abs(sample - average);
        // printf("DIP - sample:%f, avg: %f, diff: %f\n", sample, average, difference);

        if (isDipAbove) {
            if (sample - average < voltageDifference + voltageHysteresis) {
                isDipAbove = false;
            }
        } else if (isDipBelow) {
            if (average - sample < voltageDifference + voltageHysteresis) {
                isDipBelow = false;
            }
        } else if (difference > voltageDifference) {
            dipCount++;
            if (sample > average) {
                isDipAbove = true;
            } else {
                isDipBelow = true;
            }
        }
    }
    
    free(history);
    return dipCount;
}