#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

char Base[64]  ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int* chartobin(char c) {
	int* vector = (int*)malloc(sizeof(int) * 8);
	char a = c;
	int j = 0;
	for (int i = 7; i >= 0; i--)
	{
		a = c;
		if ((a & (1 << i)) == (1<<i))
			vector[j] = 1;
		else vector[j] = 0;
		j++;
	}
	return vector;
}

int* resize(char* string)
{
	int x = 8 * strlen(string);
	int* aux = (int*)malloc(sizeof(int) * x);
	int k = 0;
	for (int i = 0; i < strlen(string); i++)
	{
		int* vector;
		vector = chartobin(string[i]);
	
		for (int j = 0; j < 8; j++)
		{
			aux[k] = vector[j];
			k++;
		}
	}
	int* aux2;
	if (x % 24 == 8)
	{
		int* aux2 = (int*)malloc(sizeof(int) * (x+8));
		for (int i = 0; i < x; i++)
			aux2[i] = aux[i];
		for (int i = x; i < x+8; i++)
			aux2[i] = 0;
		return aux2;
	}else if (x % 24 == 16)
		{
		int* aux2 = (int*)malloc(sizeof(int) * (x + 16));
		for (int i = 0; i <x; i++)
			
			aux2[i] = aux[i];
		for (int i = x; i < x+16; i++)
			aux2[i] = 0;
		return aux2;
	}
	return aux;
}

char* Base64(char* string)
{
	int* aux;
	int verif = 0;
	aux = resize(string);

	int x = 8 * strlen(string);
	if (x % 24 == 16) x = x + 16, verif = 2;
	else if (x % 24 == 8) x = x + 8, verif = 1;
	int y=x;
	if (verif == 2) y = x - 12;
	else if (verif == 1) y = x - 6;

	int* vect = (int*)malloc(sizeof(int) * (y / 6));
	int j = 0;
	for (int i = 0; i < y; i++)
	{
		vect[j] = 0;
		for (int k = 5; k >= 0; k--)
		{
			vect[j] = vect[j]+ pow(2, k) * aux[i];
			i++;
		}
		i--;
		j++;
	}

	y = y / 6;
	if (verif == 1) y++;
	else if (verif == 2) y = y + 2;

	char* fin = (char*)malloc(y*sizeof(char));

	for (int i = 0; i < y - verif; i++)
		fin[i] = Base[vect[i]];
	if (verif > 0)
		for (int i = y - verif; i < y; i++)
			fin[i] = '=';
	
	return fin;
}

