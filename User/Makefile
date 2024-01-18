LDFLAGS = -L. -lPBAPI

all: libPBAPI.a testFunc testconnect testBase64 testRSA testSD testFA

libPBAPI.a: PBAPI.o
	ar rc libPBAPI.a PBAPI.o
	ranlib libPBAPI.a

PBAPI.o: PBAPI.c Structs.h
	gcc -c PBAPI.c

testFunc: testFunc.c libPBAPI.a
	gcc -o testFunc testFunc.c $(LDFLAGS)

testBase64: testBase64.c libPBAPI.a
	gcc -o testBase64 testBase64.c $(LDFLAGS)

testRSA: testRSA.c libPBAPI.a
	gcc -o testRSA testRSA.c $(LDFLAGS)

testSD: testSD.c libPBAPI.a
	gcc -o testSD testSD.c $(LDFLAGS)
 
testFA: testFA.c libPBAPI.a
	gcc -o testFA testFA.c $(LDFLAGS)
 
testconnect: testconnect.c libPBAPI.a
	gcc -o testconnect testconnect.c $(LDFLAGS)

clean:
	rm -f *.o *.a testFunc
