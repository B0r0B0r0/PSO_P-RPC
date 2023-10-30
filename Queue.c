#include "Queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void push(tuplacc** queue,tuplacc value,int* size)
{
    *queue = (tuplacc*)realloc(*queue,((*size)+1)*sizeof(tuplacc));
    (*queue)[*size].a = strdup(value.a);
    (*queue)[*size].b = strdup(value.b);
    (*size)++;
}

tuplacc pop(tuplacc** queue,int *size)
{
    if(*size==0)
        return;

    tuplacc lvalue = *queue[0];

    for(int i = 0; i<(*size)-1;i++)
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
