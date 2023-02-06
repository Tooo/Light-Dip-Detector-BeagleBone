// potThread.h
// Module to manage potentiometer thread
// Thread looping once a second to resize buffer with pot value
#ifndef _POT_THREAD_H_
#define _POT_THREAD_H_

// Begin/end the background thread which gets Potentiometer value;
void Pot_startInputing(void);
void Pot_stopInputing(void);

#endif