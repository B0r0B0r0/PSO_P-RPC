#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stddef.h>
#include<sys/types.h>
#include<string.h>
#include<math.h>
#include"myBase64.h"


int main()
{
    char* text = (char*)malloc(50);
printf("Introduceti un mesaj:\n");
scanf("%s", text);
char* aux;
aux = Base64(text);
int x = 8 * strlen(text);
if (x % 24 == 16) x = x + 16;
else if (x % 24 == 8) x = x + 8;
x = x / 6;

for (int i = 0; i < strlen(aux); i++)
	printf("%c", aux[i]);
printf("\n");
    return 0;
}