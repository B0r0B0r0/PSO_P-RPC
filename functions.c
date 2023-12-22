#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include"Per_Process.c"

int connectToServer(char* ipadd, char* portNumber)
{
        int socket=CreateSocket(ipadd,portNumber);
        return socket;
}

char* StergereDuplicate(char *text,char *ipadd,char *portNumber)
{
        char *newbuffer=(char*)malloc(strlen(text));

       //trebuie sa ma conectez la server
       //acum ar trebui trimis catre procesul administrator protocolul
       //numele functiei si parametrii si sa astepte in raspuns valoarea de return

        int sock=connectToServer(ipadd,portNumber);
        
        char *trimite=malloc(strlen(text)+9);
        strcpy(trimite,"2/Delete/");
        strcat(trimite,text);
        
        int n=write(sock,trimite,strlen(trimite));
        //trimitem datele catre procesul administrator
        if(n<0){printf("Error from sending data!\n"); exit(-1);}

        //primim rezultatul functiei
        char buffer[100000];
        int receive=read(sock,buffer,100000);
       
        if(receive<0) {printf("Eroare la primirea datelor!\n"); exit(-1);}
        // return newbuffer;
        strcpy(newbuffer,buffer);
       close(sock);
       return newbuffer;
      
}
char* Base64(char *text,char *ipadd,char *portNumber)
{
        char *newbuffer;

       //trebuie sa ma conectez la server
       //acum ar trebui trimis catre procesul administrator protocolul
       //numele functiei si parametrii si sa astepte in raspuns valoarea de return

        int sock=connectToServer(ipadd,portNumber);
        
        char *trimite=malloc(strlen(text)+9);
        strcpy(trimite,"2/Base64/");
        strcat(trimite,text);
        
        int n=write(sock,trimite,strlen(trimite));
        //trimitem datele catre procesul administrator
        if(n<0){printf("Error from sending data!\n"); exit(-1);}

        //primim rezultatul functiei
        char buffer[100000];
        int receive=read(sock,buffer,100000);
       
        if(receive<0) {printf("Eroare la primirea datelor!\n"); exit(-1);}
        // return newbuffer;
        newbuffer=malloc(strlen(buffer));
        strcpy(newbuffer,buffer);
       close(sock);
       return newbuffer;
      
}
char* DeterminaFrecventa(char *text,char *ipadd,char *portNumber)
{
        char *newbuffer;

       //trebuie sa ma conectez la server
       //acum ar trebui trimis catre procesul administrator protocolul
       //numele functiei si parametrii si sa astepte in raspuns valoarea de return

        int sock=connectToServer(ipadd,portNumber);
        
        char *trimite=malloc(strlen(text)+13);
        strcpy(trimite,"2/Frecventa/");
        strcat(trimite,text);
        
        int n=write(sock,trimite,strlen(trimite));
        //trimitem datele catre procesul administrator
        if(n<0){printf("Error from sending data!\n"); exit(-1);}

        //primim rezultatul functiei
        char buffer[100000];
        int receive=read(sock,buffer,100000);
       
        if(receive<0) {printf("Eroare la primirea datelor!\n"); exit(-1);}
        // return newbuffer;
        newbuffer=malloc(strlen(buffer));
        strcpy(newbuffer,buffer);
       close(sock);
       return newbuffer;
      
}
char* Hash(char *text,char *ipadd,char *portNumber)
{
        char *newbuffer;

       //trebuie sa ma conectez la server
       //acum ar trebui trimis catre procesul administrator protocolul
       //numele functiei si parametrii si sa astepte in raspuns valoarea de return

        int sock=connectToServer(ipadd,portNumber);
        
        char *trimite=malloc(strlen(text)+8);
        strcpy(trimite,"2/Hash/");
        strcat(trimite,text);
        
        int n=write(sock,trimite,strlen(trimite));
        //trimitem datele catre procesul administrator
        if(n<0){printf("Error from sending data!\n"); exit(-1);}

        //primim rezultatul functiei
        char buffer[100000];
        int receive=read(sock,buffer,100000);
       
        if(receive<0) {printf("Eroare la primirea datelor!\n"); exit(-1);}
        // return newbuffer;
        newbuffer=malloc(strlen(buffer));
        strcpy(newbuffer,buffer);
       close(sock);
       return newbuffer;
      
}
