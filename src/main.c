#include <stdio.h>
#include <pthread.h>
#include "list.h"
#include "receiver.h"
#include "sender.h"
#include "keyboard.h"
#include "screen.h"
static pthread_mutex_t mutexRecvList=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutexSendList=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sendListCond=PTHREAD_COND_INITIALIZER;
static pthread_cond_t recvListCond=PTHREAD_COND_INITIALIZER;
LIST* sendList;
LIST* recvList;

int main(int args, char** argv)
{
    if(args<4)
    {
        printf("Too few arguments\n");

        printf("\x1b[31mUSAGE: s-talk [YOUR DESIRED PORT NUM] [IP-ADDRESS OF THE RECEIVER] [PORT NUM OF THE RECEIVER] \n\x1b");

        exit(1);
    }
    
    printf("\x1b[33mWelcome to s-talk. The colour of your messages is set to \x1b[31mred \x1b[33mand the colour of the other user messages is set to \x1b[32mgreen\x1b[33m. In order to finish this chat please send \"!\".\n\x1b[31m");
    sendList= ListCreate();
    recvList= ListCreate();
    KeyboardInit(&sendListCond,&mutexSendList,sendList);
    Sender_init(argv[2], argv[3], &sendListCond, &mutexSendList,sendList);
   
    Receiver_init(argv[1],& recvListCond, & mutexRecvList, recvList);
    ScreenInit  (&recvListCond, & mutexRecvList, recvList);
    

    
    Receiver_waitForShutdown();
    Sender_waitForShutdown();
    keyboard_waitForShutdown();
    Screen_waitForShutdown();

    /*Cleanup*/
    pthread_mutex_destroy(&mutexRecvList);
    pthread_mutex_destroy(&mutexSendList);
    pthread_cond_destroy(&sendListCond);
    pthread_cond_destroy(&recvListCond);

    return 0;
}