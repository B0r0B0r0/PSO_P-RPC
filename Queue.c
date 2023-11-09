#include "Queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pusht(tuplacc** queue,tuplacc value,int* size)
{
    *queue = (tuplacc*)realloc(*queue,((*size)+1)*sizeof(tuplacc));
    (*queue)[*size].a = strdup(value.a);
    (*queue)[*size].b = strdup(value.b);
    (*size)++;
}

tuplacc popt(tuplacc** queue,int *size,int index)
{
    if(*size==0)
        return;

    if (index>*size)
        return;

    tuplacc lvalue = *queue[index];

    for(int i = index; i<(*size)-1;i++)
        {
            (*queue)[i].a=strdup((*queue)[i+1].a);
            (*queue)[i].b=strdup((*queue)[i+1].b);
        }

    (*queue)[(*size)-1].a = strdup("0");
    (*queue)[(*size)-1].b = strdup("0");
    *queue = (tuplacc*)realloc(*queue,((*size)-1)*sizeof(tuplacc));
    (*size)--;
    return lvalue;
}


void pushc(char*** queue, char* value, int* size)
{
    *queue = (char**)realloc(*queue,((*size)+1)*sizeof(char**));
    (*queue)[*size] = strdup(value);
    (*size)++;
}

char* popc (char*** queue, int* size, int index)
{
    if(*size==0)
        return;

    if (index>*size)
        return;

    char* lvalue = strdup((*queue)[index]);
    for(int i=index;i<(*size)-1;i++)
        (*queue)[i]=strdup((*queue)[i+1]);
    
    *queue = (char**)realloc(*queue,((*size)-1)*sizeof(char*));
    (*size)--;
    return lvalue;
}

void pushi(int **queue, int value, int* size)
{
    *queue = (int*)realloc(*queue, ((*size)+1)*sizeof(int));
    (*queue)[*size] = value;
    (*size)++;
}

int popi(int **queue, int index, int* size)
{
    if(*size==0)
        return;

    if (index>*size)
        return;
    
    int lvalue = (*queue)[index];
    for(int i = index;i<(*size)-1;i++)
        (*queue)[i]=(*queue)[i+1];
    
    *queue=(int*)realloc(*queue,((*size)-1)*sizeof(int));
    (*size)--;
    return lvalue;
}

