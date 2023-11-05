#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include "Structs.h"
#include "Queue.h"

#pragma region Global Variables

int dimensiuneCoada=0;
int curActiveThreads=0;
tuplacc *coadaReq;
pthread_mutex_t mutex;
int sockfd, newsockfd, portno, clilen, n;
struct sockaddr_in serv_addr, cli_addr;
char buffer[256];

#pragma endregion

#pragma region  Initializari

void* calculate(void*);
void* manageQueue(void*);

void initialiseSocket()
{
    //deschid socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("ERROR opening socket");

    // initializez serv_addr cu 0-uri
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 1234;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        printf("ERROR on binding");

    listen(sockfd,5);
}

void initialiseManThread()
{
    pthread_t id;
    pthread_create(&id,NULL,manageQueue,NULL);
}

#pragma endregion



#pragma region Main thread functions
void listening()
{
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
    if (newsockfd < 0)
        printf("ERROR on accept");

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) printf("ERROR reading from socket");
        printf("Here is the message: %s",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0)
        printf("ERROR writing to socket");

}
#pragma endregion



#pragma region Thread Functions

void* manageQueue(void* p)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        int dim = dimensiuneCoada;
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

    while(1)
    {
        listening();
    }

}