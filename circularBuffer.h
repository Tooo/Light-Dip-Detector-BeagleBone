// circularBuffer.h
// Manages a circular buffer
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

// Initialize/cleanup the module's data structures.
void Buffer_init(void);
void Buffer_cleanup(void);

// Insert double into buffer
void Buffer_insert(double value);

// Get given amount of values of buffer
double* Buffer_getValues(int* amount);

// Resive Buffer to new size
// If new size is smaller, take most recent values
// If new size is larger, take all values
void Buffer_resize(int size);

// Get number of values in buffer
int Buffer_getCount(void);

// Get maxmium size of buffer
int Buffer_getSize(void);

#endif