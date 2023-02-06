// shutdownManager.h
// Manages the shudown process of the app
// Refer to Brian's idea in the lecture
// and previous creation of Shutdown Manager
#ifndef _SHUTDOWNMANAGER_H_
#define _SHUTDOWNMANAGER_H_

#include <stdbool.h>

// Initialize/cleanup the module's data structures.
void Shutdown_init(void);
void Shutdown_cleanup(void);

// Have main thread wait until trigger is called
void Shutdown_wait(void);

// Main thread continues once shutdown triggers
void Shutdown_trigger(void);

// Used for all while loops to see if app is shutting down
bool Shutdown_isShuttingDown(void);

#endif