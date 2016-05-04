#include "sgemm_array_kernels.h"
#include <cmath>
#include "hc_math.hpp"

hcblasStatus gemm_NoTransAB_MICRO_NBK_MX064_NX064_KX16_TS16XMTS4(hc::accelerator_view accl_view,
					        const float *A, long aOffset,
					        const float *B, long bOffset,
					        float *C, long cOffset,
					        int M, int N, int K, int lda, int ldb, int ldc,
					        float alpha, float beta) {
#undef TILESIZE
#undef MICROTILESIZE
#define TILESIZE 16
#define MICROTILESIZE 4 
  int M_ = M >> 2;
  int N_ = N >> 2;
  hc::extent<2> grdExt((N_ + (TILESIZE - 1)) & ~(TILESIZE - 1), (M_ + (TILESIZE - 1)) & ~(TILESIZE - 1));
  hc::tiled_extent<2> t_ext = grdExt.tile(TILESIZE, TILESIZE);
  hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<2> tidx) __attribute__((hc, cpu)) {
    float rC[4][4] = {{(float)0}};
    float rA[1][4];
    float rB[1][4];
    tile_static float lA[1056];
    tile_static float lB[1056];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int block_k = K >> 4;
    int alIndex = (idy * 65) + idx;
    int blIndex = (idx * 65) + idy;
    int AIndex = aOffset + (gidx * 64) + idx + (idy * lda);
    int BIndex = bOffset + ((gidy * 64) + idy)*ldb + idx;
    long CIndex = cOffset + (gidx * 64) + idx + (((gidy * 64) + idy) * ldc);
    long AinitOffset = 0; 
    long BinitOffset = 0;
    long CinitOffset = 0;
    do {

      tidx.barrier.wait();
      lB[blIndex] = B[BIndex + BinitOffset];
      lB[blIndex + 16] = B[BIndex + BinitOffset +  16 * ldb];
      lB[blIndex + 32] = B[BIndex + BinitOffset +  32 * ldb];
      lB[blIndex + 48] = B[BIndex + BinitOffset +  48 * ldb];
      lA[alIndex] = A[AIndex + AinitOffset ];
      lA[alIndex + 16] = A[AIndex + 16 + AinitOffset];
      lA[alIndex + 32] = A[AIndex + 32 + AinitOffset];
      lA[alIndex + 48] = A[AIndex + 48 + AinitOffset];
      
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;

      for (int iter = 0; iter < TILESIZE; iter+=8) {
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
        M4x4;
      }

      AinitOffset += lda << 4;
      BinitOffset += 16;

    } while (--block_k > 0); // (((K + TILESIZE - 1) & ~(TILESIZE - 1)) / TILESIZE));

    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[0][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[0][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[0][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[0][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[1][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[1][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[1][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[1][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[2][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[2][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[2][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[2][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[3][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[3][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[3][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[3][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    
  }).wait();
#undef TILESIZE
#undef MICROTILESIZE
  return HCBLAS_SUCCEEDS;
}


hcblasStatus gemm_NoTransAB_MICRO_NBK_MX096_NX096_KX16_TS16XMTS6(hc::accelerator_view accl_view,
					        const float *A, long aOffset,
					        const float *B, long bOffset,
					        float *C, long cOffset,
					        int M, int N, int K, int lda, int ldb, int ldc,
					        float alpha, float beta) {
#undef TILESIZE
#undef MICROTILESIZE
#define TILESIZE 16
#define MICROTILESIZE 6 
  int M_ = M/6;
  int N_ = N/6 ;
  hc::extent<2> grdExt((N_ + (TILESIZE - 1)) & ~(TILESIZE - 1), (M_ + (TILESIZE - 1)) & ~(TILESIZE - 1));
  hc::tiled_extent<2> t_ext = grdExt.tile(TILESIZE, TILESIZE);
  hc::parallel_for_each(accl_view, t_ext, [ = ] (hc::tiled_index<2> tidx) __attribute__((hc, cpu)) {
    float rC[6][6] = {{(float)0}};
    float rA[1][6];
    float rB[1][6];
    tile_static float lA[1552];
    tile_static float lB[1552];
    int gidx = tidx.tile[1];
    int gidy = tidx.tile[0];
    int idx = tidx.local[1];
    int idy = tidx.local[0];
    int block_k = K >> 4;
    int alIndex = (idy * 97) + idx;
    int blIndex = (idx * 97) + idy;
    int AIndex = aOffset + (gidx * 96) + idx + (idy * lda);
    int BIndex = bOffset + ((gidy * 96) + idy)*ldb + idx;
    long CIndex = cOffset + (gidx * 96) + idx + (((gidy * 96) + idy) * ldc);
    long AinitOffset = 0; 
    long BinitOffset = 0;
    long CinitOffset = 0;
    do {

      tidx.barrier.wait();
      lB[blIndex] = B[BIndex + BinitOffset];
      lB[blIndex + 16] = B[BIndex + BinitOffset +  16 * ldb];
      lB[blIndex + 32] = B[BIndex + BinitOffset +  32 * ldb];
      lB[blIndex + 48] = B[BIndex + BinitOffset +  48 * ldb];
      lB[blIndex + 64] = B[BIndex + BinitOffset +  64 * ldb];
      lB[blIndex + 80] = B[BIndex + BinitOffset +  80 * ldb];
      lA[alIndex] = A[AIndex + AinitOffset ];
      lA[alIndex + 16] = A[AIndex + 16 + AinitOffset];
      lA[alIndex + 32] = A[AIndex + 32 + AinitOffset];
      lA[alIndex + 48] = A[AIndex + 48 + AinitOffset];
      lA[alIndex + 64] = A[AIndex + 64 + AinitOffset];
      lA[alIndex + 80] = A[AIndex + 80 + AinitOffset];
      
      tidx.barrier.wait();

      int offA = idx;
      int offB = idy;

      for (int iter = 0; iter < TILESIZE; iter+=8) {
        M6x6;
        M6x6;
        M6x6;
        M6x6;
        M6x6;
        M6x6;
        M6x6;
        M6x6;
      }

      AinitOffset += lda << 4;
      BinitOffset += 16;

    } while (--block_k > 0); // (((K + TILESIZE - 1) & ~(TILESIZE - 1)) / TILESIZE));

    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[0][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[0][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[0][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[0][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    C[CIndex + CinitOffset + 64 * ldc] = alpha*rC[0][4] + beta * C[CIndex + CinitOffset + 64 * ldc] ;
    C[CIndex + CinitOffset + 80 * ldc] = alpha*rC[0][5] + beta * C[CIndex + CinitOffset + 80 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[1][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[1][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[1][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[1][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    C[CIndex + CinitOffset + 64 * ldc] = alpha*rC[1][4] + beta * C[CIndex + CinitOffset + 64 * ldc] ;
    C[CIndex + CinitOffset + 80 * ldc] = alpha*rC[1][5] + beta * C[CIndex + CinitOffset + 80 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[2][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[2][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[2][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[2][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    C[CIndex + CinitOffset + 64 * ldc] = alpha*rC[2][4] + beta * C[CIndex + CinitOffset + 64 * ldc] ;
    C[CIndex + CinitOffset + 80 * ldc] = alpha*rC[2][5] + beta * C[CIndex + CinitOffset + 80 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[3][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[3][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[3][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[3][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    C[CIndex + CinitOffset + 64 * ldc] = alpha*rC[3][4] + beta * C[CIndex + CinitOffset + 64 * ldc] ;
    C[CIndex + CinitOffset + 80 * ldc] = alpha*rC[3][5] + beta * C[CIndex + CinitOffset + 80 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[4][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[4][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[4][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[4][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    C[CIndex + CinitOffset + 64 * ldc] = alpha*rC[4][4] + beta * C[CIndex + CinitOffset + 64 * ldc] ;
    C[CIndex + CinitOffset + 80 * ldc] = alpha*rC[4][5] + beta * C[CIndex + CinitOffset + 80 * ldc] ;
    CinitOffset+=16;
    C[CIndex + CinitOffset + 0 * ldc] = alpha*rC[5][0] + beta * C[CIndex + CinitOffset + 0 * ldc] ;
    C[CIndex + CinitOffset + 16 * ldc] = alpha*rC[5][1] + beta * C[CIndex + CinitOffset + 16 * ldc] ;
    C[CIndex + CinitOffset + 32 * ldc] = alpha*rC[5][2] + beta * C[CIndex + CinitOffset + 32 * ldc] ;
    C[CIndex + CinitOffset + 48 * ldc] = alpha*rC[5][3] + beta * C[CIndex + CinitOffset + 48 * ldc] ;
    C[CIndex + CinitOffset + 64 * ldc] = alpha*rC[5][4] + beta * C[CIndex + CinitOffset + 64 * ldc] ;
    C[CIndex + CinitOffset + 80 * ldc] = alpha*rC[5][5] + beta * C[CIndex + CinitOffset + 80 * ldc] ;
    
  }).wait();
#undef TILESIZE
#undef MICROTILESIZE
  return HCBLAS_SUCCEEDS;
}

