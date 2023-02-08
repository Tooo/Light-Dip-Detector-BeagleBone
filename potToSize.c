#include <stdlib.h>
#include <pthread.h>

#include "potToSize.h"
#include "potentiometer.h"
#include "sampler.h"
#include "shutdownManager.h"
#include "periodTimer.h"

static pthread_t potThread;
static void* Pot_threadFunction(void* args);

void Pot_startInputing(void)
{
    pthread_create(&potThread, NULL, &Pot_threadFunction, NULL);
}

void Pot_stopInputing(void)
{
    pthread_join(potThread, NULL);
}

static void* Pot_threadFunction(void* args)
{
    while(!Shutdown_isShuttingDown()) {
        int value = Pot_getValue();
        if (value == 0) {
            value = 1;
        }
        Sampler_setHistorySize(value);
        Timer_sleepForMs(1000);
    }
    return NULL;
}