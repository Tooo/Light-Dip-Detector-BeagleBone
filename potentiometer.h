// potentiometer.h
// Handles input form potentiometer
#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

void Pot_init(void);
void Pot_cleanup(void);

double Pot_getVoltage(void);

#endif