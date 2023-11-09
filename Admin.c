#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include "Structs.h"
#include "Queue.h"

#pragma region Global Variables
// variabile coada requesturi
char **coadaReq;
int dimReq=0;

// variabile threaduri
int curActiveThreads=0;
pthread_mutex_t mutex;

// variabile conexiuni
int sockfd, portno, clilen, n;
struct sockaddr_in serv_addr, cli_addr;

// variabile coada conexiuni
int* conexiuni;
int dimConex=0;

// variabile coada Send
tuplacc* coadaSends;
int dimSend=0;

#pragma endregion

#pragma region  Initializari

void* calculate(void*);
void* manageQueue(void*);
void* listening(void*);

void initialiseSocket()
{
    //deschid socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket");

    // initializez serv_addr cu 0-uri
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 2002;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        printf("ERROR on binding");

    listen(sockfd,10);
}

void initialiseManThread()
{
    pthread_t id;
    pthread_create(&id,NULL,manageQueue,NULL);
}

void initialiseListenThread()
{
    pthread_t id;
    pthread_create(&id,NULL,listening,NULL);
}
#pragma endregion



#pragma region Main thread functions
void* listening(void* p)
{
    while(1)
    {
        clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        if(newsockfd<0)
            return;
        

        char buffer[256];
        char nsfd[100]; 
        sprintf(nsfd,"%d",dimConex);
        strcpy(buffer,nsfd);
        strcat(buffer,"/");
        pushi(&conexiuni,newsockfd,&dimConex);


        int offset = strlen(buffer);
        bzero(buffer+offset,256-offset);

        n = read(newsockfd,buffer+offset,255-offset);
        if (n < 0) 
            printf("ERROR reading from socket");
            
        // buffer - un fisier care are [protocol]/[payload]

        pushc(&coadaReq,buffer,&dimReq);
    }    
    
    

}

void sending()
{
    if(dimSend==0)
        return;

    tuplacc pachet = popt(&coadaSends,&dimSend,0);
    int sndsockfd = popi(&conexiuni,atoi(pachet.a),&dimConex);

    pthread_mutex_lock(&mutex);
    n = write(sndsockfd,pachet.b,strlen(pachet.b));
    //n = write(sndsockfd,"Here is your message",20);
    pthread_mutex_unlock(&mutex);
    if (n < 0)
        printf("ERROR writing to socket");

    shutdown(sndsockfd,0);
    close(sndsockfd);
}


#pragma endregion



#pragma region Thread Functions

void* manageQueue(void* p)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        int dim = dimReq;
        int cAT= curActiveThreads;
        pthread_mutex_unlock(&mutex);

        if(dim>cAT)
        {
            pthread_t id;
            int i = dim-1;
            pthread_create(&id, NULL, calculate, &i);
            pthread_mutex_lock(&mutex);
            curActiveThreads++;            
            pthread_mutex_unlock(&mutex);
        }
    }
}

void* calculate(void* p)
{
    int par= *(int*)p;
    char* var = popc(&coadaReq,&dimReq,par);
    tuplacc buff;
    int i =0;

    for(i=0;i<strlen(var);i++)
        if(var[i]=='/')
            break;
    
    var[i]='\0';
    buff.a=strdup(var);
    buff.b=strdup(var+i+1);
    pusht(&coadaSends,buff,&dimSend);

    pthread_mutex_lock(&mutex);
    curActiveThreads--;
    pthread_mutex_unlock(&mutex);
}

#pragma endregion



void main()
{
    pthread_mutex_init(&mutex,NULL);
    initialiseSocket();
    initialiseManThread();
    initialiseListenThread();

    while(1)
    {
        //listening();
        sending();
    }

}