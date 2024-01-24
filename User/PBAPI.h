#ifndef PBAPI_H
#define PBAPI_H

typedef struct con
{
	char *ipadd, *portNumber;
} con;

char *StergereDuplicate(char *text, con param);
char *Hash(char *text, con param);
char *Base64(char *text, con param);
char *DeterminaFrecventa(char *text, char *cuv, con param);
char *Encrypt(char *text, con param);


char *getRet(int id, con param);

int StergereDuplicate_noblock(char *text, con param);
int Hash_noblock(char *payload, con param);
int Base64_noblock(char *text, con param);
int DeterminaFrecventa_noblock(char *text, char *cuv, con param);

#endif