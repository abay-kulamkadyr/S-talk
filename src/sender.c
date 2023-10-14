
#include "sender.h"


static char* MSG;
pthread_t senderTh;
char* host;
char* port;
char* last;
LIST* local_sendList;
char* findToFree;
pthread_cond_t* local_sendListCond;
pthread_mutex_t* local_mutexSendList;
int sizeOfList;
void Sender_init(   char *foreignHost,char*foreignPort,
                    pthread_cond_t * sendListCond,
                    pthread_mutex_t * mutexSendList,
                    LIST* sendList)
{
    port=foreignPort;
    host=foreignHost;
    local_sendListCond= sendListCond;
    local_mutexSendList=mutexSendList;
    local_sendList=sendList;
  
    
    
    // printf("address of condvar in keyboard %p \n", local_sendListCond);
    // printf("address of mutex in keyboard %p \n", local_mutexSendList);

    pthread_create  (&senderTh,
                    NULL,
                    toSend,
                    NULL);
}
void Sender_waitForShutdown()
{
    pthread_join(senderTh,NULL);
}
void* toSend()
{
    int rv;
    struct addrinfo hints, *serverinfo;
    int sockfd;
    int numbytes;
    memset(&hints,0, sizeof hints);
    hints.ai_family=AF_INET; 
    hints.ai_socktype=SOCK_DGRAM;

    if((rv=getaddrinfo(host,port, &hints,&serverinfo))!=0)
    {
        fprintf(stderr, "getaddinfo: %s\n", gai_strerror(rv));
        return NULL;
    }
    if((sockfd=socket(serverinfo->ai_family,serverinfo->ai_socktype,serverinfo->ai_protocol))==-1)
    {
        perror("taler: socket");
    }
    if(serverinfo==NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
    }
    while(1){
        
        pthread_mutex_lock(local_mutexSendList);
        {
            pthread_cond_wait(local_sendListCond, local_mutexSendList);
           // printf("sender is envoked\n");
            // get and remove messages from the list
            sizeOfList=ListCount(local_sendList);
            MSG= (char*)malloc(sizeOfList*sizeof(char)+1);
            //printf("Size of the list %d\n", sizeOfList);
            //printf("list current item %c\n", *((char*) ListCurr(local_sendList)));
            findToFree= (char*) ListFirst(local_sendList);
            for(int i=sizeOfList-1;i>=0;i--)
            {
                MSG[i]= *((char*) (ListTrim(local_sendList)));
            }
            //printf("Send list address at sender: %p\n",local_sendList);
            MSG[sizeOfList]='\0';
            //printf("Whole msg sent %s\n", MSG);
            
            if((    numbytes= sendto(sockfd, MSG, strlen(MSG),0, 
                    serverinfo->ai_addr,serverinfo->ai_addrlen))==-1)
            {
                perror("talker: sendto");
                exit(1);
            }
            if((*MSG=='!') & (numbytes == 1))
            {
                close(sockfd);
                exit(1);
            }
            free(MSG);
            free(findToFree);
        }
        pthread_mutex_unlock(local_mutexSendList);

    
    }
    freeaddrinfo(serverinfo);
    return NULL;
}