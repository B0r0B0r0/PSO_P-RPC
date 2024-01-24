#include "Queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pushPacket(packet **queue, packet payload, int *size)
{
	*queue = (packet *)realloc(*queue, ((*size) + 1) * sizeof(packet));
	(*queue)[*size].id = payload.id;
	(*queue)[*size].prot = payload.prot;
	(*queue)[*size].sockfd = payload.sockfd;
	(*queue)[*size].payload = strdup(payload.payload);
	(*size)++;
}

packet popPacket(packet **queue, int index, int *size)
{
	packet lvalue;
	lvalue.sockfd = -1;
	if (*size == 0)
		return lvalue;

	if (index > *size)
		return lvalue;

	lvalue.sockfd = (*queue)[index].sockfd;
	lvalue.prot = (*queue)[index].prot;
	lvalue.id = (*queue)[index].id;
	lvalue.payload = strdup((*queue)[index].payload);

	for (int i = index; i < (*size); i++)
		(*queue)[i] = (*queue)[i + 1];

	*queue = (packet *)realloc(*queue, ((*size) - 1) * sizeof(packet));
	(*size)--;
	return lvalue;
}

void pushProcess(process **queue, process proc, int *size)
{
	*queue = (process *)realloc(*queue, ((*size) + 1) * sizeof(process));
	(*queue)[*size].id = proc.id;
	(*queue)[*size].nrFunc = proc.nrFunc;
	(*queue)[*size].sockfd = proc.sockfd;
	(*queue)[*size].functionTable = (char **)malloc(proc.nrFunc * sizeof(char *));
	for (int i = 0; i < proc.nrFunc; i++)
		(*queue)[*size].functionTable[i] = strdup(proc.functionTable[i]);

	(*size)++;
}

process popProcess(process **queue, int index, int *size)
{
	process lvalue;
	lvalue.sockfd = -1;

	if (*size == 0)
		return lvalue;

	if (index > *size)
		return lvalue;

	if (index < 0)
		return lvalue;

	lvalue.id = (*queue)[index].id;
	lvalue.nrFunc = (*queue)[index].nrFunc;
	lvalue.sockfd = (*queue)[index].sockfd;
	lvalue.functionTable = (char **)malloc(lvalue.nrFunc * sizeof(char *));
	for (int i = 0; i < lvalue.nrFunc; i++)
		lvalue.functionTable[i] = strdup((*queue)[index].functionTable[i]);

	for (int i = index; i < (*size) - 1; i++)
		(*queue)[i] = (*queue)[i + 1];

	*queue = (process *)realloc(*queue, ((*size) - 1) * sizeof(process));
	(*size)--;
	return lvalue;
}
