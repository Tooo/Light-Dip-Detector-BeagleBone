OUTFILE = light_sampler
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread
CFILES = main.c sampler.c displayThread.c potThread.c system.c shutdownManager.c circularBuffer.c periodTimer.c lightSensor.c potentiometer.c ledDisplay.c

all: app

app:
	$(CC_C) $(CFLAGS) $(CFILES) -o $(OUTDIR)/$(OUTFILE)

clean:
	rm $(OUTDIR)/$(OUTFILE)
