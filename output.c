#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "output.h"
#include "sampler.h"
#include "dipDetector.h"
#include "shutdownManager.h"
#include "periodTimer.h"
#include "potentiometer.h"

// Output Thread
static pthread_t outputThread;
static void* Output_threadFunction(void* args);

// Output Functions
static void Output_printStats(void);
static void Output_printTimingJitter(void);
static void Output_printEvery200Samples(void);

void Output_startOutputing(void)
{
    pthread_create(&outputThread, NULL, &Output_threadFunction, NULL);
}

void Output_stopOutputing(void)
{
    pthread_join(outputThread, NULL);
}

static void* Output_threadFunction(void* args)
{
    while (!Shutdown_isShuttingDown()) {

        Output_printStats();
        Output_printTimingJitter();
        printf("\n");
        Output_printEvery200Samples();
        printf("\n");
        sleep(1);
    }
    Shutdown_trigger();
    return NULL;
}

static void Output_printStats(void)
{
    printf("Samples/s = %d  ", Sampler_getNumSamplesAndReset());
    printf("Pot Value = %d  ", Pot_getValue());
    printf("history size = %d  ", Sampler_getNumSamplesInHistory());
    printf("avg = %0.3f  ", Sampler_getAverageReading());
    printf("dips = %d  ", Dip_getDipCount());
}

static void Output_printTimingJitter(void)
{
    Period_statistics_t pStats;
    Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &pStats);
    char* timingJitterString = "Sampling[%0.3f, %0.3f] avg %0.3f/%d";
    printf(timingJitterString, pStats.minPeriodInMs, pStats.maxPeriodInMs, pStats.avgPeriodInMs, pStats.numSamples);
}

static void Output_printEvery200Samples(void)
{
    int index = Sampler_getNumSamplesInHistory();
    double* history = Sampler_getHistory(&index);

    index--;
    while (index >= 0) {
        printf("%0.3f ", history[index]);
        index -= 200;
    }
    free(history);
}