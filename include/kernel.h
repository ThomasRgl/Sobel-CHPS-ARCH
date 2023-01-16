#pragma once 

#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "global.h"


i32 convolve_baseline(u8 *m, i32 *f, u64 fh, u64 fw);
void sobel_baseline(u8 *cframe, u8 *oframe, f32 threshold);
