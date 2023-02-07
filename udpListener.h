// udpListener.h
// Module to manage udp server thread
// Thread looping to receive commands
#ifndef _UDP_LISTENER_H_
#define _UDP_LISTENER_H_

// Begin/end the background thread which listens to the UDP server
void Udp_startListening(void);
void Udp_stopListening(void);

#endif