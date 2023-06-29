#include "screen.h"
pthread_t screenTh;
pthread_cond_t *local_condRecvList;
pthread_mutex_t * local_mutexRecvList;
char* MSG;
LIST* localRecvList;
int sizeOfList;
char* findToFree;
void ScreenInit(pthread_cond_t * condRecvList,
                pthread_mutex_t * mutexRecvList,
                LIST* recvList){

    local_condRecvList=condRecvList;
    local_mutexRecvList=mutexRecvList;
    localRecvList= recvList;
    pthread_create (&screenTh,
                    NULL,
                    outputMsg,
                    NULL);

}
void Screen_waitForShutdown()
{
    pthread_join(screenTh,NULL);
}
void* outputMsg()
{
    while(1){
        pthread_mutex_lock(local_mutexRecvList);
        {

            pthread_cond_wait(local_condRecvList,local_mutexRecvList);
            //printf("Screen thread invoked\n");
            sizeOfList=ListCount(localRecvList);
            MSG=(char*) malloc(sizeOfList*sizeof(char));
            findToFree= (char*) (ListFirst(localRecvList));
            for(int i=sizeOfList-1;i>=0;i--)
            {
                MSG[i]= *((char*) (ListTrim(localRecvList)));
            }
            MSG[sizeOfList]='\0';
            printf("\x1b[32m%s\x1b[31m\n", MSG);
            
            free(MSG);
            free(findToFree);
        }
        pthread_mutex_unlock(local_mutexRecvList);
    }
}