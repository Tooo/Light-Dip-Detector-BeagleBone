#include <stdlib.h>
#include <pthread.h>

#include "circularBuffer.h"

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

        buffer = malloc((bufferSize+1)*sizeof(*buffer));
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
        bufferIndex = bufferIndex % bufferSize;
        if (bufferCount < bufferSize) {
            bufferCount++;
        }
    }
    pthread_mutex_unlock(&bufferMutex);
}

double* Buffer_getValues(int* amount)
{
    if (*amount == 0) {
        return NULL;
    }

    double* values;

    pthread_mutex_lock(&bufferMutex);
    {
        if (*amount > bufferCount) {
            *amount = bufferCount;
        }

        values = malloc((*amount+1)*sizeof(*buffer));

        int index = bufferIndex-1;
    
        for (int i = 0; i<*amount; i++) {
            if (index < 0) {
                index = bufferSize-1;
            }
            values[i] = buffer[index];
            index--;
        }
    }
    pthread_mutex_unlock(&bufferMutex);

    return values;
}

void Buffer_resize(int size)
{
    if (size == bufferSize) {
        return;
    }

    pthread_mutex_lock(&bufferMutex);
    {
        tempBuffer = malloc((size+1)*sizeof(*buffer));

        for (int i = 0; i<size; i++) {
            tempBuffer[i] = 0;
        }

        int amount = size;

        if (size > bufferCount) {
            amount = bufferCount;
        }

        int index = bufferIndex;

        for (int i = 0; i<amount; i++) {
            tempBuffer[i] = buffer[index];
            index++;
            index = index % bufferSize;
        }

        Buffer_clearBuffer();
        free(buffer);

        buffer = tempBuffer;
        tempBuffer = NULL;

        bufferSize = size;
        bufferIndex = amount;
        bufferCount = amount;
    }
    pthread_mutex_unlock(&bufferMutex);
}

int Buffer_getCount(void)
{
    return bufferCount;
}

int Buffer_getSize(void)
{
    return bufferSize;
}

static void Buffer_clearBuffer(void)
{
    for (int i = 0; i<bufferSize; i++) {
        buffer[i] = 0;
    }
}
