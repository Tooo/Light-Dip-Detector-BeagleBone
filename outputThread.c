#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "outputThread.h"
#include "samplerThread.h"

static pthread_t outputThread;
static void* Output_threadFunction(void* args);

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
    while(1) {
        printf("Sample Count: %d", Sampler_getNumSamplesInHistory());
        printf(" Size: %d\n", Sampler_getHistorySize());
        sleep(1);
    }
    return NULL;
}