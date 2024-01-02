#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

void main()
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];

	portno = 42002;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		printf("ERROR opening socket");

	serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}
	printf("Connection succesful!\n");

	printf("Please enter the message: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		printf("ERROR writing to socket");
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
		printf("ERROR reading from socket");
	printf("Received message is: %s", buffer);
	printf("Please enter the message: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		printf("ERROR writing to socket");
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
		printf("ERROR reading from socket");
	printf("Received message is: %s", buffer);
	while (1)
		;
}