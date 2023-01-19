#include <math.h>

#include "common.h"

//
void sort(f64 *a, u64 n) {
    for (u64 i = 0; i < n; i++)
        for (u64 j = i + 1; j < n; j++)
            if (a[i] > a[j]) {
                u64 tmp = a[i];

                a[i] = a[j];
                a[j] = tmp;
            }
}

// Arithmetic mean
f64 mean(f64 *a, u64 n) {
    f64 m = 0.0;

    for (u64 i = 0; i < n; i++)
        m += (f64)a[i];

    m /= (f64)n;

    return m;
}

// Standard deviation
f64 stddev(f64 *a, u64 n) {
    f64 d = 0.0;
    f64 m = mean(a, n);

    //
    for (u64 i = 0; i < n; i++)
        d += (a[i] - m) * (a[i] - m);

    d /= (f64)(n - 1);

    return sqrt(d);
}


// Convert an image to its grayscale equivalent - better color precision
void grayscale_weighted(u8 *bigFrame, f32 *smallFrame) {
    f32 gray;

    for (u64 i = 0; i < H * W; i ++) {
        // Convert RGB color values into grayscale for each pixel using color
        // weights Other possible weights: 0.59, 0.30, 0.11 Principle: mix
        // different quantities of R, G, B to create a variant of gray
        gray = ((float)bigFrame[i * 3] * 0.299) + 
               ((float)bigFrame[i * 3 + 1] * 0.587) +
               ((float)bigFrame[i * 3 + 2] * 0.114);

        smallFrame[i] = gray;
    }
}

// Convert an image to its grayscale equivalent - bad color precision
void grayscale_sampled(u8 *bigFrame, f32 *smallFrame) {
    for (u64 i = 0; i < H * W; i++) {
        // R: light gray
        // G: medium gray
        // B: dark gray
        f32 gray = bigFrame[i*3];

        smallFrame[i] = gray;
    }
}

// covert w * h gray image into w * h * 3 rgb 
void Biggerize(f32 *smallFrame, u8 *bigFrame) {
    for (u64 i = 0; i < H * W; i++) {
        u8 gray = (u8) smallFrame[i];

        bigFrame[i*3    ] = gray;
        bigFrame[i*3 + 1] = gray;
        bigFrame[i*3 + 2] = gray;
    }
}


void Ccleaner( u8 * tmp ){
    for( u64 i = 0; i < 1024 * 1024 * 1024; i++){
        tmp[i]++;
    }
}
