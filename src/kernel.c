#include "kernel.h"
#include <xmmintrin.h>

//
i32 convolve_baseline(f32 *m, i32 *f, u64 fh, u64 fw) {
    i32 r = 0;

    for (u64 i = 0; i < fh; i++)
        for (u64 j = 0; j < fw; j++)
            r += m[INDEX(i, j, W) ] * f[INDEX(i, j, fw)];

    return r;
}

//
void sobel_baseline(f32 *Aframe, f32 *Bframe, f32 threshold) {
    i32 gx, gy;
    f32 mag = 0.0;

    i32 f1[9] = {-1,  0,  1, 
                 -2,  0,  2, 
                 -1,  0,  1}; // 3x3 matrix

    i32 f2[9] = {-1, -2, -1, 
                  0,  0,  0, 
                  1,  2,  1}; // 3x3 matrix

    //
    for (u64 i = 0; i < (H - 3); i++)
        for (u64 j = 0; j < (W  - 3); j++) {
            gx = convolve_baseline(&Aframe[INDEX(i, j, W )], f1, 3, 3);
            gy = convolve_baseline(&Aframe[INDEX(i, j, W )], f2, 3, 3);

            mag = sqrt((gx * gx) + (gy * gy));

            Bframe[INDEX(i, j, W )] = (mag > threshold) ? 255 : 0;
        }
}

void sobel_simd_avx2(f32 *Aframe, f32 *Bframe, f32 threshold) {
    __m256 _f1  = _mm256_set1_ps(1);
    __m256 _f2  = _mm256_set1_ps(2);
    __m256 _f3  = _mm256_set1_ps(3);
    __m256 _f4  = _mm256_set1_ps(4);
    __m256 _fm1 = _mm256_set1_ps(-1);
    __m256 _fm2 = _mm256_set1_ps(-2);

    __m256 _255 = _mm256_set1_ps(255);
    __m256 _threshold2 = _mm256_set1_ps(10000);

    for (u64 i = 0; i < (H - 3); i++){
        for (u64 j = 0; j < (W  - 3); j+=8) {
            // printf("%llu %llu\n", i, j);
            // printf("%f\n", Aframe[ INDEX(i+0, j+0, W) ]);
            __m256 _a1 = _mm256_loadu_ps( &Aframe[ INDEX(i+0, j+0, W) ] );
            __m256 _a2 = _mm256_loadu_ps( &Aframe[ INDEX(i+0, j+1, W) ] );
            __m256 _a3 = _mm256_loadu_ps( &Aframe[ INDEX(i+0, j+2, W) ] );
            __m256 _a4 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+0, W) ] );
            __m256 _a5 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+1, W) ] );
            __m256 _a6 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+2, W) ] );
            __m256 _a7 = _mm256_loadu_ps( &Aframe[ INDEX(i+2, j+0, W) ] );
            __m256 _a8 = _mm256_loadu_ps( &Aframe[ INDEX(i+2, j+1, W) ] );
            __m256 _a9 = _mm256_loadu_ps( &Aframe[ INDEX(i+2, j+2, W) ] );

            __m256 _b1 = _mm256_setzero_ps();
            __m256 _b2 = _mm256_setzero_ps();

            _b1 = 
                _mm256_add_ps( _mm256_mul_ps (_a1, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a3, _f1  ),
                _mm256_add_ps( _mm256_mul_ps (_a4, _fm2 ),
                _mm256_add_ps( _mm256_mul_ps (_a6, _f2  ),
                _mm256_add_ps( _mm256_mul_ps (_a7, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a9, _f1  ), _b1))))));

            _b2 = 
                _mm256_add_ps( _mm256_mul_ps (_a1, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a2, _fm2 ),
                _mm256_add_ps( _mm256_mul_ps (_a3, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a7, _f1  ),
                _mm256_add_ps( _mm256_mul_ps (_a8, _f2  ),
                _mm256_add_ps( _mm256_mul_ps (_a9, _f1  ), _b2))))));

            _b1 = _mm256_mul_ps( _b1, _b1 );
            _b2 = _mm256_mul_ps( _b2, _b2 );
            _b1 = _mm256_add_ps( _b2, _b1 );


                // _mm256_add_ps( _mm256_mull_ps (_a9, _a9  ), _a9);

            // _mm256_store_ps( &Aframe[ INDEX(i+1, j+1, W)], _b1 );
            // printf("1 %f\n", Aframe[ INDEX(i+0, j+0, W) ]);
            _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
    __m256 __ = _mm256_set1_ps(254);
            _b1 = _mm256_add_ps( _b1, __ );


            _mm256_store_ps( &Bframe[ INDEX(i, j, W)], _b1 );
            
            // printf("2 %f\n", Aframe[ INDEX(i+0, j+0, W) ]);

        }
    }
}

//
// avx512
// void sobel_simd_avx512(i16 *Aframe, i16 *Bframe, f32 threshold) {
//     i32 gx, gy;
//     f32 mag = 0.0;
//     
//     __m512i _f1 = _mm512_set1_epi16(1);
//     __m512i _f2 = _mm512_set1_epi16(2);
//     __m512i _f3 = _mm512_set1_epi16(3);
//     __m512i _f4 = _mm512_set1_epi16(4);
//     __m512i _fm1 = _mm512_set1_epi16(-1);
//     __m512i _fm2 = _mm512_set1_epi16(-2);
//     __m512i _255 = _mm512_set1_epi16(255);
//     
//     u16 thre = (u16) threshold;
//     __m512i _threshold = _mm512_set1_epi16( thre * thre );
//     _threshold = _mm512_sllv_epi16(_threshold, _f1); 
//     _threshold = _mm512_srlv_epi16(_threshold, _f4); 
//
//     // i32 f1[9] = {-1,  0,  1, 
//     //              -2,  0,  2, 
//     //              -1,  0,  1}; // 3x3 matrix
//     //
//     // i32 f2[9] = {-1, -2, -1, 
//     //               0,  0,  0, 
//     //               1,  2,  1}; // 3x3 matrix
//     //_mm256_cvtepu8_epi16
//
//     //
//     for (u64 i = 0; i < (H - 2); i++){
//         for (u64 j = 0; j < (W  - 2); j+=32) {
//             __m512i _a1 = _mm512_loadu_epi16( &Aframe[ INDEX(i+0, j+0, W) ] );
//             __m512i _a2 = _mm512_loadu_epi16( &Aframe[ INDEX(i+0, j+1, W) ] );
//             __m512i _a3 = _mm512_loadu_epi16( &Aframe[ INDEX(i+0, j+2, W) ] );
//             __m512i _a4 = _mm512_loadu_epi16( &Aframe[ INDEX(i+1, j+0, W) ] );
//             __m512i _a5 = _mm512_loadu_epi16( &Aframe[ INDEX(i+1, j+1, W) ] );
//             __m512i _a6 = _mm512_loadu_epi16( &Aframe[ INDEX(i+1, j+2, W) ] );
//             __m512i _a7 = _mm512_loadu_epi16( &Aframe[ INDEX(i+2, j+0, W) ] );
//             __m512i _a8 = _mm512_loadu_epi16( &Aframe[ INDEX(i+2, j+1, W) ] );
//             __m512i _a9 = _mm512_loadu_epi16( &Aframe[ INDEX(i+2, j+2, W) ] );
//
//             __m512i _b1 = _mm512_setzero_si512();
//             __m512i _b2 = _mm512_setzero_si512();
//             
//             _b1 = 
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a1, _fm1 ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a3, _f1  ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a4, _fm2 ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a6, _f2  ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a7, _fm1 ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a9, _f1  ), _b1))))));
//             
//             _b2 = 
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a1, _fm1 ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a2, _fm2 ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a3, _fm1 ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a7, _f1  ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a8, _f2  ),
//                 _mm512_add_epi16( _mm512_mullo_epi16 (_a9, _f1  ), _b2))))));
//             
//             // max : 4 * 255
//             
//             // 
//             _b1 = _mm512_sllv_epi16(_b1, _f1); 
//             _b1 = _mm512_srlv_epi16(_b1, _f4); // not sure lol 
//             _b2 = _mm512_sllv_epi16(_b1, _f1); 
//             _b2 = _mm512_srlv_epi16(_b1, _f4); // not sure lol 
//             
//             _b1 = _mm512_add_epi16( _mm512_mullo_epi16 (_b1, _b1 ),
//                                     _mm512_mullo_epi16 (_b2, _b2  ));
//
//             __mmask32 mask = _mm512_cmp_epi16_mask(
//                     _threshold, _b1, _MM_CMPINT_LE);
//
//             // _b1 = _mm512_and_epi8( _b1, _mm512_castsi512_epi16( mask ) );
//
//             // _b1 = _mm512_maskz_expand_epi16(mask, _255);
//
//             _mm512_storeu_epi16 ( &Aframe[ INDEX(i+1, j+1, W)], _b1);
//             // _b1_low = _b1 * _b1
//             // _b1_hight = _b1 * _b1
//             // 
//             // _b2_low = _b2 * _b2
//             // _b2_hight = _b2 * _b2
//
//
//             // _b1 = _b1 + _a1 * _f1;
//
//             // mag = sqrt((gx * gx) + (gy * gy));
//
//             // Bframe[INDEX(i, j, W )] = (mag > threshold) ? 255 : mag;
//         }
//     }
// }

