// potentiometer.h
// Module to manage potentiometer
#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

// Initialize/cleanup the module's data structures.
void Pot_init(void);
void Pot_cleanup(void);

// Get value from potentiometer
// From 0 to 4095
int Pot_getValue(void);

#endif