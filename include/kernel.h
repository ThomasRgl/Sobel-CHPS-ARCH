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


i32 convolve_baseline(f32 *m, i32 *f, u64 fh, u64 fw);
void sobel_baseline(f32 *cframe, f32 *oframe, f32 threshold);
void sobel_opti_v1(f32 *cframe, f32 *oframe, f32 threshold);
void sobel_opti_v2(f32 *cframe, f32 *oframe, f32 threshold);

void sobel_simd_avx2(f32 *Aframe, f32 *Bframe, f32 threshold);
void sobel_simd_avx2v2(f32 *Aframe, f32 *Bframe, f32 threshold);

// NOT WORKING
void sobel_simd_avx512(i16 *Aframe, i16 *Bframe, f32 threshold);
