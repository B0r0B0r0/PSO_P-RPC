#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

int CreateSocket(const char *ipAddress, const char *portNumber)
{
    int descriptor, portNum, n;
    struct sockaddr_in adresaServer;
    struct hostent *server;

    // char buffer[256];

    portNum = atoi(portNumber);
    descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor < 0)
    {
        printf("Eroare la crearea descriptorului!");
        exit(0);

    }
    bzero((char *)&adresaServer, sizeof(adresaServer));

    adresaServer.sin_family = AF_INET;
    adresaServer.sin_addr.s_addr = inet_addr(ipAddress);
    adresaServer.sin_port = htons(portNum);

    int Conectare = connect(descriptor, (struct sockaddr *)&adresaServer, sizeof(adresaServer));

    if (Conectare < 0)
    {
        printf("Eroare la conectare!\n");
        exit(0);
    }else printf("Succes!\n");

    return descriptor;
}

