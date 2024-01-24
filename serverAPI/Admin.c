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
pthread_mutex_t mutex_coada_req = PTHREAD_MUTEX_INITIALIZER;

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
pthread_mutex_t mutex_coada_sends = PTHREAD_MUTEX_INITIALIZER;

// variabile procese
process *coadaProcesses;
int dimProc = 0;
pthread_mutex_t mutex_coada_proc = PTHREAD_MUTEX_INITIALIZER;

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

	int opt = 1;
	if (setsockopt(con->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

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
		users.message = recv(newsockfd, buffer, 256, 0);
		if (users.message < 0)
			printf("ERROR reading from socket");

		printf("mesajul primit este: %s\n", buffer);
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
			{
				bufAux[i] = buffer[i];
				i++;
			}

			aux.id = (unsigned int)atoi(bufAux);
			strcpy(buffer, buffer + i + 1);
		}
		else
			aux.id = 0;
		char crAux = buffer[0];
		strcpy(buffer, buffer + 2);

		if (crAux == '1')
		{
			aux.payload = strdup(buffer);
			aux.prot = 1;
			pthread_mutex_lock(&mutex_coada_req);
			pushPacket(&coadaRequests, aux, &dimRequests);
			pthread_mutex_unlock(&mutex_coada_req);
		}
		else
		{
			aux.payload = "\0";
			aux.prot = 2;
			pthread_mutex_lock(&mutex_coada_req);
			pushPacket(&coadaRequests, aux, &dimRequests);
			pthread_mutex_unlock(&mutex_coada_req);
		}

		if (aux.id != 0 && aux.prot != 2)
		{
			close(newsockfd);
			shutdown(newsockfd, 0);
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
		processes.message = recv(newsockfd, buffer, 256, 0);

		if (processes.message < 0)
			printf("ERROR reading from socket");

		process newProc;
		newProc.sockfd = newsockfd;

		char *token = strtok(buffer, "/");
		newProc.id = atoi(token);
		pthread_mutex_lock(&mutex_coada_proc);
		popProcess(&coadaProcesses, findProc(newProc.id), &dimProc);
		pthread_mutex_unlock(&mutex_coada_proc);

		token = strtok(NULL, "/");

		newProc.nrFunc = atoi(token);
		token = strtok(NULL, "/");

		newProc.functionTable = (char **)malloc(newProc.nrFunc * sizeof(char *));

		for (int i = 0; i < newProc.nrFunc; i++)
		{
			newProc.functionTable[i] = strdup(token);
			printf("%s\n", token);
			token = strtok(NULL, "/");
		}
		pthread_mutex_lock(&mutex_coada_proc);
		pushProcess(&coadaProcesses, newProc, &dimProc);
		pthread_mutex_unlock(&mutex_coada_proc);
	}
}

void *manageQueue(void *p)
{
	while (1)
	{
		pthread_mutex_lock(&mutex_coada_req);
		pthread_mutex_lock(&mutex);
		int dim = dimRequests;
		int cAT = curActiveThreads;
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&mutex_coada_req);

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
	bzero(funcName, 256);

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
	printf("%s\n", payload);
	char *buffer = calloc(1024, sizeof(char));
	strcpy(buffer, payload);
	int n = send(sockfd, buffer, 1024, 0);
	return n;
}

void searchAndSend(packet par)
{
	int id = par.id;
	for (int j = 0; j < 5; j++)
	{
		sleep(1);
		for (int i = 0; i < dimSends; i++)

			if (coadaSends[i].id == id)
			{
				pthread_mutex_lock(&mutex_coada_sends);
				packet toSend = popPacket(&coadaSends, i, &dimSends);
				pthread_mutex_unlock(&mutex_coada_sends);
				char *buffer = calloc(strlen(toSend.payload) + 20, sizeof(char));
				strcpy(buffer, toSend.payload);
				sendToSockFd(buffer, par.sockfd);
				goto end;
			}
	}
	sendToSockFd(par.payload, par.sockfd);
end:
}

#pragma endregion

void *calculate(void *p)
{
	pthread_mutex_lock(&mutex_coada_req);
	packet aux = popPacket(&coadaRequests, 0, &dimRequests);
	pthread_mutex_unlock(&mutex_coada_req);
	if (aux.sockfd == -1)
		goto final;

	// scot numele functiei din pachet
	// verific daca este protocol de calculate sau de send
	// indicat sa fac doua functii

	if (aux.prot == 2)
	{
		searchAndSend(aux);
		goto final;
	}

	char *response = malloc(1024 * sizeof(char));

	process proc = findFunction(aux.payload);
	if (proc.sockfd == -1)
	{
		// sendToSockFd("Function could not be found", aux.sockfd);
		// goto final;
		response = strdup("Function could not be found");
	}
	else
	{
		sendToSockFd(aux.payload, proc.sockfd);

		recv(proc.sockfd, response, 1024, 0);
	}
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
	pthread_mutex_lock(&mutex_coada_sends);
	pushPacket(&coadaSends, aux, &dimSends);
	pthread_mutex_unlock(&mutex_coada_sends);

final:
	pthread_mutex_lock(&mutex);
	curActiveThreads--;
	pthread_mutex_unlock(&mutex);
}

#pragma endregion

int main()
{
	printf("Server started!\n");
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex_coada_req, NULL);
	pthread_mutex_init(&mutex_coada_proc, NULL);
	pthread_mutex_init(&mutex_coada_sends, NULL);
	initialiseSocket(42002, &users);
	initialiseSocket(42001, &processes);
	printf("The sockets have been opened\n");
	initialiseManThread();
	initialiseListenThread();
	printf("Threads have been created\n");

	while (1)
	{
		//
	}
	return 0;
}