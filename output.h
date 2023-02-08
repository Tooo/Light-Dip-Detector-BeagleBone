// output.h
// Module to manage outputs and output thread
// Thread looping each section to output information
#ifndef _OUTPUT_THREAD_H_
#define _OUTPUT_THREAD_H_

// Begin/end the background thread which output to screen;
void Output_startOutputing(void);
void Output_stopOutputing(void);

#endif