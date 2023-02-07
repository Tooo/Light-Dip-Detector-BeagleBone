#include <pthread.h>
#include <unistd.h>

#include "samplerThread.h"
#include "circularBuffer.h"
#include "lightSensor.h"
#include "periodTimer.h"
#include "shutdownManager.h"
#include "periodTimer.h"

static long long sampleTotalTaken = 0;
static double voltageAverage = 0;
static double averageWeight = 0.001;
static double sampleTakenInPeriod = 0;

static pthread_t sampleThread;
static void* Sampler_threadFunction(void* args);

static void Sampler_calculateAverage(double voltage);

void Sampler_startSampling(void)
{
    pthread_create(&sampleThread, NULL, &Sampler_threadFunction, NULL);
}

void Sampler_stopSampling(void)
{
    pthread_join(sampleThread, NULL);
}

void Sampler_setHistorySize(int newSize)
{
    Buffer_resize(newSize);
}

int Sampler_getHistorySize(void)
{
    return Buffer_getSize();
}

double *Sampler_getHistory(int *length)
{
    return Buffer_getValues(length);
}

int Sampler_getNumSamplesInHistory()
{
    return Buffer_getCount();
}

double Sampler_getAverageReading(void)
{
    return voltageAverage;
}

long long Sampler_getNumSamplesTaken(void)
{
    return sampleTotalTaken;
}

int Sampler_getNumSamplesAndReset()
{
    int sampleTaken = sampleTakenInPeriod;
    sampleTakenInPeriod = 0;
    return sampleTaken;
}

static void* Sampler_threadFunction(void* args)
{
    while (!Shutdown_isShuttingDown())
    {
        double voltage = LightSensor_getVoltage();
        Buffer_insert(voltage);
        sampleTotalTaken++;
        sampleTakenInPeriod++;
        Sampler_calculateAverage(voltage);
        Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);
        Timer_sleepForMs(1);
    }
    return NULL;
}

static void Sampler_calculateAverage(double voltage)
{
    if (sampleTotalTaken == 1) {
        voltageAverage = voltage;
    } else {
        voltageAverage = averageWeight * voltage + (1.0-averageWeight) * voltageAverage;
    }
}