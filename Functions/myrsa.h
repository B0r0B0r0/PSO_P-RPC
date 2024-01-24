#ifndef MYRSA_H
#define MYRSA_H

long int cd(long int x);
void ce();
void Message(char *ms);
int prime(long int pr) ;
char* encrypt(char *ms);
char* decrypt(char *ms);  

#endif