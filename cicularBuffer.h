// circularBuffer.h

#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

// Initialize/cleanup the module's data structures.
void Buffer_init(void);
void Buffer_cleanup(void);

void Buffer_insert(double value);

double* Buffer_getValues(int amount);

void Buffer_resize(int size);

int Buffer_getCount(void);

#endif