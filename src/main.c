/*
  This code performs edge detection using a Sobel filter on a video stream meant
  as input to a neural network
*/

#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//
#include "common.h"
#include "kernel.h"




//
int main(int argc, char **argv) {
    //
    if (argc < 3)
        return printf("Usage: %s [raw input file] [raw output file]\n",
                      argv[0]),
               1;

    // Size of a frame
    u64 size = sizeof(u8) * H * W * 3;

    //
    f64 elapsed_s = 0.0;
    f64 elapsed_ns = 0.0;
    f64 mib_per_s = 0.0;
    struct timespec t1, t2;
    f64 samples[MAX_SAMPLES];
    //
    u64 nb_bytes = 1, frame_count = 0, samples_count = 0;

    //
    u8 *cframe = _mm_malloc(size, 32);
    u8 *oframe = _mm_malloc(size, 32);

    //
    FILE *fpi = fopen(argv[1], "rb");
    FILE *fpo = fopen(argv[2], "wb");

    //
    if (!fpi)
        return printf("Error: cannot open file '%s'\n", argv[1]), 2;

    //
    if (!fpo)
        return printf("Error: cannot open file '%s'\n", argv[2]), 2;

    // Read & process video frames
    while ((nb_bytes = fread(cframe, sizeof(u8), H * W * 3, fpi))) {
        //
        grayscale_weighted(cframe);

        do {

            // Start
            clock_gettime(CLOCK_MONOTONIC_RAW, &t1);

            // Put other versions here

#if BASELINE
            sobel_baseline(cframe, oframe, 100.0);
#endif
            // Stop
            clock_gettime(CLOCK_MONOTONIC_RAW, &t2);

            // Nano seconds
            elapsed_ns = (f64)(t2.tv_nsec - t1.tv_nsec);

        } while (elapsed_ns <= 0.0);

        // Seconds
        elapsed_s = elapsed_ns / 1e9;

        // 2 arrays
        mib_per_s = ((f64)(nb_bytes << 1) / (1024.0 * 1024.0)) / elapsed_s;

        //
        if (samples_count < MAX_SAMPLES)
            samples[samples_count++] = elapsed_ns;

        // frame number; size in Bytes; elapsed ns; elapsed s; bytes per second
        fprintf(stdout, "%20llu; %20llu bytes; %15.3lf ns; %15.3lf MiB/s\n",
                frame_count, nb_bytes << 1, elapsed_ns, mib_per_s);

        // Write this frame to the output pipe
        fwrite(oframe, sizeof(u8), H * W * 3, fpo);

        //
        frame_count++;
    }

    //
    sort(samples, samples_count);

    //
    f64 min, max, avg, mea, dev;

    //
    mea = mean(samples, samples_count);

    //
    dev = stddev(samples, samples_count);

    //
    min = samples[0];
    max = samples[samples_count - 1];

    elapsed_s = mea / 1e9;

    // 2 arrays (input & output)
    mib_per_s = ((f64)(size << 1) / (1024.0 * 1024.0)) / elapsed_s;

    //
    fprintf(stderr,
            "\n%20llu bytes; %15.3lf ns; %15.3lf ns; %15.3lf ns; %15.3lf "
            "MiB/s; %15.3lf %%;\n",
            (sizeof(u8) * H * W * 3) << 1, min, max, mea, mib_per_s,
            (dev * 100.0 / mea));

    //
    _mm_free(cframe);
    _mm_free(oframe);

    //
    fclose(fpi);
    fclose(fpo);

    return 0;
}

