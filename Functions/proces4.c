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
#include"myFrecventa.h"
#include<pthread.h>

typedef struct arguments{
    char* str;
    int fd;
    char* caut;
}arguments;

void* ReturnRequest(void* arg)
{
        struct arguments *params=(struct arguments*)arg;
        int fd=params->fd;
        char* str=strdup(params->str);
          printf("000 %s\n",params->str);
        //char *separa=malloc(10000);
        printf("1. %s\n",params->caut);
        int nr=Frecventa(params->str,params->caut);
        char* buffer=malloc(10);
        sprintf(buffer,"%d",nr);
        printf("%s\n",buffer);
        int n=write(fd,buffer,strlen(buffer));
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
            c=strtok(aux,"/");//NUELE FUNCTIEI
            //c=strtok(NULL,"/");//numele functiei
            char *numefunc=strdup(c);
            if(strcmp("frecventa",numefunc)==0);//daca functia ceruta este a noastra atunci o rulam
            {
                c=strtok(NULL,"/");//aici avem paramterii functiei
                
               // ReturnRequest(fd,c);
                struct arguments params;
                params.fd=myfd;
                params.str=strdup(c);
                c=strtok(NULL,"/");
                params.caut=strdup(c);
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
    memcpy(buffer+strlen(fd_p),"/1",3);
    memcpy(buffer+strlen(buffer),"/frecventa",11);
    //strcat(buffer,"/delete");//functia si parametrii
     char nullchar='\0';
    memcpy(buffer+strlen(buffer),&nullchar,1);
    printf("%s\n",buffer);//functia si parametrii
   
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