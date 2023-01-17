#pragma once 

#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <emmintrin.h>
#include <immintrin.h>


#include "types.h"
#include "global.h"


i32 convolve_baseline(i16 *m, i32 *f, u64 fh, u64 fw);
void sobel_baseline(i16 *cframe, i16 *oframe, f32 threshold);
void sobel_simd_v1(i16 *Aframe, i16 *Bframe, f32 threshold);
void sobel_simd_avx2(i16 *Aframe, i16 *Bframe, f32 threshold);
