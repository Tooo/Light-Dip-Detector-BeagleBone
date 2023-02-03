// main.c
// Main Thread of Light Sampler program

#include "shutdownManager.h"

static void main_init(void);
static void main_cleanup(void);

int main(void)
{
    main_init();

    Shutdown_wait();

    main_cleanup();
    return 0;
}

static void main_init(void)
{
    Shutdown_init();
}

static void main_cleanup(void)
{
    Shutdown_cleanup();
}