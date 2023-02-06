// ledDisplay.h
// Module to manage LED Display
// Thread looping to set digits on display
#ifndef _LED_DISPLAY_H_
#define _LED_DISPLAY_H_

// Begin/end the background thread which display values;
void Display_startDisplaying(void);
void Display_stopDisplaying(void);

// Initialize/cleanup the module's data structures.
void Display_init(void);
void Display_cleanup(void);

// Set display digit variable, used in display thread
void Display_setDigit(int digit);

#endif