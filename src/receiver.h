#ifndef _RECEIVER_H_
#define _RECEIVER_H_
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
void Receiver_init( char *MyPortNum, pthread_cond_t *recvListCond, 
                    pthread_mutex_t * mutexRecvList, LIST* recvList);
void Receiver_waitForShutdown();
void* receive(void * PortNum);
#endif