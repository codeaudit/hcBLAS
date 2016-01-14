/*
*
*  FILENAME : hcblaslib.h
*  This file is the top level header file which includes the Hcblaslilbrary class
*  for performing five blas operations ( saxpy, sger, sgemv, sgemm, cgemm )
*
*/

#ifndef HCBLASLIB_H
#define HCBLASLIB_H

#include <iostream>
#include "hc.hpp"
#include <amp_short_vectors.h>

using namespace hc;
using namespace Concurrency::graphics;
using namespace std;
/* enumerator to indicate the status of  blas operation */
enum hcblasStatus {
    HCBLAS_SUCCEEDS = 0,
    HCBLAS_INVALID = -1
};

/* enumerator to define the layout of  input matrix for blas operation */
enum hcblasOrder {
    RowMajor ,
    ColMajor
};

/* enumerator to define the type of operation to be performed on the input matrix
 ( NO_TRANSPOSE, TRANSPOSE, CONJUGATE) */
enum hcblasTranspose {
    NoTrans = 'n',
    Trans = 't'
};

struct hc_Complex
{
     float real;
     float img;
};

/* Class which implements the blas ( SGEMM, CGEMM, SGEMV, SGER, SAXPY )  */
class Hcblaslibrary
{
    public:
/*                  Y = alpha * X + Y                                    */
    hcblasStatus hcblas_saxpy(const int N,
                              const float *alpha, float *X, const int incX,
                              float *Y, const int incY, const long xOffset,
                              const long yOffset);

/* SAXPY - Overloaded function with arguments of type hc::array_view */

    hcblasStatus hcblas_saxpy(hc::accelerator_view &accl_view,
			      const int N, const float &alpha,
			      hc::array<float> &X, const int incX,
                              hc::array<float> &Y, const int incY, 
			      const long xOffset, const long yOffset);

/* SAXPY - Overloaded function with arguments related to batch processing */

    hcblasStatus hcblas_saxpy(hc::accelerator_view &accl_view,
                              const int N, const float &alpha,
                              hc::array<float> &X, const int incX, const long X_batchOffset,
                              hc::array<float> &Y, const int incY, const long Y_batchOffset,
                              const long xOffset, const long yOffset, const int batchSize);

/* SAXPY - Overloaded function with arguments of type hc::array_view */

    hcblasStatus hcblas_saxpy(hc::accelerator_view &accl_view,
			      const int N, const float &alpha,
			      hc::array_view<float> &X, const int incX,
                              hc::array_view<float> &Y, const int incY, 
			      const long xOffset, const long yOffset);

/* SAXPY - Overloaded function with arguments related to batch processing */

    hcblasStatus hcblas_saxpy(hc::accelerator_view &accl_view,
                              const int N, const float &alpha,
                              hc::array_view<float> &X, const int incX, const long X_batchOffset,
                              hc::array_view<float> &Y, const int incY, const long Y_batchOffset,
                              const long xOffset, const long yOffset, const int batchSize);
							  
/*                  A = alpha * X * Y' + A                               */
    hcblasStatus hcblas_sger(hcblasOrder order, const int M, const int N, const float *alpha,
                             float *X, const long xOffset, const int incX,
                             float *Y, const long yOffset, const int incY,
                             float *A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments of type hc::array_view */

    hcblasStatus hcblas_sger(hc::accelerator_view &accl_view,
			     hcblasOrder order, const int M, const int N, const float &alpha,
                             hc::array<float> &X, const long xOffset, const int incX,
                             hc::array<float> &Y, const long yOffset, const int incY,
                             hc::array<float> &A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments related to batch processing */

    hcblasStatus hcblas_sger(hc::accelerator_view &accl_view,
                             hcblasOrder order, const int M, const int N, const float &alpha,
                             hc::array<float> &X, 
                             const long xOffset, const long X_batchOffset, const int incX,
                             hc::array<float> &Y, 
                             const long yOffset, const long Y_batchOffset, const int incY,
                             hc::array<float> &A, 
                             const long aOffset, const long A_batchOffset, const int lda, const int batchSize);

/* SGER - Overloaded function with arguments of type hc::array_view */

    hcblasStatus hcblas_sger(hc::accelerator_view &accl_view,
			     hcblasOrder order, const int M, const int N, const float &alpha,
                             hc::array_view<float> &X, const long xOffset, const int incX,
                             hc::array_view<float> &Y, const long yOffset, const int incY,
                             hc::array_view<float> &A, const long aOffset, const int lda);

/* SGER - Overloaded function with arguments related to batch processing */

    hcblasStatus hcblas_sger(hc::accelerator_view &accl_view,
                             hcblasOrder order, const int M, const int N, const float &alpha,
                             hc::array_view<float> &X, 
                             const long xOffset, const long X_batchOffset, const int incX,
                             hc::array_view<float> &Y, 
                             const long yOffset, const long Y_batchOffset, const int incY,
                             hc::array_view<float> &A, 
                             const long aOffset, const long A_batchOffset, const int lda, const int batchSize);
							 
/*                  Y = alpha * op(A) * X + beta * Y                     */
    hcblasStatus hcblas_sgemv(hcblasOrder order, hcblasTranspose type, const int M,
                              const int N, const float *alpha, float *A,
                              const long aOffset,const int lda, float *X,
                              const long xOffset, const int incX,
                              const float *beta, float *Y,const long yOffset,
                              const int incY);

/* SGEMV- Overloaded function with arguments of type hc::array_view */
    hcblasStatus hcblas_sgemv(hc::accelerator_view &accl_view,
			      hcblasOrder order, hcblasTranspose type, const int M,
                              const int N, const float &alpha, 
                              hc::array<float> &A, const long aOffset, const int lda, 
			      hc::array<float> &X, const long xOffset, const int incX,
                              const float &beta,  
			      hc::array<float> &Y, const long yOffset, const int incY);

/* SGEMV- Overloaded function with arguments related to batch processing */
    hcblasStatus hcblas_sgemv(hc::accelerator_view &accl_view,
                              hcblasOrder order, hcblasTranspose type, const int M,
                              const int N, const float &alpha, hc::array<float> &A, 
                              const long aOffset, const long A_batchOffset, const int lda,
                              hc::array<float> &X, 
                              const long xOffset, const long X_batchOffset, const int incX,
                              const float &beta, hc::array<float> &Y, 
                              const long yOffset, const long Y_batchOffset, const int incY, const int batchSize);

/* SGEMV- Overloaded function with arguments of type hc::array_view */
    hcblasStatus hcblas_sgemv(hc::accelerator_view &accl_view,
			      hcblasOrder order, hcblasTranspose type, const int M,
                              const int N, const float &alpha, 
                              hc::array_view<float> &A, const long aOffset, const int lda, 
			      hc::array_view<float> &X, const long xOffset, const int incX,
                              const float &beta,  
			      hc::array_view<float> &Y, const long yOffset, const int incY);

/* SGEMV- Overloaded function with arguments related to batch processing */
    hcblasStatus hcblas_sgemv(hc::accelerator_view &accl_view,
                              hcblasOrder order, hcblasTranspose type, const int M,
                              const int N, const float &alpha, hc::array_view<float> &A, 
                              const long aOffset, const long A_batchOffset, const int lda,
                              hc::array_view<float> &X, 
                              const long xOffset, const long X_batchOffset, const int incX,
                              const float &beta, hc::array_view<float> &Y, 
                              const long yOffset, const long Y_batchOffset, const int incY, const int batchSize);
							  
/*                  C = alpha * op(A) * op(B) + beta * C                 */
    hcblasStatus hcblas_sgemm(hcblasOrder order, hcblasTranspose typeA,
                              hcblasTranspose typeB, const int M,
                              const int N, const int K, const float *alpha,
                              float *A, const long lda, float *B,
                              const long ldb, const float *beta, float *C,
                              const long ldc, const long aOffset,
                              const long bOffset, const long cOffset);

/* SGEMM- Overloaded function with arguments of type hc::array_view */
    hcblasStatus hcblas_sgemm(hc::accelerator_view &accl_view,
 			      hcblasOrder order, hcblasTranspose typeA,
                              hcblasTranspose typeB, const int M,
                              const int N, const int K, const float &alpha,
                              hc::array<float> &A, const long lda, 
		              hc::array<float> &B, const long ldb, 
			      const float &beta,  
		       	      hc::array<float> &C, const long ldc, 
			      const long aOffset, const long bOffset, const long cOffset);


/* SGEMM- Overloaded function with arguments related to batch processing */
    hcblasStatus hcblas_sgemm(hc::accelerator_view &accl_view,
                              hcblasOrder order, hcblasTranspose typeA,
                              hcblasTranspose typeB, const int M,
                              const int N, const int K, const float &alpha,
                              hc::array<float> &A, const long lda, const long A_batchOffset,
                              hc::array<float> &B, const long ldb, const long B_batchOffset,
                              const float &beta,
                              hc::array<float> &C, const long ldc, const long C_batchOffset,
                              const long aOffset, const long bOffset, const long cOffset, const int batchSize);

/* SGEMM- Overloaded function with arguments of type hc::array_view */
    hcblasStatus hcblas_sgemm(hc::accelerator_view &accl_view,
 			      hcblasOrder order, hcblasTranspose typeA,
                              hcblasTranspose typeB, const int M,
                              const int N, const int K, const float &alpha,
                              hc::array_view<float> &A, const long lda, 
		              hc::array_view<float> &B, const long ldb, 
			      const float &beta,  
			      hc::array_view<float> &C, const long ldc, 
			      const long aOffset, const long bOffset, const long cOffset);


/* SGEMM- Overloaded function with arguments related to batch processing */
    hcblasStatus hcblas_sgemm(hc::accelerator_view &accl_view,
                              hcblasOrder order, hcblasTranspose typeA,
                              hcblasTranspose typeB, const int M,
                              const int N, const int K, const float &alpha,
                              hc::array_view<float> &A, const long lda, const long A_batchOffset,
                              hc::array_view<float> &B, const long ldb, const long B_batchOffset,
                              const float &beta,
                              hc::array_view<float> &C, const long ldc, const long C_batchOffset,
                              const long aOffset, const long bOffset, const long cOffset, const int batchSize);
							  
/*                  C = alpha * op(A) * op(B) + beta * C                   */
    hcblasStatus hcblas_cgemm(hcblasOrder order, hcblasTranspose typeA,
                              hcblasTranspose typeB, const int M, 
                              const int N, const int K,
                              const hc_Complex *alpha,
                              const hc_Complex *A, const long aOffset, const long lda,
                              const hc_Complex *B, const long bOffset, const long ldb,
                              const hc_Complex *beta, hc_Complex *C,
                              const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments of type hc::array_view */     
   hcblasStatus hcblas_cgemm(hc::accelerator_view &accl_view,
			     hcblasOrder order, hcblasTranspose typeA,
                             hcblasTranspose typeB, const int M,
                             const int N, const int K,
                             const Concurrency::graphics::float_2 &alpha,
                             hc::array<float_2> &A, const long aOffset, const long lda,
                             hc::array<float_2> &B, const long bOffset, const long ldb,
                             const Concurrency::graphics::float_2 &beta, 
                             hc::array<float_2> &C, const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_cgemm(hc::accelerator_view &accl_view,
                             hcblasOrder order, hcblasTranspose typeA,
                             hcblasTranspose typeB, const int M,
                             const int N, const int K,
                             const Concurrency::graphics::float_2 &alpha,
                             hc::array<float_2> &A, 
                             const long aOffset, const long A_batchOffset, const long lda,
                             hc::array<float_2> &B, 
			     const long bOffset, const long B_batchOffset, const long ldb,
                             const Concurrency::graphics::float_2 &beta,
                             hc::array<float_2> &C, 
			     const long cOffset, const long C_batchOffset, const long ldc, const int batchSize);

/* CGEMM - Overloaded function with arguments of type hc::array_view */     
   hcblasStatus hcblas_cgemm(hc::accelerator_view &accl_view,
			     hcblasOrder order, hcblasTranspose typeA,
                             hcblasTranspose typeB, const int M,
                             const int N, const int K,
                             const Concurrency::graphics::float_2 &alpha,
                             hc::array_view<float_2> &A, const long aOffset, const long lda,
                             hc::array_view<float_2> &B, const long bOffset, const long ldb,
                             const Concurrency::graphics::float_2 &beta, 
                             hc::array_view<float_2> &C, const long cOffset, const long ldc);

/* CGEMM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_cgemm(hc::accelerator_view &accl_view,
                             hcblasOrder order, hcblasTranspose typeA,
                             hcblasTranspose typeB, const int M,
                             const int N, const int K,
                             const Concurrency::graphics::float_2 &alpha,
                             hc::array_view<float_2> &A, 
                             const long aOffset, const long A_batchOffset, const long lda,
                             hc::array_view<float_2> &B, 
			     const long bOffset, const long B_batchOffset, const long ldb,
                             const Concurrency::graphics::float_2 &beta,
                             hc::array_view<float_2> &C, 
			     const long cOffset, const long C_batchOffset, const long ldc, const int batchSize);
				 
/* SSCAL - X = alpha * X */
   hcblasStatus hcblas_sscal(const int N, const float *alpha,
                             float *X, const int incX, const long xOffset);

/* SSCAL - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_sscal(hc::accelerator_view &accl_view,
                             const int N, const float &alpha,
                             hc::array<float> &X, const int incX,
                             const long xOffset);

/* SSCAL - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sscal(hc::accelerator_view &accl_view,
                             const int N,const float &alpha,
                             hc::array<float> &X, const int incX,
                             const long xOffset, const long X_batchOffset, const int batchSize);

/* SSCAL - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_sscal(hc::accelerator_view &accl_view,
                             const int N, const float &alpha,
                             hc::array_view<float> &X, const int incX,
                             const long xOffset);

/* SSCAL - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sscal(hc::accelerator_view &accl_view,
                             const int N,const float &alpha,
                             hc::array_view<float> &X, const int incX,
                             const long xOffset, const long X_batchOffset, const int batchSize);
							 
/* DSCAL - X = alpha * X */
   hcblasStatus hcblas_dscal(const int N, const double *alpha,
                             double *X, const int incX, const long xOffset);

/* DSCAL - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_dscal(hc::accelerator_view &accl_view,
                             const int N, const double &alpha,
                             hc::array<double> &X, const int incX,
                             const long xOffset);

/* DSCAL - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dscal(hc::accelerator_view &accl_view,
                             const int N,const double &alpha,
                             hc::array<double> &X, const int incX,
                             const long xOffset, const long X_batchOffset, const int batchSize);

/* DSCAL - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_dscal(hc::accelerator_view &accl_view,
                             const int N, const double &alpha,
                             hc::array_view<double> &X, const int incX,
                             const long xOffset);

/* DSCAL - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dscal(hc::accelerator_view &accl_view,
                             const int N,const double &alpha,
                             hc::array_view<double> &X, const int incX,
                             const long xOffset, const long X_batchOffset, const int batchSize);
							 
/* SCOPY - Copies a vector X to a vector Y */
   hcblasStatus hcblas_scopy(const int N, float *X, const int incX, const long xOffset, 
                             float *Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_scopy(hc::accelerator_view &accl_view, const int N,
                             hc::array<float> &X, const int incX, const long xOffset,
                             hc::array<float> &Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_scopy(hc::accelerator_view &accl_view, const int N,
                             hc::array<float> &X, const int incX, const long xOffset,
                             hc::array<float> &Y, const int incY, const long yOffset,
                             const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* SCOPY - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_scopy(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<float> &X, const int incX, const long xOffset,
                             hc::array_view<float> &Y, const int incY, const long yOffset);

/* SCOPY - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_scopy(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<float> &X, const int incX, const long xOffset,
                             hc::array_view<float> &Y, const int incY, const long yOffset,
                             const long X_batchOffset, const long Y_batchOffset, const int batchSize);
							 
/* DCOPY - Copies a vector X to a vector Y */
   hcblasStatus hcblas_dcopy(const int N, double *X, const int incX, const long xOffset, 
                             double *Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_dcopy(hc::accelerator_view &accl_view, const int N,
                             hc::array<double> &X, const int incX, const long xOffset,
                             hc::array<double> &Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dcopy(hc::accelerator_view &accl_view, const int N,
                             hc::array<double> &X, const int incX, const long xOffset,
                             hc::array<double> &Y, const int incY, const long yOffset,
                             const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* DCOPY - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_dcopy(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<double> &X, const int incX, const long xOffset,
                             hc::array_view<double> &Y, const int incY, const long yOffset);

/* DCOPY - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dcopy(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<double> &X, const int incX, const long xOffset,
                             hc::array_view<double> &Y, const int incY, const long yOffset,
                             const long X_batchOffset, const long Y_batchOffset, const int batchSize);
							 
/* SDOT - Single Precision Dot product */
   hcblasStatus hcblas_sdot(const int N, float *X, const int incX, const long xOffset, 
                            float *Y, const int incY, const long yOffset, float *dot);

/* SDOT - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_sdot(hc::accelerator_view &accl_view, const int N,
                            hc::array<float> &X, const int incX, const long xOffset,
                            hc::array<float> &Y, const int incY, const long yOffset, float &dot);

/* SDOT - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sdot(hc::accelerator_view &accl_view, const int N,
                            hc::array<float> &X, const int incX, const long xOffset,
                            hc::array<float> &Y, const int incY, const long yOffset, float &dot,
                            const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* SDOT - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_sdot(hc::accelerator_view &accl_view, const int N,
                            hc::array_view<float> &X, const int incX, const long xOffset,
                            hc::array_view<float> &Y, const int incY, const long yOffset, float &dot);

/* SDOT - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sdot(hc::accelerator_view &accl_view, const int N,
                            hc::array_view<float> &X, const int incX, const long xOffset,
                            hc::array_view<float> &Y, const int incY, const long yOffset, float &dot,
                            const long X_batchOffset, const long Y_batchOffset, const int batchSize);
							
/* DDOT - Double Precision Dot product */
   hcblasStatus hcblas_ddot(const int N, double *X, const int incX, const long xOffset,
                            double *Y, const int incY, const long yOffset, double *dot);

/* DDOT - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_ddot(hc::accelerator_view &accl_view, const int N,
                            hc::array<double> &X, const int incX, const long xOffset,
                            hc::array<double> &Y, const int incY, const long yOffset, double &dot);

/* DDOT - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_ddot(hc::accelerator_view &accl_view, const int N,
                            hc::array<double> &X, const int incX, const long xOffset,
                            hc::array<double> &Y, const int incY, const long yOffset, double &dot,
                            const long X_batchOffset, const long Y_batchOffset, const int batchSize);

/* DDOT - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_ddot(hc::accelerator_view &accl_view, const int N,
                            hc::array_view<double> &X, const int incX, const long xOffset,
                            hc::array_view<double> &Y, const int incY, const long yOffset, double &dot);

/* DDOT - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_ddot(hc::accelerator_view &accl_view, const int N,
                            hc::array_view<double> &X, const int incX, const long xOffset,
                            hc::array_view<double> &Y, const int incY, const long yOffset, double &dot,
                            const long X_batchOffset, const long Y_batchOffset, const int batchSize);
							
/* SASUM - Absolute value of a Vector - Single Precision */
   hcblasStatus hcblas_sasum(const int N, float *X, const int incX, const long xOffset, float *Y);    
   
/* SASUM - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_sasum(hc::accelerator_view &accl_view, const int N,
                             hc::array<float> &X, const int incX,
                             const long xOffset, float &Y);

/* SASUM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sasum(hc::accelerator_view &accl_view, const int N,
                             hc::array<float> &X, const int incX,
                             const long xOffset, float &Y, const long X_batchOffset, const int batchSize);

/* SASUM - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_sasum(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<float> &X, const int incX,
                             const long xOffset, float &Y);

/* SASUM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_sasum(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<float> &X, const int incX,
                             const long xOffset, float &Y, const long X_batchOffset, const int batchSize);
							 
/* DASUM - Absolute value of a Vector - Double Precision */
   hcblasStatus hcblas_dasum(const int N, double *X, const int incX, const long xOffset, double *Y);

/* DASUM - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_dasum(hc::accelerator_view &accl_view, const int N,
                             hc::array<double> &X, const int incX,
                             const long xOffset, double &Y);

/* DASUM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dasum(hc::accelerator_view &accl_view, const int N,
                             hc::array<double> &X, const int incX,
                             const long xOffset, double &Y, const long X_batchOffset, const int batchSize);

/* DASUM - Overloaded function with arguments of type hc::array_view */
   hcblasStatus hcblas_dasum(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<double> &X, const int incX,
                             const long xOffset, double &Y);

/* DASUM - Overloaded function with arguments related to batch processing */
   hcblasStatus hcblas_dasum(hc::accelerator_view &accl_view, const int N,
                             hc::array_view<double> &X, const int incX,
                             const long xOffset, double &Y, const long X_batchOffset, const int batchSize);
};


#endif
