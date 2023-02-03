// shutdownManager.h
// Manages the shudown process of the app
#ifndef _SHUTDOWNMANAGER_H_
#define _SHUTDOWNMANAGER_H_

// Initialize/cleanup the module's data structures.
void Shutdown_init(void);
void Shutdown_cleanup(void);

void Shutdown_wait(void);
void Shutdown_trigger(void);

#endif