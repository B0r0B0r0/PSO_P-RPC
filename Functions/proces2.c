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
#include"myStergereDuplicate.h"
#include<pthread.h>

typedef struct arguments{
    int fd;
    char*str;
}arguments;

void* ReturnRequest(void* arg)
{
    //aici thread-ul nr2 va executa functia dorita
    //se va ocupa de trimiterea rezultatului catre administrator
    //pid_t proces=CreateProcess();

        struct arguments *params=(struct arguments*)arg;
        int fd=params->fd;
        char* str=strdup(params->str);
      
        char* key=StergereDuplicate(str);
        printf("%s\n",key);
        
        int n=send(fd,key,strlen(key),0);
        if(n<=0)
        {
            printf("Eroare la transmiterea mesajului\n");
            exit(0);
        } 
        else printf("trimis!\n");
    
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
            printf("%s\n",buffer);
            char *c=malloc(50);
            c=strtok(aux,"/");//numele functiei
           // c=strtok(NULL,"/");//numele functiei
           // c=strtok(NULL,"/");
            char *numefunc=strdup(c);
            if(strcmp("delete",numefunc)==0)//daca functia ceruta este a noastra atunci o rulam
            {
                c=strtok(NULL,"/"); 
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
    strcpy(buffer,fd_p);
    //strcat(buffer,fd_p);
    memcpy(buffer+strlen(fd_p),"/1",3);
    memcpy(buffer+strlen(buffer),"/delete",8);
    //strcat(buffer,"/delete");//functia si parametrii
     char nullchar='\0';
    memcpy(buffer+strlen(buffer),&nullchar,1);
    printf("%s\n",buffer);
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