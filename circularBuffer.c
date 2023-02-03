#include <stdlib.h>
#include <pthread.h>

#include "cicularBuffer.h"

static pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER;

static double* buffer;
static double* tempBuffer;

static int bufferSize;
static int bufferCount;
static int bufferIndex;

static void Buffer_clearBuffer(void);

void Buffer_init(void)
{
    pthread_mutex_lock(&bufferMutex);
    {
        bufferSize = 1000;
        bufferCount = 0;
        bufferIndex = 0;
        buffer = malloc((bufferSize)*sizeof(*buffer));
        tempBuffer = NULL;
        Buffer_clearBuffer();
    }
    pthread_mutex_unlock(&bufferMutex);
}

void Buffer_cleanup(void)
{
    pthread_mutex_lock(&bufferMutex);
    {
    Buffer_clearBuffer();
    free(buffer);
    }
    pthread_mutex_unlock(&bufferMutex);
}

static void Buffer_clearBuffer(void)
{
    for (int i = 0; i<bufferSize; i++) {
        buffer[i] = 0;
    }
}