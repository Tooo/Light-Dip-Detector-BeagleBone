#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>

#include "udpListener.h"
#include "shutdownManager.h"
#include "samplerThread.h"
#include "dipDetector.h"

#define MAX_LEN 1024
#define MAX_PACKET_SIZE 20
#define PORT 12345

static int socketDescriptor;
static struct sockaddr_in sinRemote;

static pthread_t udpThread;
static void* Udp_threadFunction(void* args);

static void Udp_serverInit(void);

static void Udp_help(void);
static void Udp_history(void);
static void Udp_count(void);
static void Udp_length(void);
static void Udp_history(void);
static void Udp_get(int count);
static void Udp_sendArray(double* array, int length);
static void Udp_dips(void);
static void Udp_stop(void);
static void Udp_unknown(void);

static void Udp_send(char* messageTx);

void Udp_startListening(void)
{
    pthread_create(&udpThread, NULL, &Udp_threadFunction, NULL);
}

void Udp_stopListening(void)
{
    pthread_join(udpThread, NULL);
}

static void* Udp_threadFunction(void* args)
{
    Udp_serverInit();
    char previousMessage[MAX_LEN] = "";

    while (!Shutdown_isShuttingDown()){
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN];

        int bytesRx = recvfrom(socketDescriptor, messageRx, MAX_LEN - 1, 0,
                                (struct sockaddr*) &sinRemote, &sin_len);

        messageRx[bytesRx] = '\0';

        // Just "enter"
        if (bytesRx == 1) {
            strcpy(messageRx, previousMessage);
        }

        // Menu options
        if (strncmp(messageRx, "help", 4) == 0) {
            Udp_help();
        } else if (strncmp(messageRx, "count", 5) == 0) {
            Udp_count();
        } else if (strncmp(messageRx, "length", 5) == 0) {
            Udp_length();
        } else if (strncmp(messageRx, "history", 7) == 0) {
            Udp_history();
        } else if (strncmp(messageRx, "dips", 4) == 0) {
            Udp_dips();
        } else if (strncmp(messageRx, "stop", 4) == 0) {
            Udp_stop();
        } else if (strstr(messageRx, "get") != NULL) {
            Udp_get(10);
        } else {
            Udp_unknown();
        }     

        strcpy(previousMessage, messageRx);

    }
    close (socketDescriptor);
    return NULL;
}

static void Udp_serverInit(void) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
}

static void Udp_help(void)
{
    char messageTx[MAX_LEN];
    char* helpStr = "Accepted command examples:\n";
    char* helpStr2 = "count    -- display total number of samples taken.\n";
    char* helpStr3 = "length   -- display number of samples in history (both max and current).\n";
    char* helpStr4 = "history  -- display the full sample history being saved.\n";
    char* helpStr5 = "get 10   -- display the 10 most recent history values.\n";
    char* helpStr6 = "dips     -- display number of dips.\n";
    char* helpStr7 = "stop     -- cause the server program to end.\n";
    char* helpStr8 = "<enter>  -- repeat last command.\n";
    snprintf(messageTx, MAX_LEN, "%s%s%s%s%s%s%s%s", helpStr, helpStr2, helpStr3, helpStr4, helpStr5, helpStr6, helpStr7, helpStr8);
    Udp_send(messageTx);
}

static void Udp_count(void)
{
    char messageTx[MAX_LEN];
    long long count = Sampler_getNumSamplesTaken();
    snprintf(messageTx, MAX_LEN, "Number of samples taken = %lld.\n", count);
    Udp_send(messageTx);
}

static void Udp_length(void)
{
    char lengthStr[MAX_LEN/2];
    int size = Sampler_getHistorySize();
    snprintf(lengthStr, MAX_LEN/2, "History can hold  %4d samples.\n", size);

    char lengthStr2[MAX_LEN/2];
    int holding = Sampler_getNumSamplesInHistory();
    snprintf(lengthStr2, MAX_LEN/2, "Currently holding %4d samples.\n", holding);

    char messageTx[MAX_LEN];
    snprintf(messageTx, MAX_LEN, "%s%s", lengthStr, lengthStr2);
    Udp_send(messageTx);
}

static void Udp_history(void)
{
    int length = Sampler_getNumSamplesInHistory();
    double* history = Sampler_getHistory(&length);

    Udp_sendArray(history, length);

    free(history);
}

static void Udp_get(int count)
{
    if (count < Sampler_getNumSamplesInHistory()) {
        int length = Sampler_getNumSamplesInHistory();
        char messageTx[MAX_LEN];
        snprintf(messageTx, MAX_LEN, "Get size invalid. There are %d samples.\n", length);
        Udp_send(messageTx);
        return;
    }
    double* history = Sampler_getHistory(&count);
    Udp_sendArray(history, count);
}

static void Udp_sendArray(double* array, int length)
{
    int index = 0;
    while (index < length - 1) {
        char messageTx[MAX_LEN] = "";
        int packetSize;
        if (index > length - MAX_PACKET_SIZE - 1) {
            packetSize = (length - index) % MAX_PACKET_SIZE;
        } else {
            packetSize = MAX_PACKET_SIZE;
        }

        for (int i = 0; i < packetSize; i++) {
            char sample[MAX_LEN/(MAX_PACKET_SIZE+1)];
            snprintf(sample, MAX_LEN/(MAX_PACKET_SIZE+1), "%0.3f, ", array[index]);
            index++;
            strncat(messageTx, sample, MAX_LEN/(MAX_PACKET_SIZE+1));
        }
        strncat(messageTx, "\n", MAX_LEN/(MAX_PACKET_SIZE+1));
        Udp_send(messageTx);
    }
}

static void Udp_dips(void)
{
    char messageTx[MAX_LEN];
    int dips = Dip_getDipCount();
    snprintf(messageTx, MAX_LEN, "Dips = %d\n", dips);
    Udp_send(messageTx);
}

static void Udp_stop(void)
{
    char* messageTx = "Program terminating.\n";
    Udp_send(messageTx);
    Shutdown_trigger();
}

static void Udp_unknown(void)
{
    char* messageTx = "Unknown Command. Please type help for command examples.\n";
    Udp_send(messageTx);
}

static void Udp_send(char* messageTx) 
{
    unsigned int sin_len = sizeof(sinRemote);
    sendto(socketDescriptor, messageTx, strlen(messageTx), 0,
            (struct sockaddr *) &sinRemote, sin_len);
}