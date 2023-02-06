#include <pthread.h>

#include "displayThread.h"
#include "ledDisplay.h"
#include "shutdownManager.h"

static pthread_t displayThread;
static void* Display_threadFunction(void* args);

void Display_startDisplaying(void)
{
    pthread_create(&displayThread, NULL, &Display_threadFunction, NULL);
}

void Display_stopDisplaying(void)
{
    pthread_join(displayThread, NULL);
}

static void* Display_threadFunction(void* args)
{
    while(!Shutdown_isShuttingDown()) {

    }
    return NULL;
}