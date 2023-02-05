#include <pthread.h>
#include <unistd.h>

#include "sampler.h"
#include "cicularBuffer.h"
#include "lightSensor.h"

static double sampleTotalAmount = 0;
static long long sampleTotalTaken = 0;

static pthread_t sampleThread;
static void* Sampler_threadFunction(void* args);

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
    return 1.1;
}

long long Sampler_getNumSamplesTaken(void)
{
    return sampleTotalTaken;
}

static void* Sampler_threadFunction(void* args)
{
    while (1)
    {
        double voltage = LightSensor_getVoltage();
        Buffer_insert(voltage);
        sampleTotalAmount += voltage;
        sampleTotalTaken++;
        sleep(1);
    }
    return NULL;
}