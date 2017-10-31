## Makefile

CC     = gcc
# CFLAGS = -g3 -std=gnu99 -pedantic -Wall
CFLAGS = -std=c99 -pedantic -Wall
HWK    = /c/cs223/hw5

Cloud:  Cloud.o btree.o
	${CC} ${CFLAGS} -o Cloud Cloud.o btree.o

Cloud.o: Cloud.c
	${CC} ${CFLAGS} -c Cloud.c

## add stuff for btree's

btreetest: btreetest.o btree.o
	${CC} ${CFLAGS} -o btreetest btreetest.o btree.o

btreetest.o: btreetest.c /c/cs223/hw5/btree.h 
	${CC} ${CFLAGS} -c btreetest.c

btree.o: btree.c
	${CC} ${CFLAGS} -c btree.c

clean:
	rm -i *.o *~

