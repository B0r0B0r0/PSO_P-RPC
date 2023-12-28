#include "Queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pushc(char ***queue, char *value, int *size)
{
	*queue = (char **)realloc(*queue, ((*size) + 1) * sizeof(char **));
	(*queue)[*size] = strdup(value);
	(*size)++;
}

char *popc(char ***queue, int *size, int index)
{
	if (*size == 0)
		return;

	if (index > *size)
		return;

	char *lvalue = strdup((*queue)[index]);
	for (int i = index; i < (*size) - 1; i++)
		(*queue)[i] = strdup((*queue)[i + 1]);

	*queue = (char **)realloc(*queue, ((*size) - 1) * sizeof(char *));
	(*size)--;
	return lvalue;
}

void pushi(int **queue, int value, int *size)
{
	*queue = (int *)realloc(*queue, ((*size) + 1) * sizeof(int));
	(*queue)[*size] = value;
	(*size)++;
}

int popi(int **queue, int index, int *size)
{
	if (*size == 0)
		return;

	if (index > *size)
		return;

	int lvalue = (*queue)[index];
	for (int i = index; i < (*size) - 1; i++)
		(*queue)[i] = (*queue)[i + 1];

	*queue = (int *)realloc(*queue, ((*size) - 1) * sizeof(int));
	(*size)--;
	return lvalue;
}

void pushPacket(packet **queue, packet payload, int *size)
{
	*queue = (packet *)realloc(*queue, ((*size) + 1) * sizeof(*queue));
	(*queue)[*size].id = payload.id;
	(*queue)[*size].prot = payload.prot;
	(*queue)[*size].sockfd = payload.sockfd;
	(*queue)[*size].payload = strdup(payload.payload);
	(*size)++;
}

packet popPapcket(packet **queue, int index, int *size)
{
	if (*size == 0)
		return;

	if (index > *size)
		return;

	packet lvalue;
	lvalue.sockfd = (*queue)[index].sockfd;
	lvalue.prot = (*queue)[index].prot;
	lvalue.id = (*queue)[index].id;
	lvalue.payload = strdup((*queue)[index].payload);

	for (int i = index; i < (*size) - 1; i++)
		(*queue)[i] = (*queue)[i + 1];

	*queue = (packet *)realloc(*queue, ((*size) - 1) * sizeof(packet));
	(*size)--;
	return lvalue;
}
