#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stddef.h>
#include<sys/types.h>
#include<string.h>
#include<math.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<ifaddrs.h>
#include"Per_Process.c"
#include"myBase64.h"
#include<pthread.h>

typedef struct arguments{
    int fd;
    char*str;
}arguments;

void* ReturnRequest(void * arg)
{       
        struct arguments *params=(struct arguments*)arg;
        int fd=params->fd;
        char* str=strdup(params->str);
        char* key=Base64(str);
        
        int n=write(fd,key,strlen(key));
        if(n<0)
        {
            printf("Eroare la transmiterea mesajului\n");
            exit(0);
        } 
}
void* ListenfromAdmin(void* fd)
{
        int myfd=*((int*)fd);
        char *buffer=malloc(256);
        bzero(buffer,256);
        pthread_t th2;
        while (1)
    {
        int n=read(myfd,buffer,256);
        if(n>0)
        {
            char *aux=strdup(buffer);
            char *c=malloc(50);
            c=strtok(aux,"/");//NUME FUNCTIE
           // c=strtok(NULL,"/");//numele functiei
            char *numefunc=strdup(c);
            if(strcmp("base64",numefunc)==0);//daca functia ceruta este a noastra atunci o rulam
            {
                c=strtok(NULL,"/"); //in c este textul din fisier modificat
                //ReturnRequest(fd,c);
                struct arguments params;
                params.fd=myfd;
                params.str=strdup(c);
                pthread_create(&th2,NULL,ReturnRequest,(void*)&params);
                pthread_join(th2,NULL);
            }
        }
        bzero(buffer,256);
    }
}

int main(int argc, char** argv)
{
    
    if(argc<3)
    {
        printf("Error!\n");
        exit(0);
    }
    int sock_fd=CreateSocket(argv[1],argv[2]);
    pthread_t th1;
    char* buffer=malloc(256);
    bzero(buffer,256);
    //strcpy(buffer,"1/");//protocolul
    int fd=getpid();
    char* fd_p=malloc(10);
    sprintf(fd_p,"%d",fd);
    //strcat(buffer,fd_p);
    strcpy(buffer,fd_p);
    strcat(buffer,"/1");
    strcat(buffer,"/base64");//functia si parametrii
    int n=write(sock_fd,buffer,256);


    if(n<0)
    {
        printf("Eroare la trimitere!\n");
        exit(0);
    }

    //ListenfromAdmin(sock_fd);
    pthread_create(&th1,NULL,ListenfromAdmin,(void*)&sock_fd);
    pthread_join(th1,NULL);

    return 0;
}