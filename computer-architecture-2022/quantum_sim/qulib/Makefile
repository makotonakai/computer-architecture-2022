CC=gcc
CFLAGS=-g -fopenmp -O4

all: libqu.a

libqu.a: norm.o File.o sim.o
	ar rcs libqu.a norm.o File.o sim.o

norm.o: norm.c norm.h

File.o: File.c File.h

sim.o: sim.c sim.h

clean:
	rm *.o *.a
