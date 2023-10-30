#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "Structs.h"
#include "Queue.h"

int dimensiuneCoada=1;
int curActiveThreads=0;
tuplacc *coadaReq;
pthread_mutex_t mutex;

#pragma region  Initializari

void* calculate(void*);
void* manageQueue(void*);

void initialiseSocket()
{
     
}

void initialiseManThread()
{
    pthread_t id;
    pthread_create(&id,NULL,manageQueue,NULL);
}

#pragma endregion



#pragma region Main thread functions
void listen()
{

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
    printf("Hello, World!");
    pthread_mutex_lock(&mutex);
    curActiveThreads--;
    dimensiuneCoada--;
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
        listen();
    }

}