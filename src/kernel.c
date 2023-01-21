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
void sobel_baseline(f32 * restrict Aframe, f32 * restrict Bframe, f32 threshold) {
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

            mag = (gx * gx) + (gy * gy);

            Bframe[INDEX(i, j, W )] = (mag > 10000) ? 255 : 0;
        }
}

void sobel_opti_v0(f32 * restrict  Aframe, f32 * restrict Bframe, f32 threshold) {
    f32 gx, gy;
    i32 mag = 0.0;
    i32 threshold2 = 100000;
    
    for (u64 i = 0; i < (H - 3); i++)
        for (u64 j = 0; j < (W  - 3); j++) {
            
            // f1
            gx = 0;
            gy = 0;
            
            u64 idx;
            
            idx = INDEX(i, j, W );

            gx += Aframe[idx + IDX_1] * (-1); //same  f2 optimize ! 
            gx += Aframe[idx + IDX_3] * ( 1);
            gx += Aframe[idx + IDX_4] * (-2);
            gx += Aframe[idx + IDX_6] * ( 2);
            gx += Aframe[idx + IDX_7] * (-1);
            gx += Aframe[idx + IDX_9] * ( 1); // same f2

            // f2
            gy += Aframe[idx + IDX_1] * (-1);
            gy += Aframe[idx + IDX_2] * (-2);
            gy += Aframe[idx + IDX_3] * (-1);
            gy += Aframe[idx + IDX_7] * ( 1);
            gy += Aframe[idx + IDX_8] * ( 2);
            gy += Aframe[idx + IDX_9] * ( 1);


           mag = gx * gx + gy * gy;
           
            Bframe[  INDEX(i+1,j+1,W)  ] = (mag > 10000) ? 255 : 0;
    }
}

void sobel_opti_v2(f32 * restrict  Aframe, f32 * restrict Bframe, f32 threshold) {
    f32 gx, gy;

    i32 mag = 0.0;
    i32 threshold2 = 100000;
    
    for (u64 i = 0; i < (H - 3); i+=2)
        for (u64 j = 0; j < (W  - 3); j++) {
            
            // f1
            u64 idx;
            idx = INDEX(i, j, W );

            f32 frame1 = Aframe[idx + IDX_1];
            f32 frame2 = Aframe[idx + IDX_2];
            f32 frame3 = Aframe[idx + IDX_3];

            f32 frame4 = Aframe[idx + IDX_4];
            f32 frame5 = Aframe[idx + IDX_5];
            f32 frame6 = Aframe[idx + IDX_6];

            f32 frame7 = Aframe[idx + IDX_7];
            f32 frame8 = Aframe[idx + IDX_8];
            f32 frame9 = Aframe[idx + IDX_9];

            f32 frame10 = Aframe[idx + IDX_10];
            f32 frame11 = Aframe[idx + IDX_11];
            f32 frame12 = Aframe[idx + IDX_12];

            // f32 frame13 = Aframe[idx + IDX_10];
            // f32 frame14 = Aframe[idx + IDX_11];
            // f32 frame15 = Aframe[idx + IDX_12];
            //
            //
            // f32 frame16 = Aframe[idx + IDX_10];
            // f32 frame17 = Aframe[idx + IDX_11];
            // f32 frame18 = Aframe[idx + IDX_12];
            //
            //
            // f32 frame19 = Aframe[idx + IDX_10];
            // f32 frame20 = Aframe[idx + IDX_11];
            // f32 frame21 = Aframe[idx + IDX_12];


            gx = 0;
            gy = 0;
            
            gx += frame1 * (-1); 
            gx += frame3 * ( 1);
            gx += frame4 * (-2);
            gx += frame6 * ( 2);
            gx += frame7 * (-1);
            gx += frame9 * ( 1); 

            // f2
            gy += frame1 * (-1);
            gy += frame2 * (-2);
            gy += frame3 * (-1);
            gy += frame7 * ( 1);
            gy += frame8 * ( 2);
            gy += frame9 * ( 1);

            mag = gx * gx + gy * gy;
            Bframe[  idx + IDX_4  ] = (mag > 10000) ? 255 : 0;
           

            gx = 0;
            gy = 0;

            gx += frame4 * (-1); 
            gx += frame6 * ( 1);
            gx += frame7 * (-2);
            gx += frame9 * ( 2);
            gx += frame10 * (-1);
            gx += frame12 * ( 1); 

            // f2
            gy += frame4 * (-1);
            gy += frame5 * (-2);
            gy += frame6 * (-1);
            gy += frame10 * ( 1);
            gy += frame11 * ( 2);
            gy += frame12 * ( 1);

            mag = gx * gx + gy * gy;
            Bframe[  idx + IDX_7  ] = (mag > 10000) ? 255 : 0;


            // gx += frame4 * (-1); 
            // gx += frame6 * ( 1);
            // gx += frame7 * (-2);
            // gx += frame9 * ( 2);
            // gx += frame10 * (-1);
            // gx += frame12 * ( 1); 
            //
            // // f2
            // gy += frame4 * (-1);
            // gy += frame5 * (-2);
            // gy += frame6 * (-1);
            // gy += frame10 * ( 1);
            // gy += frame11 * ( 2);
            // gy += frame12 * ( 1);
            //
            // mag = gx * gx + gy * gy;
            // Bframe[  idx + IDX_4  ] = (mag > 10000) ? 255 : 0;
            //


        }
    
}

void sobel_simd_avx2v3(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold) {
    // __m256 _fm2  = _mm256_set1_ps(-2);
    // __m256 _f2  = _mm256_set1_ps(2);


    __m256 _255 = _mm256_set1_ps(254);
    __m256 _threshold2 = _mm256_set1_ps(10000);

    for (u64 i = 0; i < (H - 3); i+=3){
        for (u64 j = 0; j < (W  - 3); j+=8) {
            
            u64 idx = INDEX(i, j, W );



            // column 1
            __m256 _a1 = _mm256_loadu_ps( &Aframe[ idx + IDX_1 ] );
            __m256 _a2 = _mm256_loadu_ps( &Aframe[ idx + IDX_4 ] );
            __m256 _a3 = _mm256_loadu_ps( &Aframe[ idx + IDX_7 ] );
            __m256 _a4 = _mm256_loadu_ps( &Aframe[ idx + IDX_10 ] );
            __m256 _a5 = _mm256_loadu_ps( &Aframe[ idx + IDX_13 ] );
            // __m256 _a6 = _mm256_loadu_ps( &Aframe[ idx + IDX_16 ] );
            
            // line 1
            __m256 _bx1 = _mm256_setzero_ps();
            __m256 _by1 = _mm256_setzero_ps();

            _bx1 = _mm256_sub_ps( _bx1, _a1 ); // bx1 - a1
            _bx1 = _mm256_sub_ps( _bx1, _a2 ); // bx1 - 2 * a2
            _bx1 = _mm256_sub_ps( _bx1, _a2 );
            _bx1 = _mm256_sub_ps( _bx1, _a3 ); //bx1 - a3

            _by1 = _mm256_sub_ps( _by1, _a1 ); 
            _by1 = _mm256_add_ps( _by1, _a3 ); 
            
            // line 2
            __m256 _bx2 = _mm256_setzero_ps();
            __m256 _by2 = _mm256_setzero_ps();

            _bx2 = _mm256_sub_ps( _bx2, _a2 ); 
            _bx2 = _mm256_sub_ps( _bx2, _a3 ); 
            _bx2 = _mm256_sub_ps( _bx2, _a3 );
            _bx2 = _mm256_sub_ps( _bx2, _a4 ); 
        
            _by2 = _mm256_sub_ps( _by2, _a2 ); 
            _by2 = _mm256_add_ps( _by2, _a4 ); 
           

            // line 3
            __m256 _bx3 = _mm256_setzero_ps();
            __m256 _by3 = _mm256_setzero_ps();

            _bx3 = _mm256_sub_ps( _bx3, _a3 ); 
            _bx3 = _mm256_sub_ps( _bx3, _a4 ); 
            _bx3 = _mm256_sub_ps( _bx3, _a4 );
            _bx3 = _mm256_sub_ps( _bx3, _a5 ); 

            _by3 = _mm256_sub_ps( _by3, _a3 ); 
            _by3 = _mm256_add_ps( _by3, _a5 ); 
            
            //



            // column 2
            _a1 = _mm256_loadu_ps( &Aframe[ idx + IDX_2 ] );
            _a2 = _mm256_loadu_ps( &Aframe[ idx + IDX_5 ] );
            _a3 = _mm256_loadu_ps( &Aframe[ idx + IDX_8 ] );
            _a4 = _mm256_loadu_ps( &Aframe[ idx + IDX_11 ] );
            _a5 = _mm256_loadu_ps( &Aframe[ idx + IDX_14 ] );
            // __m256 _a6 = _mm256_loadu_ps( &Aframe[ idx + IDX_16 ] );
            
            // line 1 
            _by1 = _mm256_sub_ps( _by1, _a1 ); 
            _by1 = _mm256_sub_ps( _by1, _a1 ); 
            _by1 = _mm256_add_ps( _by1, _a3 ); 
            _by1 = _mm256_add_ps( _by1, _a3 ); 
            
            // line 2
            _by2 = _mm256_sub_ps( _by2, _a2 ); 
            _by2 = _mm256_sub_ps( _by2, _a2 ); 
            _by2 = _mm256_add_ps( _by2, _a4 ); 
            _by2 = _mm256_add_ps( _by2, _a4 ); 
            
            // line 3
            _by3 = _mm256_sub_ps( _by3, _a3 ); 
            _by3 = _mm256_sub_ps( _by3, _a3 ); 
            _by3 = _mm256_add_ps( _by3, _a5 ); 
            _by3 = _mm256_add_ps( _by3, _a5 ); 




            // i32 f1[9] = {-1,  0,  1, 
            //              -2,  0,  2, 
            //              -1,  0,  1}; // 3x3 matrix
            //
            // i32 f2[9] = {-1, -2, -1, 
            //               0,  0,  0, 
            //               1,  2,  1}; // 3x3 matrix
            //



            // column 3
            _a1 = _mm256_loadu_ps( &Aframe[ idx + IDX_3 ] );
            _a2 = _mm256_loadu_ps( &Aframe[ idx + IDX_6 ] );
            _a3 = _mm256_loadu_ps( &Aframe[ idx + IDX_9 ] );
            _a4 = _mm256_loadu_ps( &Aframe[ idx + IDX_12 ] );
            _a5 = _mm256_loadu_ps( &Aframe[ idx + IDX_15 ] );
            // __m256 _a6 = _mm256_loadu_ps( &Aframe[ idx + IDX_16 ] );
            
            // line 1

            _bx1 = _mm256_add_ps( _bx1, _a1 ); // bx1 + a1
            _bx1 = _mm256_add_ps( _bx1, _a2 ); // bx1 + 2 * a2
            _bx1 = _mm256_add_ps( _bx1, _a2 );
            _bx1 = _mm256_add_ps( _bx1, _a3 ); //bx1 + a3

            _by1 = _mm256_sub_ps( _by1, _a1 ); 
            _by1 = _mm256_add_ps( _by1, _a3 ); 
            
            // line 2

            _bx2 = _mm256_add_ps( _bx2, _a2 ); 
            _bx2 = _mm256_add_ps( _bx2, _a3 ); 
            _bx2 = _mm256_add_ps( _bx2, _a3 );
            _bx2 = _mm256_add_ps( _bx2, _a4 ); 
        
            _by2 = _mm256_sub_ps( _by2, _a2 ); 
            _by2 = _mm256_add_ps( _by2, _a4 ); 
           

            // line 3

            _bx3 = _mm256_add_ps( _bx3, _a3 ); 
            _bx3 = _mm256_add_ps( _bx3, _a4 ); 
            _bx3 = _mm256_add_ps( _bx3, _a4 );
            _bx3 = _mm256_add_ps( _bx3, _a5 ); 

            _by3 = _mm256_sub_ps( _by3, _a3 ); 
            _by3 = _mm256_add_ps( _by3, _a5 ); 
 
            



            // finish and store 
            
            // line 1
            _bx1 = _mm256_mul_ps( _bx1, _bx1 );
            _by1 = _mm256_mul_ps( _by1, _by1 );
            _bx1 = _mm256_add_ps( _bx1, _by1 );
            //
            _bx1 = _mm256_cmp_ps( _threshold2, _bx1, _MM_CMPINT_LE);
            _bx1 = _mm256_add_ps( _bx1, _255 );

            _mm256_store_ps( &Bframe[ INDEX(i+0, j, W)], _bx1 );
           

            // line 2
            _bx2 = _mm256_mul_ps( _bx2, _bx2 );
            _by2 = _mm256_mul_ps( _by2, _by2 );
            _bx2 = _mm256_add_ps( _bx2, _by2 );

            _bx2 = _mm256_cmp_ps( _threshold2, _bx2, _MM_CMPINT_LE);
            _bx2 = _mm256_add_ps( _bx2, _255 );

            _mm256_store_ps( &Bframe[ INDEX(i+1, j, W)], _bx2 );
           

            // line 3
            _bx3 = _mm256_mul_ps( _bx3, _bx3 );
            _by3 = _mm256_mul_ps( _by3, _by3 );
            _bx3 = _mm256_add_ps( _bx3, _by3 );

            _bx3 = _mm256_cmp_ps( _threshold2, _bx3, _MM_CMPINT_LE);
            _bx3 = _mm256_add_ps( _bx3, _255 );

            _mm256_store_ps( &Bframe[ INDEX(i+2, j, W)], _bx3 );
            




            // _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
            // _b1 = _mm256_add_ps( _b1, _255 );
            // _mm256_store_ps( &Bframe[ INDEX(i, j, W)], _b1 );
            
                    }
    }
}

void sobel_opti_v1(f32 * restrict  Aframe, f32 * restrict Bframe, f32 threshold) {
    f32 gx, gy;
    i32 mag = 0.0;
    i32 threshold2 = 100000;
    
    for (u64 i = 0; i < (H - 3); i++)
        for (u64 j = 0; j < (W  - 3); j++) {
            
            // f1
            u64 idx;
            idx = INDEX(i, j, W );

            f32 frame1 = Aframe[idx + IDX_1];
            f32 frame2 = Aframe[idx + IDX_2];
            f32 frame3 = Aframe[idx + IDX_3];

            f32 frame4 = Aframe[idx + IDX_4];
            f32 frame5 = Aframe[idx + IDX_5];
            f32 frame6 = Aframe[idx + IDX_6];

            f32 frame7 = Aframe[idx + IDX_7];
            f32 frame8 = Aframe[idx + IDX_8];
            f32 frame9 = Aframe[idx + IDX_9];

            // f32 frame10 = Aframe[idx + IDX_10];
            // f32 frame11 = Aframe[idx + IDX_11];
            // f32 frame12 = Aframe[idx + IDX_12];

            gx = 0;
            gy = 0;
            
            gx += frame1 * (-1); //same  f2 optimize ! 
            gx += frame3 * ( 1);
            gx += frame4 * (-2);
            gx += frame6 * ( 2);
            gx += frame7 * (-1);
            gx += frame9 * ( 1); // same f2

            // f2
            gy += frame1 * (-1);
            gy += frame2 * (-2);
            gy += frame3 * (-1);
            gy += frame7 * ( 1);
            gy += frame8 * ( 2);
            gy += frame9 * ( 1);


            mag = gx * gx + gy * gy;
           
            Bframe[  idx + IDX_4  ] = (mag > 10000) ? 255 : 0;
        }
    
}

void sobel_simd_avx2(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold) {
    __m256 _f1  = _mm256_set1_ps(1);
    __m256 _f2  = _mm256_set1_ps(2);
    __m256 _f3  = _mm256_set1_ps(3);
    __m256 _f4  = _mm256_set1_ps(4);
    __m256 _fm1 = _mm256_set1_ps(-1);
    __m256 _fm2 = _mm256_set1_ps(-2);

    __m256 _255 = _mm256_set1_ps(255);
    __m256 _threshold2 = _mm256_set1_ps(10000);

    for (u64 i = 0; i < (H - 3); i+=1){
        for (u64 j = 0; j < (W  - 3); j+=8) {
            // printf("%llu %llu\n", i, j);
            // printf("%f\n", Aframe[ INDEX(i+0, j+0, W) ]);
            
            __m256 _b1 = _mm256_setzero_ps();
            __m256 _b2 = _mm256_setzero_ps();

            __m256 _a1 = _mm256_loadu_ps( &Aframe[ INDEX(i+0, j+0, W) ] );
            _b1 = _mm256_fmadd_ps(  _a1, _fm1 , _b1);
            _b2 = _mm256_fmadd_ps(  _a1, _fm1 , _b2);

            __m256 _a2 = _mm256_loadu_ps( &Aframe[ INDEX(i+0, j+1, W) ] );
            _b2 = _mm256_fmadd_ps(  _a2, _fm2 , _b2);

            __m256 _a3 = _mm256_loadu_ps( &Aframe[ INDEX(i+0, j+2, W) ] );
            _b1 = _mm256_fmadd_ps(  _a3, _f1  , _b1);
            _b2 = _mm256_fmadd_ps(  _a3, _fm1 , _b2);

            __m256 _a4 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+0, W) ] );
            _b1 = _mm256_fmadd_ps(  _a4, _fm2 , _b1);

            // __m256 _a5 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+1, W) ] );
            __m256 _a6 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+2, W) ] );
            _b1 = _mm256_fmadd_ps(  _a6, _f2  , _b1);

            __m256 _a7 = _mm256_loadu_ps( &Aframe[ INDEX(i+2, j+0, W) ] );
            _b1 = _mm256_fmadd_ps(  _a7, _fm1 , _b1);
            _b2 = _mm256_fmadd_ps(  _a7, _f1  , _b2);

            __m256 _a8 = _mm256_loadu_ps( &Aframe[ INDEX(i+2, j+1, W) ] );
            _b2 = _mm256_fmadd_ps(  _a8, _f2  , _b2);

            __m256 _a9 = _mm256_loadu_ps( &Aframe[ INDEX(i+2, j+2, W) ] );
            _b1 = _mm256_fmadd_ps(  _a9, _f1  , _b1);
            _b2 = _mm256_fmadd_ps(  _a9, _f1  , _b2);

                        //fmadd version

            //

            // _b1 = 
            //     _mm256_add_ps( _mm256_mul_ps (_a1, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a3, _f1  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a4, _fm2 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a6, _f2  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a7, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a9, _f1  ), _b1))))));
            //
            // _b2 = 
            //     _mm256_add_ps( _mm256_mul_ps (_a1, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a2, _fm2 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a3, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a7, _f1  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a8, _f2  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a9, _f1  ), _b2))))));

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

void sobel_simd_avx2v2(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold) {
    __m256 _f1  = _mm256_set1_ps(1);
    __m256 _f2  = _mm256_set1_ps(2);
    __m256 _f3  = _mm256_set1_ps(3);
    __m256 _f4  = _mm256_set1_ps(4);
    __m256 _fm1 = _mm256_set1_ps(-1);
    __m256 _fm2 = _mm256_set1_ps(-2);

    __m256 _255 = _mm256_set1_ps(255);
    __m256 _threshold2 = _mm256_set1_ps(10000);

    for (u64 i = 0; i < (H - 3); i+=2){
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
            __m256 _a10 = _mm256_loadu_ps( &Aframe[ INDEX(i+3, j+0, W) ] );
            __m256 _a11 = _mm256_loadu_ps( &Aframe[ INDEX(i+3, j+1, W) ] );
            __m256 _a12 = _mm256_loadu_ps( &Aframe[ INDEX(i+3, j+2, W) ] );


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


            _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
            _b1 = _mm256_add_ps( _b1, _255 );
            _mm256_store_ps( &Bframe[ INDEX(i, j, W)], _b1 );
            
            _b1 = _mm256_setzero_ps();
            _b2 = _mm256_setzero_ps();

            _b1 = 
                _mm256_add_ps( _mm256_mul_ps (_a4, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a6, _f1  ),
                _mm256_add_ps( _mm256_mul_ps (_a7, _fm2 ),
                _mm256_add_ps( _mm256_mul_ps (_a9, _f2  ),
                _mm256_add_ps( _mm256_mul_ps (_a10, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a12, _f1  ), _b1))))));

            _b2 = 
                _mm256_add_ps( _mm256_mul_ps (_a4, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a5, _fm2 ),
                _mm256_add_ps( _mm256_mul_ps (_a6, _fm1 ),
                _mm256_add_ps( _mm256_mul_ps (_a10, _f1  ),
                _mm256_add_ps( _mm256_mul_ps (_a11, _f2  ),
                _mm256_add_ps( _mm256_mul_ps (_a12, _f1  ), _b2))))));

            _b1 = _mm256_mul_ps( _b1, _b1 );
            _b2 = _mm256_mul_ps( _b2, _b2 );
            _b1 = _mm256_add_ps( _b2, _b1 );


            _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
            _b1 = _mm256_add_ps( _b1, _255 );
            _mm256_store_ps( &Bframe[ INDEX(i+1, j, W)], _b1 );
            

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


#define TMP_W 32 * 8
#define TMP_H 48


// void sobel_CacheBlock(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 * tmpx, f32 * tmpy) {
void sobel_CL(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 * tmpx, f32 * tmpy) {

    f32 gx, gy;
    i32 mag = 0.0;
    i32 threshold2 = 100000;
    
    for (u64 ii = 0; ii < 15; ii++){
        // printf("ii %llu\n", ii);

        for (u64 jj = 0; jj < 5; jj++) {
            // printf("jj %llu\n", jj);

            for( u64 i = 0; i < 48 - 2 ; i++){
                // printf("i %llu\n", i);
                for( u64 j = 0; j < 32 * 8 - 2 ; j++){
                    u64 idx = INDEX(ii * (48-2) + i, jj  * (32 * 8 - 2) + j, W );
                    
                    f32 frame1 = Aframe[idx + IDX_1];
                    f32 frame2 = Aframe[idx + IDX_2];
                    f32 frame3 = Aframe[idx + IDX_3];

                    f32 frame4 = Aframe[idx + IDX_4];
                    f32 frame5 = Aframe[idx + IDX_5];
                    f32 frame6 = Aframe[idx + IDX_6];

                    f32 frame7 = Aframe[idx + IDX_7];
                    f32 frame8 = Aframe[idx + IDX_8];
                    f32 frame9 = Aframe[idx + IDX_9];


                    gx = 0;
                    gy = 0;
                    
                    gx += frame1 * (-1); //same  f2 optimize ! 
                    gx += frame3 * ( 1);
                    gx += frame4 * (-2);
                    gx += frame6 * ( 2);
                    gx += frame7 * (-1);
                    gx += frame9 * ( 1); // same f2

                    // f2
                    gy += frame1 * (-1);
                    gy += frame2 * (-2);
                    gy += frame3 * (-1);
                    gy += frame7 * ( 1);
                    gy += frame8 * ( 2);
                    gy += frame9 * ( 1);


                    mag = gx * gx + gy * gy;
           
                    Bframe[  idx + IDX_4  ] = (mag > 10000) ? 255 : 0;


                }

            }
        }
    }
    
}



void sobel_CL_AVX(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 * tmpx, f32 * tmpy) {

    // f32 gx, gy;
    // i32 mag = 0.0;
    // i32 threshold2 = 100000;

    __m256 _f1  = _mm256_set1_ps(1);
    __m256 _f2  = _mm256_set1_ps(2);
    __m256 _f3  = _mm256_set1_ps(3);
    __m256 _f4  = _mm256_set1_ps(4);
    __m256 _fm1 = _mm256_set1_ps(-1);
    __m256 _fm2 = _mm256_set1_ps(-2);

    __m256 _255 = _mm256_set1_ps(255);
    __m256 _threshold2 = _mm256_set1_ps(10000);


    
    for (u64 ii = 0; ii < 15; ii++){
        // printf("ii %llu\n", ii);

        for (u64 jj = 0; jj < 5; jj++) {
            // printf("jj %llu\n", jj);

            for( u64 i = 0; i < 48 - 2 ; i++){
                // printf("i %llu\n", i);
                for( u64 j = 0; j < 32 * 8 - 2 ; j+=8){

                    u64 idx = INDEX(ii * (48-2) + i, jj  * (32 * 8 - 2) + j, W );
                    // printf("idx %llu\n", idx);
                    
            
                    __m256 _b1 = _mm256_setzero_ps();
                    __m256 _b2 = _mm256_setzero_ps();

                    __m256 _a1 = _mm256_loadu_ps( &Aframe[ idx + IDX_1 ] );
                    _b1 = _mm256_fmadd_ps(  _a1, _fm1 , _b1);
                    _b2 = _mm256_fmadd_ps(  _a1, _fm1 , _b2);

                    __m256 _a2 = _mm256_loadu_ps( &Aframe[ idx + IDX_2 ] );
                    _b2 = _mm256_fmadd_ps(  _a2, _fm2 , _b2);

                    __m256 _a3 = _mm256_loadu_ps( &Aframe[ idx + IDX_3 ] );
                    _b1 = _mm256_fmadd_ps(  _a3, _f1  , _b1);
                    _b2 = _mm256_fmadd_ps(  _a3, _fm1 , _b2);

                    __m256 _a4 = _mm256_loadu_ps( &Aframe[ idx + IDX_4 ] );
                    _b1 = _mm256_fmadd_ps(  _a4, _fm2 , _b1);

                    // __m256 _a5 = _mm256_loadu_ps( &Aframe[ INDEX(i+1, j+1, W) ] );
                    __m256 _a6 = _mm256_loadu_ps( &Aframe[ idx + IDX_6 ] );
                    _b1 = _mm256_fmadd_ps(  _a6, _f2  , _b1);

                    __m256 _a7 = _mm256_loadu_ps( &Aframe[ idx + IDX_7 ] );
                    _b1 = _mm256_fmadd_ps(  _a7, _fm1 , _b1);
                    _b2 = _mm256_fmadd_ps(  _a7, _f1  , _b2);

                    __m256 _a8 = _mm256_loadu_ps( &Aframe[ idx + IDX_8 ] );
                    _b2 = _mm256_fmadd_ps(  _a8, _f2  , _b2);

                    __m256 _a9 = _mm256_loadu_ps( &Aframe[ idx + IDX_9 ] );
                    _b1 = _mm256_fmadd_ps(  _a9, _f1  , _b1);
                    _b2 = _mm256_fmadd_ps(  _a9, _f1  , _b2);

                                //fmadd version

                    //
                    _b1 = _mm256_mul_ps( _b1, _b1 );
                    _b2 = _mm256_mul_ps( _b2, _b2 );
                    _b1 = _mm256_add_ps( _b2, _b1 );


                    _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
                    __m256 __ = _mm256_set1_ps(254);
                    _b1 = _mm256_add_ps( _b1, __ );


                    _mm256_storeu_ps( &Bframe[ idx + IDX_4], _b1 );
                    

                }

            }
        }
    }
    
}



void sobel_1D(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold, f32 *restrict bufferx, f32 *restrict buffery) {
    f32 gx, gy;
    i32 mag = 0.0;
    i32 threshold2 = 100000;
    
    for (u64 ii = 0; ii < 15; ii++){
        // printf("ii %llu\n", ii);

        for (u64 jj = 0; jj < 5; jj++) {
            // printf("jj %llu\n", jj);

            for( u64 i = 0; i < 48 - 2 ; i++){
                // printf("i %llu\n", i);
                for( u64 j = 0; j < 32 * 8 - 2 ; j++){

                    u64 idx = INDEX(ii * (48-2) + i, jj  * (32 * 8 - 2) + j, W );
                    u64 buff_idx = INDEX(i, j, 32*8 );
                    
                    f32 frame1 = Aframe[idx + IDX_1];
                    f32 frame2 = Aframe[idx + IDX_2];
                    f32 frame3 = Aframe[idx + IDX_3];

                    // f32 frame4 = Aframe[idx + IDX_4];
                    // f32 frame5 = Aframe[idx + IDX_5];
                    // f32 frame6 = Aframe[idx + IDX_6];
                    //
                    // f32 frame7 = Aframe[idx + IDX_7];
                    // f32 frame8 = Aframe[idx + IDX_8];
                    // f32 frame9 = Aframe[idx + IDX_9];


                    gx = 0;
                    gy = 0;
                    
                    gx += frame1 * ( 1); 
                    gx += frame2 * ( 2);
                    gx += frame3 * ( 1);

                    gy += frame1 * (-1);
                    gy += frame2 * ( 0);
                    gy += frame3 * ( 1);


                    bufferx[  buff_idx   ] = gx;
                    buffery[  buff_idx   ] = gy;

                }
            }


           for( u64 i = 0; i < 48 - 2 ; i++){
                // printf("i %llu\n", i);
                for( u64 j = 0; j < 32 * 8 - 2 ; j++){

                    u64 idx = INDEX(ii * (48-2) + i, jj  * (32 * 8 - 2) + j, W );
                    u64 buff_idx = INDEX(i, j, 32*8 );
                    
                    f32 frame1x = bufferx[buff_idx + 0];
                    f32 frame2x = bufferx[buff_idx + 32*8];
                    f32 frame3x = bufferx[buff_idx + 32*8*2];
 
                    f32 frame1y = buffery[buff_idx + 0];
                    f32 frame2y = buffery[buff_idx + 32*8];
                    f32 frame3y = buffery[buff_idx + 32*8*2];


                    gx = 0;
                    gy = 0;
                    
                    gy += frame1y * ( 1); 
                    gy += frame2y * ( 2);
                    gy += frame3y * ( 1);

                    gx += frame1x * (-1);
                    gx += frame2x * ( 0);
                    gx += frame3x * ( 1);

                    mag = gx * gx + gy * gy;

                    Bframe[  idx + IDX_4  ] = (mag > 10000) ? 255 : 0;

                }
            }



           



        }
    }
}


void sobel_simd_avx2v4(f32 *restrict Aframe, f32 *restrict Bframe, f32 threshold) {
    __m256 _f1  = _mm256_set1_ps(1);
    __m256 _f2  = _mm256_set1_ps(2);
    __m256 _f3  = _mm256_set1_ps(3);
    __m256 _f4  = _mm256_set1_ps(4);
    __m256 _fm1 = _mm256_set1_ps(-1);
    __m256 _fm2 = _mm256_set1_ps(-2);

    __m256 _255 = _mm256_set1_ps(255);
    __m256 _threshold2 = _mm256_set1_ps(10000);

    for (u64 i = 0; i < (H - 3); i+=2){
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
            __m256 _a10 = _mm256_loadu_ps( &Aframe[ INDEX(i+3, j+0, W) ] );
            __m256 _a11 = _mm256_loadu_ps( &Aframe[ INDEX(i+3, j+1, W) ] );
            __m256 _a12 = _mm256_loadu_ps( &Aframe[ INDEX(i+3, j+2, W) ] );

            
            //b1
            __m256 _b1 = _mm256_setzero_ps();
            __m256 _b2 = _mm256_setzero_ps();
 
            _b1 = _mm256_sub_ps( _b1 ,_a1  );
            _b1 = _mm256_add_ps( _b1 ,_a3  );
            _b1 = _mm256_sub_ps( _b1 ,_a4  );
            _b1 = _mm256_sub_ps( _b1 ,_a4  );
            _b1 = _mm256_add_ps( _b1 ,_a6  );
            _b1 = _mm256_add_ps( _b1 ,_a6  );
            _b1 = _mm256_sub_ps( _b1 ,_a7  );
            _b1 = _mm256_add_ps( _b1 ,_a9  );

             
            _b2 = _mm256_sub_ps( _b2, _a1 );
            _b2 = _mm256_sub_ps( _b2, _a2 );
            _b2 = _mm256_sub_ps( _b2, _a2 );
            _b2 = _mm256_sub_ps( _b2, _a3 );
            _b2 = _mm256_add_ps( _b2, _a7 );
            _b2 = _mm256_add_ps( _b2, _a8 );
            _b2 = _mm256_add_ps( _b2, _a8 );
            _b2 = _mm256_add_ps( _b2, _a9 );

            _b1 = _mm256_mul_ps( _b1, _b1 );
            _b2 = _mm256_mul_ps( _b2, _b2 );
            _b1 = _mm256_add_ps( _b2, _b1 );


            _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
            _b1 = _mm256_add_ps( _b1, _255 );
            _mm256_store_ps( &Bframe[ INDEX(i, j, W)], _b1 );
           
            // 
            _b1 = _mm256_setzero_ps();
            _b2 = _mm256_setzero_ps();

            _b1 = _mm256_sub_ps( _b1 ,_a4  );
            _b1 = _mm256_add_ps( _b1 ,_a6  );
            _b1 = _mm256_sub_ps( _b1 ,_a7  );
            _b1 = _mm256_sub_ps( _b1 ,_a7  );
            _b1 = _mm256_add_ps( _b1 ,_a9  );
            _b1 = _mm256_add_ps( _b1 ,_a9  );
            _b1 = _mm256_sub_ps( _b1 ,_a10  );
            _b1 = _mm256_add_ps( _b1 ,_a12  );

             
            _b2 = _mm256_sub_ps( _b2, _a4 );
            _b2 = _mm256_sub_ps( _b2, _a5 );
            _b2 = _mm256_sub_ps( _b2, _a5 );
            _b2 = _mm256_sub_ps( _b2, _a6 );
            _b2 = _mm256_add_ps( _b2, _a10 );
            _b2 = _mm256_add_ps( _b2, _a11 );
            _b2 = _mm256_add_ps( _b2, _a11 );
            _b2 = _mm256_add_ps( _b2, _a12 );

            //
            // _b1 = 
            //     _mm256_add_ps( _mm256_mul_ps (_a4, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a6, _f1  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a7, _fm2 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a9, _f2  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a10, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a12, _f1  ), _b1))))));
            //
            // _b2 = 
            //     _mm256_add_ps( _mm256_mul_ps (_a4, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a5, _fm2 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a6, _fm1 ),
            //     _mm256_add_ps( _mm256_mul_ps (_a10, _f1  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a11, _f2  ),
            //     _mm256_add_ps( _mm256_mul_ps (_a12, _f1  ), _b2))))));

            _b1 = _mm256_mul_ps( _b1, _b1 );
            _b2 = _mm256_mul_ps( _b2, _b2 );
            _b1 = _mm256_add_ps( _b2, _b1 );


            _b1 = _mm256_cmp_ps( _threshold2, _b1, _MM_CMPINT_LE);
            _b1 = _mm256_add_ps( _b1, _255 );
            _mm256_store_ps( &Bframe[ INDEX(i+1, j, W)], _b1 );
            

        }
    }
}
