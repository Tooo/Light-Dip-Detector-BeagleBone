#include <pthread.h>

#include "shutdownManager.h"
#include "samplerThread.h"
#include "potThread.h"
#include "dipDetector.h"
#include "ledDisplay.h"
#include "outputThread.h"
#include "udpListener.h"

static pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t shutdownCond = PTHREAD_COND_INITIALIZER;
static bool isShuttingDown;

void Shutdown_init(void)
{   
    isShuttingDown = false;
    Sampler_startSampling();
    Pot_startInputing();
    Dip_startDetecting();
    Display_startDisplaying();
    Output_startOutputing();
    Udp_startListening();
}

void Shutdown_cleanup(void)
{
    Udp_stopListening();
    Output_stopOutputing();
    Display_stopDisplaying();
    Dip_stopDetecting();
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
    isShuttingDown = true;
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_signal(&shutdownCond);
    }
    pthread_mutex_unlock(&shutdownMutex);
}

bool Shutdown_isShuttingDown(void)
{
    return isShuttingDown;
}