CC=gcc
CFLAGS=-g -fopenmp -O4
INCLUDES=-I../qulib
LIBS=../qulib/libqu.a

all: hw1 ${LIBS}

hw1: main.o 
	${CC} -o hw1 main.o ${LIBS} ${CFLAGS} -lm

main.o: main.c
	${CC} -c ${INCLUDES} ${CFLAGS} main.c

clean:
	rm *.o *.a
