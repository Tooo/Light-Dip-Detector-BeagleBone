// dipDetector.h
// Module to detect the dips
// Thread looping to detect dips and set led Display
#ifndef _DIP_DETECTOR_H_
#define _DIP_DETECTOR_H_

// Begin/end the background thread which detects dips
void Dip_startDetecting(void);
void Dip_stopDetecting(void);

#endif