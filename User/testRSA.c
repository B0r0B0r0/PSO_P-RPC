#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <time.h>
#include "PBAPI.h"

int main()
{
	con params;

	params.ipadd = strdup("127.0.0.1");
	params.portNumber = strdup("42002");
	char *a = Encrypt("deliaandreea", params);
	printf("%s\n", a);
	return 1;
}
