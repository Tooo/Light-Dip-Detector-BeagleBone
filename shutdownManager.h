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

void Shutdown_wait(void);
void Shutdown_trigger(void);

bool Shutdown_isShuttingDown(void);

#endif