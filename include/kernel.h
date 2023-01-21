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
void sobel_baseline(f32 *restrict cframe, f32 *restrict oframe, f32 threshold);
void sobel_opti_v1(f32 *restrict cframe, f32 *restrict oframe, f32 threshold);
void sobel_opti_v2(f32 *restrict cframe, f32 *restrict oframe, f32 threshold);

void sobel_simd_avx2(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold);
void sobel_simd_avx2v2(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold);
void sobel_simd_avx2v3(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold);
void sobel_simd_avx2v4(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold);

//CL
void sobel_CL(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 * tmpx, f32 * tmpy);
void sobel_CL_AVX(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 * tmpx, f32 * tmpy);

//1D
void sobel_1D(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 * tmpx, f32 * tmpy);

// NOT WORKING
void sobel_simd_avx512(i16 *restrict Aframe, i16 *restrict Bframe, f32 threshold);
