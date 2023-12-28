#ifndef QUEUE_H
#define QUEUE_H
#include "Structs.h"

// Coada Pachet
void pushPacket(packet **queue, packet payload, int *size);
packet popPapcket(packet **queue, int index, int *size);
//

// coada char
void pushc(char ***, char *, int *);
char *popc(char ***, int *, int);

// coada int

void pushi(int **, int, int *);
int popi(int **, int, int *);

#endif