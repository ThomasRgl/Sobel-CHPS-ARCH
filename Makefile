# CC=/opt/aocc/bin/clang
CC=gcc

# BASELINE=___
# AVX2=___
# OPT1=___
DEFINE= -D$(BASELINE) -D$(AVX2) -D$(OPT1)

CFLAGS= -g3 -ggdb
# OFLAGS= -O3 -march=native -mtune=native -funroll-loops -ftree-vectorize
OFLAGS= -Ofast -march=native -mtune=native -funroll-loops -ftree-vectorize 
# OFLAGS= -Ofast -ffast-math -march=native -mtune=native -funroll-loops -ftree-vectorize

#OFLAGS= -Ofast -ffast-math -march=native -mtune=native 
# OFLAGS= -O1 -march=skylake-avx512
LDFLAGS= -lm

INCLUDE=-I include/

all: common kernel main
	$(CC) $(CFLAGS) $(OFLAGS) $(LDFLAGS) $(INCLUDE) build/* -o sobel  $(LDFLAGS)

common: src/common.c
	$(CC) -c $(CFLAGS) $(OFLAGS) $(INCLUDE) $< -o build/$@ 

main: src/main.c
	$(CC) -c -D$(DEFINE) $(CFLAGS) $(OFLAGS) $(INCLUDE) $< -o build/$@ 

kernel: src/kernel.c
	$(CC) -c $(CFLAGS) $(OFLAGS) $(INCLUDE) $< -o build/$@ 


clean:
	rm -Rf *~ sobel build/*
