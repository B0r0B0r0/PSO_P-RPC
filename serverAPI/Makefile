CC = gcc
LDFLAGS = -L . -lQueue

all: libQueue.a admin

libQueue.a: Queue.o
	ar rc libQueue.a Queue.o
	ranlib libQueue.a

Queue.o: Queue.c Structs.h
	$(CC) -c Queue.c

admin: Admin.c libQueue.a
	$(CC) -o admin Admin.c $(LDFLAGS)


clean:
	rm -f *.o *.a admin
