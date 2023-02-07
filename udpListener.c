#include <stdlib.h>
#include "string.h"
#include <pthread.h>
#include <netinet/in.h>

#include "udpListener.h"
#include "shutdownManager.h"
#include "dipDetector.h"

#define MAX_LEN 1024
#define PORT 12345

static int socketDescriptor;
static struct sockaddr_in sinRemote;

static pthread_t udpThread;
static void* Udp_threadFunction(void* args);

static void Udp_serverInit(void);



static void Udp_dips(void);
static void Udp_stop(void);

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
        

        char messageTx[MAX_LEN];
        


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

    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
}

static void Udp_help(char* messageTx)
{

}

static void Udp_count(char* messageTx)
{

}

static void Udp_length(char* messageTx)
{

}

static void Udp_history(char* messageTx)
{

}

static void Udp_get(char* messageTx, int count)
{

}

static void Udp_dips(void)
{
    char* messageTx[MAX_LEN];
    int dips = Dip_getDipCount();
    snprintf(messageTx, MAX_LEN, "")
}

static void Udp_stop(void)
{
    char* messageTx = "Program terminating.";
    Udp_send(messageTx);
    Shutdown_trigger();
}

static void Udp_send(char* messageTx) 
{
    unsigned int sin_len = sizeof(sinRemote);
    sendto(socketDescriptor, messageTx, strlen(messageTx), 0,
            (struct sockaddr *) &sinRemote, sin_len);
}