#include <stdlib.h>
#include <pthread.h>

#include "cicularBuffer.h"

static pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER;

// Buffer and tempBuffer for resize
static double* buffer;
static double* tempBuffer;

// Buffer properties
static int bufferSize;
static int bufferCount;
static int bufferIndex;

// Set all buffer elements to 0.
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

void Buffer_insert(double value)
{
    pthread_mutex_lock(&bufferMutex);
    {
        buffer[bufferIndex] = value;
        bufferIndex++;
        if (bufferIndex >= bufferSize) {
            bufferIndex = 0;
        }
    }
    pthread_mutex_unlock(&bufferMutex);
}

double* Buffer_getValues(int amount)
{
    if (amount >= bufferCount) {
        amount = bufferCount;
    }

    double* values = malloc(amount*sizeof(*buffer));

    pthread_mutex_lock(&bufferMutex);
    {
        int remaining = 0;

        int valuesIndex = 0;
        for (int i = 0; i<remaining; i++) {
            values[valuesIndex] = buffer[i];
        }

    }
    pthread_mutex_unlock(&bufferMutex);
}

void Buffer_resize(int size)
{
    if (size == bufferSize) {
        return;
    }

    pthread_mutex_lock(&bufferMutex);
    {
        
    }
    pthread_mutex_unlock(&bufferMutex);
}

int Buffer_getCount(void)
{
    return bufferCount;
}

static void Buffer_clearBuffer(void)
{
    for (int i = 0; i<bufferSize; i++) {
        buffer[i] = 0;
    }
}
