#ifndef _BLAS_INTERFACE_H_BEF6CF88E4D04B528112A18FD1976A91_
#define _BLAS_INTERFACE_H_BEF6CF88E4D04B528112A18FD1976A91_

/************************/
/*  blas_interface.h   */
/*    Version 1.0       */
/*     2023/06/04       */
/************************/

#ifdef _MSC_VER
#define sgemv_ SGEMV
#define sgemm_ SGEMM
#define dgemv_ DGEMV
#define dgemm_ DGEMM
#define cgemv_ CGEMV
#define cgemm_ CGEMM
#define zgemv_ ZGEMV
#define zgemm_ ZGEMM
#endif

// float
extern "C" void sgemv_(char* trans, int* m, int* n, float* alpha, float* A, int* lda, float* x, int* incx, float* beta,
                       float* y, int* incy);
extern "C" void sgemm_(char* transa, char* transb, int* m, int* n, int* k, float* alpha, float* a, int* lda, float* b,
                       int* ldb, float* beta, float* c, int* ldc);
// double
extern "C" void dgemv_(char* trans, int* m, int* n, double* alpha, double* A, int* lda, double* x, int* incx,
                       double* beta, double* y, int* incy);
extern "C" void dgemm_(char* transa, char* transb, int* m, int* n, int* k, double* alpha, double* a, int* lda,
                       double* b, int* ldb, double* beta, double* c, int* ldc);
// complex
extern "C" void cgemv_(char* trans, int* m, int* n, float* alpha, float* A, int* lda, float* x, int* incx, float* beta,
                       float* y, int* incy);
extern "C" void cgemm_(char* transa, char* transb, int* m, int* n, int* k, float* alpha, float* a, int* lda, float* b,
                       int* ldb, float* beta, float* c, int* ldc);
// double complex
extern "C" void zgemv_(char* trans, int* m, int* n, double* alpha, double* A, int* lda, double* x, int* incx,
                       double* beta, double* y, int* incy);
extern "C" void zgemm_(char* transa, char* transb, int* m, int* n, int* k, double* alpha, double* a, int* lda,
                       double* b, int* ldb, double* beta, double* c, int* ldc);

#endif
