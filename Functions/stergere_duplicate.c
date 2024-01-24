// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// char** StergereDuplicate(char *str)
// {
//     int x=1;
//     for(int i=0;i<strlen(str);i++)
//     {
//         if(str[i]==' ')
//             x++;
//     }
//     char **vector=(char**)malloc(sizeof(char*)*x);

//     char*ch=malloc(10);
//     ch=strtok(str," ");
//     int i=0;
//     while(ch!=NULL)
//     {
//         vector[i]=strdup(ch);
//         ch=strtok(NULL," ");
//     }
//     for (int m = 0; m < x; m++) {
// 		for (int n = m+1; n < x; n++) {
// 			if (strcmp(vector[m], vector[n]) == 0)
// 			{
// 				for (int l = n; l < x-1; l++)
// 					strcpy(vector[l], vector[l + 1]);
// 				x--;
// 			}
// 		}
// 	}
// 	char** vector_nou = (char**)malloc(sizeof(char*) * x);
// 	for (int i = 0; i < x; i++)
// 		vector_nou[i]=strdup(vector[i]);

//     return vector_nou;
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
char* StergereDuplicate(char*str)
{
	char  word[100], twoD[10][30];
	int i = 0, j = 0, k = 0, len1 = 0, len2 = 0, l = 0;
 
	// printf ("Enter the string\n");
	// gets (str);

 
	// let us convert the string into 2D array
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == ' ')
		{
			twoD[k][j] = '\0';
			k ++;
			j = 0;
		}
		else
		{
			twoD[k][j] = str[i];
			j ++;
		}
	}
 
	twoD[k][j] = '\0';
 
	j = 0;
	for (i = 0; i < k; i++)
	{
		int present = 0;
		for (l = 1; l < k + 1; l++)
		{
			if (twoD[l][j] == '\0' || l == i)
			{
				continue;
			}
 
			if (strcmp (twoD[i], twoD[l]) == 0) {
				twoD[l][j] = '\0';
				present = present + 1;
			}
		}
		// if (present > 0)	     | uncomment this `if` block if you
		// {			     | want to remove all the occurrences 
		// 	twoD[i][j] = '\0';   | of the words including the word
		// }			     | itself.
	}
 
	j = 0;
 char *final=malloc(500);
 strcpy(final," ");
 bzero(final,500);
	for (i = 0; i < k + 1; i++)
	{
		if (twoD[i][j] == '\0')
			continue;
		else
			//printf ("%s ", twoD[i]); 
            strcat(final,twoD[i]),strcat(final," ");
	}
 
	printf ("\n");
 
	return final;
}