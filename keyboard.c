#include "keyboard.h"
#include <pthread.h>
#include <stdio.h>
pthread_t keyboardTh;
static pthread_cond_t *local_condSendList;
static pthread_mutex_t *local_mutexSendList;
static LIST* localList;

char* MSG;
void KeyboardInit   (pthread_cond_t * condSendList, 
                    pthread_mutex_t* mutexSendList,
                    LIST* sendList){
    local_condSendList=condSendList;
    local_mutexSendList= mutexSendList;
    localList= sendList;
    pthread_create  (&keyboardTh,
                    NULL,
                    keyInput,
                    NULL);
    // printf("address of condvar in keyboard %p \n", local_condSendList);
    // printf("address of mutex in keyboard %p \n", local_mutexSendList);

}
void keyboard_waitForShutdown()
{
    pthread_join(keyboardTh,NULL);
}
char* getln()
{
    char *line=NULL, *tmp=NULL;
    size_t size=0, index=0;
    int ch= EOF;
    while(ch)
    {
        ch=getc(stdin);
        if(ch==EOF || ch=='\n')
            ch=0;
        if(size<=index)
        {
            size+=10;
            tmp=realloc(line,size);
            if(!tmp)
            {
                free(line);
                line=NULL;
                break;
            }
            line=tmp;
        }
        line[index++]=ch;
    }
    return line;
}
void* keyInput(){
    while(1){
        
            //wait until user puts a message to the list
        
        {
            
            // get input from user
            //printf("Put something: \n");
            char* MSG=getln();
            // add each a pointer to the character to sendList
            pthread_mutex_lock(local_mutexSendList);
           // printf("Send list address at keyabord: %p\n",localList);
            //printf("Screen thread invoked\n");
            //printf("sent meesage");
            for(int i=0;i<strlen(MSG);i++)
            {
                ListAdd(localList, (void*) MSG+i);
              // printf(" %c", *((char*)ListCurr(localList)));
            }
           // printf("\n");
            pthread_cond_signal(local_condSendList);
            pthread_mutex_unlock(local_mutexSendList);
            
            //
            
        }
        
    }
}