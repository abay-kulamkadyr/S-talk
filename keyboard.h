#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
void KeyboardInit(  pthread_cond_t * condSendList, 
                    pthread_mutex_t* mutexSendList,
                    LIST* sendList);
void keyboard_waitForShutdown();
void* keyInput();
#endif