OUTFILE_1 = light_sampler
OUTFILE_2 = noworky
OUTDIR = $(HOME)/cmpt433/public/myApps

CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -pthread
CFILES_1 = main.c sampler.c dipDetector.c potToSize.c output.c udpListener.c system.c shutdownManager.c circularBuffer.c periodTimer.c lightSensor.c potentiometer.c ledDisplay.c

all: light_sampler noworky

light_sampler:
	$(CC_C) $(CFLAGS) $(CFILES_1) -o $(OUTDIR)/$(OUTFILE_1)

noworky:
	$(CC_C) $(CFLAGS) noworky.c -o $(OUTDIR)/$(OUTFILE_2)

clean:
	rm $(OUTDIR)/$(OUTFILE_1) $(OUTDIR)/$(OUTFILE_2)

