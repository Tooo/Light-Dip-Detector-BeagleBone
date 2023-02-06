OUTFILE = light_sampler
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread
CFILES = main.c samplerThread.c potThread.c outputThread.c system.c shutdownManager.c circularBuffer.c periodTimer.c lightSensor.c potentiometer.c ledDisplay.c

all: light_sampler

light_sampler:
	$(CC_C) $(CFLAGS) $(CFILES) -o $(OUTDIR)/$(OUTFILE)

clean:
	rm $(OUTDIR)/$(OUTFILE)
