#ifndef _SENDER_H_
#define _SENDER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "list.h"
#include <pthread.h>
#include <string.h>
struct arg_struct
{
    char* arg1;
    char* arg2;
};
void* toSend();
void Sender_init(   char *foreignHost,char*foreignPort, 
                    pthread_cond_t * sendListCond, 
                    pthread_mutex_t * mutexSendList,
                    LIST* sendList);

void Sender_waitForShutdown();
#endif