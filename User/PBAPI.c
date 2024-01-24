#include "PBAPI.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

int connectToServer(con param)
{
	// int socket = CreateSocket(param.ipadd, param.portNumber);
	// return socket;
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		printf("ERROR opening socket");

	serv_addr.sin_port = htons(atoi(param.portNumber));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(param.ipadd);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}
	return sockfd;
}

char *callfunction(char *payload, con param)
{
	int sockfd = connectToServer(param);
	char *buffer = calloc(strlen(payload) + 20, sizeof(char));
	strcpy(buffer, payload);
	int n = send(sockfd, buffer, strlen(buffer) + 20, 0);
	// trimitem datele catre procesul administrator
	if (n < 0)
	{
		printf("Error from sending data!\n");
		exit(-1);
	}

	char recBuffer[1024];
	n = recv(sockfd, recBuffer, 1024, 0);
	if (n < 0)
	{
		printf("Eroare la primirea datelor!\n");
		exit(-1);
	}

	shutdown(sockfd, 2);
	close(sockfd);
	return strdup(recBuffer);
}

int callfunction_noblock(char *payload, con param)
{
	int sockfd = connectToServer(param);
	char *newpayload = strdup("2/");
	sleep(1);
	srand(time(NULL));
	int iID = (unsigned)rand() / 1000;
	char aux[50];
	sprintf(aux, "%d/", iID);
	newpayload = realloc(newpayload, (strlen(newpayload) + strlen(aux) + strlen(payload) + 1) * sizeof(char));
	strcpy(newpayload + strlen(newpayload), aux);
	strcpy(newpayload + strlen(newpayload), payload);
	char *buffer = calloc(strlen(buffer) + 50, sizeof(char));
	strcpy(buffer, newpayload);
	int n = send(sockfd, buffer, strlen(buffer) + 50, 0);
	// trimitem datele catre procesul administrator

	if (n < 0)
	{
		printf("Error from sending data!\n");
		exit(-1);
	}

	shutdown(sockfd, 2);
	close(sockfd);
	usleep(100);
	return iID;
}

char *StergereDuplicate(char *text, con param)
{
	char *buffer = strdup("1/1/delete/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text));
	strcat(buffer, text);

	return callfunction(buffer, param);
}

int StergereDuplicate_noblock(char *text, con param)
{
	char *buffer = strdup("1/delete/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text) + 1);
	strcat(buffer, text);

	return callfunction_noblock(buffer, param);
}

char *Base64(char *text, con param)
{
	char *buffer = strdup("1/1/base64/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text));
	strcat(buffer, text);

	return callfunction(buffer, param);
}

int Base64_noblock(char *text, con param)
{
	char *buffer = strdup("1/base64/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text) + 1);
	strcat(buffer, text);

	return callfunction_noblock(buffer, param);
}

char *DeterminaFrecventa(char *text, char *cuv, con param)
{
	char *buffer = calloc(1024, sizeof(char));
	strcpy(buffer, "1/1/frecventa/");
	strcat(buffer, text);
	strcat(buffer, "/");
	strcat(buffer, cuv);

	return callfunction(buffer, param);
}

char *Encrypt(char *text, con param)
{
	char *buffer = strdup("1/1/encrypt/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text));
	strcat(buffer, text);

	return callfunction(buffer, param);
}

int DeterminaFrecventa_noblock(char *text, char *cuv, con param)
{
	char *buffer = strdup("1/frecventa/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text) + strlen(cuv) + 2);
	strcat(buffer, text);
	strcat(buffer, "/");
	strcat(buffer, cuv);

	return callfunction_noblock(buffer, param);
}

char *Hash(char *text, con param)
{
	char *buffer = strdup("1/1/hash/");
	buffer = realloc(buffer, strlen(buffer) + strlen(text));
	strcat(buffer, text);

	return callfunction(buffer, param);
}

int Hash_noblock(char *text, con param)
{
	char *buffer = strdup("1/hash/");
	buffer = (char *)realloc(buffer, (strlen(buffer) + strlen(text) + 1) * sizeof(char));
	strcpy(buffer + strlen(buffer), text);

	return callfunction_noblock(buffer, param);
}

char *getRet(int id, con param)
{
	usleep(1000);
	char payload[100];
	sprintf(payload, "2/%d/2", id);
	return callfunction(payload, param);
}
