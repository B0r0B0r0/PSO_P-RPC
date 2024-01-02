#ifndef STRUCTS_H
#define STRUCTS_H

#include <netinet/in.h>
typedef struct packet
{
	int sockfd, prot, id;
	char *payload;
} packet;

typedef struct process
{
	int id, sockfd, nrFunc;
	char **functionTable;
} process;

typedef struct connection
{
	int sockfd, portno, clilen, message;
	struct sockaddr_in serv_addr, cli_addr;
} connection;

#endif