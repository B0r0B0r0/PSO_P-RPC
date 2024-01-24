#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int Frecventa(char* str, char* cuv)
{
    //int f=open(filename,O_RDONLY);
	/*if(f==-1)
	{
	printf("Eroare la deschiderea fisierului!\n");
	exit(0);
	}*/
	//int x=NrCuvinte(filename);
	int x=1;
	for(int i=0;i<strlen(str);i++)
	{ if(str[i]==' ')
		x++; }
		printf("%d\n",x);
		printf("%s\n",cuv);
	char **vector=(char**)malloc(sizeof(char*)*x);
	if(vector==NULL) printf("Eroare la alocare");
	
	//char buffer[10000];
	//int i=0;
	//int nr=NrLinii(filename);
	
    /*for(int i=0;i<x;i++)
    vector[i]=(char*)malloc(10);

    char c;
	int j=0,k=0;
	ssize_t bytesRead;
	while((bytesRead=read(f,&c,1))>0)//asa memorez cuvintele din fisier intr-un vector de cuvinte
	{
	    if(c=='\n'|| c==' ')
	    {
            j++;k=0;
        }
        else{
            vector[j][k]=c;
            k++;
        }*/
	//}
	int i;
	char* cuvant=malloc(10);
	cuvant=strtok(str," ");
	vector[0]=strdup(cuvant);
	printf("%s\n",cuvant);
	printf("%d\n",i);
	for( i=1;i<9;i++)
	{
		cuvant=strtok(NULL," ");
		vector[i]=strdup(cuvant);
		printf("%s\n",cuvant);
		printf("%d\n",i);
		
	}
    
	int numara=0;
	for(int i=0;i<x;i++)
	{
	if(strcmp(cuv,vector[i])==0)
	numara++;
	}
	//close(f);
	printf("%d\n",numara);
	return numara;
}
