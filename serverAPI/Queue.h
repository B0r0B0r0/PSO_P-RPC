#ifndef QUEUE_H
#define QUEUE_H
#include "Structs.h"

// Coada Pachet
void pushPacket(packet **queue, packet payload, int *size);
packet popPacket(packet **queue, int index, int *size);
//

// Coada procese
void pushProcess(process **queue, process proc, int *size);
process popProcess(process **queue, int index, int *size);
//

#endif