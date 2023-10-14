#ifndef _SCREEN_H_
#define _SCREEN_H_
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
void ScreenInit(    pthread_cond_t * condRecvList,
                    pthread_mutex_t * mutexRecvList,
                    LIST* recvList);
void Screen_waitForShutdown();
void* outputMsg();
#endif