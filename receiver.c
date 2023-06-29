#include "receiver.h"
#include <stdio.h>
#include <pthread.h>
#include "list.h"
#define BACKLOG 1
#define MAX_MSG_LEN 100

#define MAX_BUF 200
int sizeOfList;
char* MSG;
pthread_t recvTh;
pthread_cond_t * local_recvListCond;
pthread_mutex_t * local_mutexRecvList;
LIST* localRecvList;
void Receiver_init( char *MyPortNum, pthread_cond_t * condRecvList,
                    pthread_mutex_t *mutexRecvList,
                    LIST* recvList)
{
    local_recvListCond=condRecvList;
    local_mutexRecvList= mutexRecvList;
    localRecvList=recvList;
    

    pthread_create  (&recvTh,
                    NULL,
                    receive,
                    (void*) MyPortNum);
}

void* receive(void * portNum)
{   
    
    char *buf;
    int numbytes;
    int sockfd;
   
    struct addrinfo hints, *serverInfo;
    int rv;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags= AI_PASSIVE;



    if((rv=getaddrinfo(NULL, (char*) portNum, &hints,&serverInfo))!=0)
    {
        fprintf(stderr, "getaddinfo: %s\n",gai_strerror(rv));
    }

    if((sockfd= socket( serverInfo->ai_family,
                    serverInfo->ai_socktype,
                    serverInfo->ai_protocol))==-1)
                    {
                        perror("listener: socket");
                    }


   if(( bind(sockfd,serverInfo->ai_addr,serverInfo->ai_addrlen))==-1)
   {
       close(sockfd);
       perror("listener: bind");
   }
   if(serverInfo==NULL)
   {
       fprintf(stderr,"listener: failed to bind socket \n");
       return NULL;
   }
    freeaddrinfo(serverInfo);
    addr_len=sizeof their_addr;
    while(1){
        buf=(char*) malloc(MAX_BUF*sizeof(char));
        if((numbytes= recvfrom( sockfd,buf,MAX_BUF-1,0,
                            (struct sockaddr *)&their_addr,&addr_len))==-1)
                            {
                                perror("recvfrom");
                                exit(1);
                            }
          pthread_mutex_lock(local_mutexRecvList);
          {
             // printf("Received message: %s\n",buf);
            //printf("receiver thread is envoked\n");
            if((*buf=='!') & (numbytes == 1))
            {
                close(sockfd);
                printf("\x1b[33mThe other side decided to end the conversation, finishing...\n\x1b[0m");

                exit(1);
            }
            printf("\x1b[0mOther User: ");
            for(int i=0;i<numbytes;i++)
                ListAdd(localRecvList,(void*) buf+i);
          }
          //free(buf);
          pthread_cond_signal(local_recvListCond);
          pthread_mutex_unlock(local_mutexRecvList);
        // buf[numbytes]='\0';
        // printf("%s\n",buf);
    }
    return NULL;
}
void Receiver_waitForShutdown()
{
    pthread_join(recvTh,NULL);
    
}