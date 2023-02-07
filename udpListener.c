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
    char* previousMessage = "";

    while (!Shutdown_isShuttingDown()){
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN];

        int bytesRx = recvfrom(socketDescriptor, messageRx, MAX_LEN - 1, 0,
                                (struct sockaddr*) &sinRemote, &sin_len);

        messageRx[bytesRx] = '\0';

        if (strcmp(messageRx, "")) {
            strcpy(messageRx, previousMessage);
        }

        if (strcmp(messageRx, "help")) {
            Udp_help();
        } else if (strcmp(messageRx, "count")) {
            Udp_count();
        } else if (strcmp(messageRx, "length")) {
            Udp_length();
        } else if (strcmp(messageRx, "history")) {
            Udp_history();
        } else if (strcmp(messageRx, "dips")) {
            Udp_dips();
        } else if (strcmp(messageRx, "stop")) {
            Udp_stop();
        } else if (strstr(messageRx, "get") != NULL) {
            Udp_get(10);
        } else {
            Udp_unknown();
        }     


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
    snprintf(messageTx, MAX_LEN, "Number of samples taken = %lld\n", count);
    Udp_send(messageTx);
}

static void Udp_length(void)
{
    char lengthStr[MAX_LEN/2];
    int size = Sampler_getHistorySize();
    snprintf(lengthStr, MAX_LEN, "History can hold  %4d samples.\n", size);

    char lengthStr2[MAX_LEN/2];
    int holding = Sampler_getNumSamplesInHistory();
    snprintf(lengthStr, MAX_LEN, "Currently holding %4d samples.\n", holding);

    char messageTx[MAX_LEN];
    snprintf(messageTx, MAX_LEN, "%s%s\n", lengthStr, lengthStr2);
    Udp_send(messageTx);
}

static void Udp_history(void)
{


}

static void Udp_get(int count)
{

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