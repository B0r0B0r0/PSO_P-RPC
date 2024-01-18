#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <time.h>
#include "PBAPI.h"

int suma(int a, int b)
{
	printf("param1: %d\tparam2: %d\n", a, b);
	return a + b;
}

int main()
{

	con params;

	params.ipadd = strdup("127.0.0.1");
	params.portNumber = strdup("42002");
	char *h, *b, *sd, *fa;
	h = Hash("deliaandreea", params);
	b = Base64("deliaandreea", params);
	sd = StergereDuplicate("ana ana are mere multe multe si bune bune", params);
	fa = DeterminaFrecventa("ana ana are mere multe multe si bune bune", "ana", params);
	printf("hash: %s\nbase64: %s\nStergereDuplicate: %s\nFrecventa ana: %s\n", h, b, sd, fa);

	char *en = Encrypt("delia", params);
	printf("criptare RSA: %s\n", en);

	return 1;
}