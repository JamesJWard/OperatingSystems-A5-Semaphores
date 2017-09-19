CFLAGS = -Wall
CC = gcc

prog: main.o
	${CC} ${CFLAGS} -o prog main.o -lpthread

main.o: main.c header.h

clean:
	rm *.o prog