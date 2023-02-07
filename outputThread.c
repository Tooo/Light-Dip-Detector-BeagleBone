#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "outputThread.h"
#include "samplerThread.h"
#include "dipDetector.h"
#include "shutdownManager.h"

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
    while (!Shutdown_isShuttingDown()) {
        printf("Samples/s = %d  ", Sampler_getNumSamplesAndReset());
        printf("history size = %d  ", Sampler_getNumSamplesInHistory());
        printf("avg = %0.3f  ", Sampler_getAverageReading());
        printf("dips = %d  \n", Dip_getDipCount());

        int length = Sampler_getNumSamplesInHistory();
        double* history = Sampler_getHistory(&length);

        length--;

        while (length > 0) {
            printf("%0.3f ", history[length]);
            length -= 200;
        }
        printf("\n");
        free(history);

        sleep(1);
    }
    Shutdown_trigger();
    return NULL;
}