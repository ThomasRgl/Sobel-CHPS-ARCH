#pragma once 

#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "global.h"

//
void sort(f64 *a, u64 n);

// Arithmetic mean
f64 mean(f64 *a, u64 n);

// Standard deviation
f64 stddev(f64 *a, u64 n);


void grayscale_weighted(u8 *frame);

void grayscale_sampled(u8 *frame);
