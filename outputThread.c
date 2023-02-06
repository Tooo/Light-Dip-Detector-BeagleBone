#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "outputThread.h"
#include "samplerThread.h"
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
    for (int j=0; j < 5; j++) {
        printf("Sample Count: %d", Sampler_getNumSamplesInHistory());
        printf(" Size: %d\n", Sampler_getHistorySize());
        int length = 10;
        double* history = Sampler_getHistory(&length);

        for (int i = 0; i < length; i++) {
            printf("%f ", history[i]);
        }
        printf("\n");
        free(history);

        sleep(1);
    }
    Shutdown_trigger();
    return NULL;
}