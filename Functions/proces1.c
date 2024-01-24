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
#include"Hash.h"
#include <pthread.h>

typedef struct arguments{
    int fd;
    unsigned char*str;
}arguments;

void *ReturnRequest(void * arg)
{
        struct arguments *params=(struct arguments*)arg;
        int fd=params->fd;
        unsigned char* str=strdup(params->str);
        unsigned long key=hash(str);
        int nr=snprintf(NULL,0,"%lu",key);
        char* buffer=malloc(nr);
        snprintf(buffer,nr+1,"%lu",key);

        int n=write(fd,buffer,strlen(buffer));
        if(n<0)
        {
            printf("Eroare la transmiterea mesajului\n");
            exit(0);
        }
}
void *ListenfromAdmin(void* fd)
{
        int myfd=*((int*)fd);
        char *buffer=malloc(256);
        bzero(buffer,256);
        pthread_t th2;//cel de-al doilea thread -- se ocupa cu return-ul
        while (1)
    {
        int n=read(myfd,buffer,256);        
        if(n>0)
        {
            printf("Primit de la administrator pentru prelucrare: %s\n",buffer);
            char *aux=strdup(buffer);
            char *c=malloc(50);
           // c=strtok(aux,"/");//acum avem protocolul
            c=strtok(aux,"/");//numele functiei
            char *numefunc=strdup(c);
            if(strcmp("hash",numefunc)==0);//daca functia ceruta este a noastra atunci o rulam
            {
                c=strtok(NULL,"/"); //in c sunt parametrii
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
    pthread_t th1;// --primul thread se ocupa cu ascultarea
    
    char* buffer=malloc(256);
    bzero(buffer,256);
    //strcpy(buffer,"1/");//protocolul
    int fd=getpid();//asta ramane id
    //trebuie sa mai ramana numarul de functii
    char* fd_p=malloc(10);
    sprintf(fd_p,"%d",fd);
    strcpy(buffer,fd_p);
    strcat(buffer,"/1");//asta este numarul de functii per proces
    //strcat(buffer,fd_p);
    strcat(buffer,"/hash");//functia si parametrii
   
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