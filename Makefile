CC=gcc

CFLAGS=-g3
#OFLAGS= -O1 -march=native
OFLAGS= -O1 -march=skylake-avx512
LDFLAGS= -lm

INCLUDE=-I include/

all: common kernel main
	$(CC) $(CFLAGS) $(OFLAGS) $(LDFLAGS) $(INCLUDE) build/* -o sobel  $(LDFLAGS)

common: src/common.c
	$(CC) -c $(CFLAGS) $(OFLAGS) $(INCLUDE) $< -o build/$@ 

main: src/main.c
	$(CC) -c -DBASELINE=1 $(CFLAGS) $(OFLAGS) $(INCLUDE) $< -o build/$@ 

kernel: src/kernel.c
	$(CC) -c $(CFLAGS) $(OFLAGS) $(INCLUDE) $< -o build/$@ 


clean:
	rm -Rf *~ sobel build/*
