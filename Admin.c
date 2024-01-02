#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include "Structs.h"
#include "Queue.h"

#pragma region Global Variables
// variabile coada requesturi
packet *coadaRequests;
int dimRequests = 0;

// variabile threaduri
int curActiveThreads = 0;
pthread_mutex_t mutex;

// variabile conexiuni
// int sockfd, portno, clilen, n;
// struct sockaddr_in serv_addr, cli_addr;
connection processes, users;

// variabile coada Send
packet *coadaSends;
int dimSends = 0;

// variabile procese
process *coadaProcesses;
int dimProc = 0;

#pragma endregion

#pragma region Initializari

void *calculate(void *);
void *manageQueue(void *);
void *listenProcesses(void *);
void *listenUsers(void *);

void initialiseSocket(int portNumber, connection *con)
{
	// deschid socket
	con->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (con->sockfd < 0)
		printf("ERROR opening socket");

	// initializez serv_addr cu 0-uri
	bzero((char *)&con->serv_addr, sizeof(con->serv_addr));

	con->portno = portNumber;
	con->serv_addr.sin_family = AF_INET;
	con->serv_addr.sin_port = htons(con->portno);
	con->serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(con->sockfd, (struct sockaddr *)&con->serv_addr, sizeof(con->serv_addr)) < 0)
		printf("ERROR on binding");

	listen(con->sockfd, 10);
}

void initialiseManThread()
{
	pthread_t id;
	pthread_create(&id, NULL, manageQueue, NULL);
}

void initialiseListenThread()
{
	pthread_t id1, id2;
	pthread_create(&id1, NULL, listenUsers, NULL);
	pthread_create(&id1, NULL, listenProcesses, NULL);
}
#pragma endregion

#pragma region Main thread functions

// void sending(connection *con)
// {
// 	if (dimSend == 0)
// 		return;

// 	tuplacc pachet = popt(&coadaSends, &dimSend, 0);
// 	int sndsockfd = popi(&conexiuni, atoi(pachet.a), &dimConex);
// 	printf("%s\n", pachet.b);
// 	pthread_mutex_lock(&mutex);
// 	con->message = write(sndsockfd, pachet.b, strlen(pachet.b));
// 	// n = write(sndsockfd,"Here is your message",20);
// 	pthread_mutex_unlock(&mutex);
// 	if (con->message < 0)
// 		printf("ERROR writing to socket");

// 	shutdown(sndsockfd, 0);
// 	close(sndsockfd);
// }

#pragma endregion

#pragma region Thread Functions

int findProc(int id)
{
	for (int i = 0; i < dimProc; i++)
		if (coadaProcesses[i].id == id)
			return i;

	return -1;
}

void *listenUsers(void *p)
{
	// connection *con = (connection *)p;
	while (1)
	{
		users.clilen = sizeof(users.cli_addr);
		int newsockfd = accept(users.sockfd, (struct sockaddr *)&users.cli_addr, &users.clilen);
		printf("Connection has been tried\n");
		if (newsockfd < 0)
			return NULL;

		char buffer[256];
		users.message = read(newsockfd, buffer, 256);
		if (users.message < 0)
			printf("ERROR reading from socket");

		// decapsulare + formare packet de tipul struct

		packet aux;
		aux.sockfd = newsockfd;
		char protAux = buffer[0];
		strcpy(buffer, buffer + 2);

		if (protAux == '2')
		{
			char bufAux[16];
			int i = 0;
			while (buffer[i] != '/')
				bufAux[i] = buffer[i++];
			aux.id = atoi(bufAux);
			strcpy(buffer, buffer + i);
		}
		else
			aux.id = 0;

		char crAux = buffer[0];
		strcpy(buffer, buffer + 2);

		if (crAux == '1')
		{
			aux.payload = strdup(buffer);
			aux.prot = 1;
			pushPacket(&coadaRequests, aux, &dimRequests);
		}
		else
		{
			aux.payload = "\0";
			aux.prot = 2;
			pushPacket(&coadaRequests, aux, &dimRequests);
		}
	}
}

void *listenProcesses(void *p)
{
	while (1)
	{
		processes.clilen = sizeof(processes.cli_addr);
		int newsockfd = accept(processes.sockfd, (struct sockaddr *)&processes.cli_addr, &processes.clilen);

		if (newsockfd < 0)
			return NULL;

		char buffer[256];
		processes.message = read(newsockfd, buffer, 256);
		if (processes.message < 0)
			printf("ERROR reading from socket");

		process newProc;
		newProc.sockfd = newsockfd;

		char *token = strtok(buffer, "/");
		newProc.id = atoi(token);

		popProcess(&coadaProcesses, findProc(newProc.id), &dimProc);

		token = strtok(NULL, "/");

		newProc.nrFunc = atoi(token);
		token = strtok(NULL, "/");

		newProc.functionTable = (char **)malloc(newProc.nrFunc * sizeof(char *));

		for (int i = 0; i < newProc.nrFunc; i++)
		{
			newProc.functionTable[i] = strdup(token);
			token = strtok(NULL, "/");
		}

		pushProcess(&coadaProcesses, newProc, &dimProc);
	}
}

void *manageQueue(void *p)
{
	while (1)
	{
		int dim = dimRequests;
		int cAT = curActiveThreads;

		if (cAT < 15 && dim > 0)
		{
			pthread_t id;
			pthread_create(&id, NULL, calculate, NULL);
			pthread_mutex_lock(&mutex);
			curActiveThreads++;
			pthread_mutex_unlock(&mutex);
		}
	}
}

#pragma region Calculate Thread Functions

process findFunction(char *function)
{
	// tre sa scot numele functiei

	char funcName[256];

	for (int i = 0; i < strlen(function); i++)
	{
		if (function[i] == '/')
		{
			strncpy(funcName, function, i);
			break;
		}
	}

	for (int i = 0; i < dimProc; i++)
		for (int j = 0; j < coadaProcesses[i].nrFunc; i++)
			if (strcmp(coadaProcesses[i].functionTable[j], funcName) == 0)
				return coadaProcesses[i];

	process err;
	err.sockfd = -1;
	return err;
}

int sendToSockFd(char *payload, int sockfd)
{

	if (strlen(payload) == 0)
		return -1;

	if (sockfd == -1)
		return -1;

	int n = write(sockfd, payload, strlen(payload));
	return n;
}

void searchAndSend(packet par)
{
	int id = par.id;
	while (1)
	{
		for (int i = 0; i < dimSends; i++)
			if (coadaSends[i].id == id)
			{
				packet toSend = popPacket(&coadaSends, i, &dimSends);
				sendToSockFd(toSend.payload, par.sockfd);
				break;
			}

		sleep(1);
	}
}

#pragma endregion

void *calculate(void *p)
{
	packet aux = popPacket(&coadaRequests, 0, &dimRequests);
	if (aux.sockfd == -1)
		goto final;
	// scot numele functiei din pachet
	// verific daca este protocol de calculate sau de send
	// indicat sa fac doua functii

	sendToSockFd("Received!\n", aux.sockfd);
	goto final;

	if (aux.prot == 2)
	{
		searchAndSend(aux);
		goto final;
	}

	process proc = findFunction(aux.payload);
	if (proc.sockfd == -1)
	{
		sendToSockFd("Function could not be found", aux.sockfd);
		goto final;
	}
	sendToSockFd(aux.payload, proc.sockfd);

	char response[1024];
	read(proc.sockfd, response, strlen(response));

	// trimitere pachet la emitator
	if (aux.id == 0)
	{
		sendToSockFd(response, aux.sockfd);
		shutdown(aux.sockfd, 0);
		close(aux.sockfd);
		goto final;
	}
	//

	// incapsulare pachet nou
	aux.prot = 2;
	aux.payload = strdup(response);
	pushPacket(&coadaSends, aux, &dimSends);

final:
	pthread_mutex_lock(&mutex);
	curActiveThreads--;
	pthread_mutex_unlock(&mutex);
}

#pragma endregion

void main()
{
	printf("Server started!\n");
	pthread_mutex_init(&mutex, NULL);
	initialiseSocket(42002, &users);
	initialiseSocket(42001, &processes);
	printf("The sockets have been opened\n");
	initialiseManThread();
	initialiseListenThread();
	printf("Threads have been created\n");

	while (1)
	{
		// listening();
		// sending();
	}
}