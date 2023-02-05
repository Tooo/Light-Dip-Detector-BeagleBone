#include <pthread.h>

#include "shutdownManager.h"
#include "samplerThread.h"
#include "potThread.h"
#include "displayThread.h"

static pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t shutdownCond = PTHREAD_COND_INITIALIZER;

void Shutdown_init(void)
{
    Sampler_startSampling();
    Pot_startInputing();
    Display_startDisplaying();
}

void Shutdown_cleanup(void)
{
    Display_stopDisplaying();
    Pot_stopInputing();
    Sampler_stopSampling();
}

void Shutdown_wait(void)
{
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_wait(&shutdownCond, &shutdownMutex);
    }
    pthread_mutex_unlock(&shutdownMutex);
}

void Shutdown_trigger(void)
{
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_signal(&shutdownCond);
    }
    pthread_mutex_unlock(&shutdownMutex);
}