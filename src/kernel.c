#include "kernel.h"

//
i32 convolve_baseline(u8 *m, i32 *f, u64 fh, u64 fw) {
    i32 r = 0;

    for (u64 i = 0; i < fh; i++)
        for (u64 j = 0; j < fw; j++)
            r += m[INDEX(i, j, W) ] * f[INDEX(i, j, fw)];

    return r;
}

//
void sobel_baseline(u8 *Aframe, u8 *Bframe, f32 threshold) {
    i32 gx, gy;
    f32 mag = 0.0;

    i32 f1[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1}; // 3x3 matrix

    i32 f2[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1}; // 3x3 matrix

    //
    for (u64 i = 0; i < (H - 2); i++)
        for (u64 j = 0; j < (W  - 2); j++) {
            gx = convolve_baseline(&Aframe[INDEX(i, j, W )], f1, 3, 3);
            gy = convolve_baseline(&Aframe[INDEX(i, j, W )], f2, 3, 3);

            mag = sqrt((gx * gx) + (gy * gy));

            Bframe[INDEX(i, j, W )] = (mag > threshold) ? 255 : mag;
        }
}
