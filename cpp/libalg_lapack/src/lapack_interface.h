#ifndef _LAPACK_INTERFACE_H_4251E62FBE294DD787C379C1B490B0C2_
#define _LAPACK_INTERFACE_H_4251E62FBE294DD787C379C1B490B0C2_

/************************/
/*  lapack_interface.h  */
/*    Version 1.0       */
/*     2023/05/22       */
/************************/

// This file contains the interface to the LAPACK library.

// Eigenvalues and eigenvectors interface
#ifdef _MSC_VER
#define sgeev_ SGEEV
#define dgeev_ DGEEV
#define cgeev_ CGEEV
#define zgeev_ ZGEEV
#endif
// float
extern "C" void sgeev_(char* jobvl, char* jobvr, int* n, float* a, int* lda, float* wr, float* wi, float* vl, int* ldvl,
                       float* vr, int* ldvr, float* work, int* lwork, int* info);
// double
extern "C" void dgeev_(char* jobvl, char* jobvr, int* n, double* a, int* lda, double* wr, double* wi, double* vl,
                       int* ldvl, double* vr, int* ldvr, double* work, int* lwork, int* info);
// complex
extern "C" void cgeev_(char* jobvl, char* jobvr, int* n, float* a, int* lda, float* w, float* vl, int* ldvl, float* vr,
                       int* ldvr, float* work, int* lwork, float* rwork, int* info);
// double complex
extern "C" void zgeev_(char* jobvl, char* jobvr, int* n, double* a, int* lda, double* w, double* vl, int* ldvl,
                       double* vr, int* ldvr, double* work, int* lwork, double* rwork, int* info);
#endif

// LU interface
#ifdef _MSC_VER
#define sgetrf_ SGETRF
#define dgetrf_ DGETRF
#define cgetrf_ CGETRF
#define zgetrf_ ZGETRF
#endif
// float
extern "C" void sgetrf_(int* m, int* n, float* a, int* lda, int* ipiv, int* info);
// double
extern "C" void dgetrf_(int* m, int* n, double* a, int* lda, int* ipiv, int* info);
// complex
extern "C" void cgetrf_(int* m, int* n, float* a, int* lda, int* ipiv, int* info);
// double complex
extern "C" void zgetrf_(int* m, int* n, double* a, int* lda, int* ipiv, int* info);

// QR interface
#ifdef _MSC_VER
#define sgeqrf_ SGEQRF
#define sorgqr_ SORGQR

#define dgeqrf_ DGEQRF
#define dorgqr_ DORGQR

#define cgeqrf_ CGEQRF
#define cungqr_ CUNGQR

#define zgeqrf_ ZGEQRF
#define zungqr_ ZUNGQR
#endif
// float
extern "C" void sgeqrf_(int* m, int* n, float* a, int* lda, float* tau, float* work, int* lwork, int* info);
extern "C" void sorgqr_(int* m, int* n, int* k, float* a, int* lda, float* tau, float* work, int* lwork, int* info);
// double
extern "C" void dgeqrf_(int* m, int* n, double* a, int* lda, double* tau, double* work, int* lwork, int* info);
extern "C" void dorgqr_(int* m, int* n, int* k, double* a, int* lda, double* tau, double* work, int* lwork, int* info);
// complex
extern "C" void cgeqrf_(int* m, int* n, float* a, int* lda, float* tau, float* work, int* lwork, int* info);
extern "C" void cungqr_(int* m, int* n, int* k, float* a, int* lda, float* tau, float* work, int* lwork, int* info);
// double complex
extern "C" void zgeqrf_(int* m, int* n, double* a, int* lda, double* tau, double* work, int* lwork, int* info);
extern "C" void zungqr_(int* m, int* n, int* k, double* a, int* lda, double* tau, double* work, int* lwork, int* info);

// Schur interface
#ifdef _MSC_VER
#define sgees_ SGEES
#define dgees_ DGEES
#define cgees_ CGEES
#define zgees_ ZGEES
#endif
// float
extern "C" void sgees_(char* jobvs, char* sort, int (*select)(float*, float*), int* n, float* a, int* lda, int* sdim,
                       float* wr, float* wi, float* vs, int* ldvs, float* work, int* lwork, int* bwork, int* info);
// double
extern "C" void dgees_(char* jobvs, char* sort, int (*select)(double*, double*), int* n, double* a, int* lda, int* sdim,
                       double* wr, double* wi, double* vs, int* ldvs, double* work, int* lwork, int* bwork, int* info);
// complex
extern "C" void cgees_(char* jobvs, char* sort, int (*select)(float*), int* n, float* a, int* lda, int* sdim, float* w,
                       float* vs, int* ldvs, float* work, int* lwork, float* rwork, int* bwork, int* info);
// double complex
extern "C" void zgees_(char* jobvs, char* sort, int (*select)(double*), int* n, double* a, int* lda, int* sdim,
                       double* w, double* vs, int* ldvs, double* work, int* lwork, double* rwork, int* bwork,
                       int* info);

// SVD interface
#ifdef _MSC_VER
#define sgesvd_ SGESVD
#define sgesdd_ SGESDD
#define sgesvj_ SGESVJ
#define sgejsv_ SGEJSV

#define dgesvd_ DGESVD
#define dgesdd_ DGESDD
#define dgesvj_ DGESVJ
#define dgejsv_ DGEJSV

#define cgesvd_ CGESVD
#define cgesdd_ CGESDD
#define cgesvj_ CGESVJ
#define cgejsv_ CGEJSV

#define zgesvd_ ZGESVD
#define zgesdd_ ZGESDD
#define zgesvj_ ZGESVJ
#define zgejsv_ ZGEJSV
#endif
// float
extern "C" void sgesvd_(char* jobu, char* jobvt, int* m, int* n, float* a, int* lda, float* s, float* u, int* ldu,
                        float* vt, int* ldvt, float* work, int* lwork, int* info);
extern "C" void sgesdd_(char* jobz, int* m, int* n, float* a, int* lda, float* s, float* u, int* ldu, float* vt,
                        int* ldvt, float* work, int* lwork, int* iwork, int* info);
extern "C" void sgesvj_(char* joba, char* jobu, char* jobv, int* m, int* n, float* a, int* lda, float* sva, int* mv,
                        float* v, int* ldv, float* work, int* lwork, int* info);
extern "C" void sgejsv_(char* joba, char* jobu, char* jobv, char* jobr, char* jobt, char* jobp, int* m, int* n,
                        float* a, int* lda, float* sva, float* u, int* ldu, float* v, int* ldv, float* work, int* lwork,
                        int* iwork, int* info);
// double
extern "C" void dgesvd_(char* jobu, char* jobvt, int* m, int* n, double* a, int* lda, double* s, double* u, int* ldu,
                        double* vt, int* ldvt, double* work, int* lwork, int* info);
extern "C" void dgesdd_(char* jobz, int* m, int* n, double* a, int* lda, double* s, double* u, int* ldu, double* vt,
                        int* ldvt, double* work, int* lwork, int* iwork, int* info);
extern "C" void dgesvj_(char* joba, char* jobu, char* jobv, int* m, int* n, double* a, int* lda, double* sva, int* mv,
                        double* v, int* ldv, double* work, int* lwork, int* info);
extern "C" void dgejsv_(char* joba, char* jobu, char* jobv, char* jobr, char* jobt, char* jobp, int* m, int* n,
                        double* a, int* lda, double* sva, double* u, int* ldu, double* v, int* ldv, double* work,
                        int* lwork, int* iwork, int* info);
// complex
extern "C" void cgesvd_(char* jobu, char* jobvt, int* m, int* n, float* a, int* lda, float* s, float* u, int* ldu,
                        float* vt, int* ldvt, float* work, int* lwork, float* rwork, int* info);
extern "C" void cgesdd_(char* jobz, int* m, int* n, float* a, int* lda, float* s, float* u, int* ldu, float* vt,
                        int* ldvt, float* work, int* lwork, float* rwork, int* iwork, int* info);
extern "C" void cgesvj_(char* joba, char* jobu, char* jobv, int* m, int* n, float* a, int* lda, float* sva, int* mv,
                        float* v, int* ldv, float* work, int* lwork, float* rwork, int* lrwork, int* info);
extern "C" void cgejsv_(char* joba, char* jobu, char* jobv, char* jobr, char* jobt, char* jobp, int* m, int* n,
                        float* a, int* lda, float* sva, float* u, int* ldu, float* v, int* ldv, float* work, int* lwork,
                        float* rwork, int* lrwork, int* iwork, int* info);
// double complex
extern "C" void zgesvd_(char* jobu, char* jobvt, int* m, int* n, double* a, int* lda, double* s, double* u, int* ldu,
                        double* vt, int* ldvt, double* work, int* lwork, double* rwork, int* info);
extern "C" void zgesdd_(char* jobz, int* m, int* n, double* a, int* lda, double* s, double* u, int* ldu, double* vt,
                        int* ldvt, double* work, int* lwork, double* rwork, int* iwork, int* info);
extern "C" void zgesvj_(char* joba, char* jobu, char* jobv, int* m, int* n, double* a, int* lda, double* sva, int* mv,
                        double* v, int* ldv, double* work, int* lwork, double* rwork, int* lrwork, int* info);
extern "C" void zgejsv_(char* joba, char* jobu, char* jobv, char* jobr, char* jobt, char* jobp, int* m, int* n,
                        double* a, int* lda, double* sva, double* u, int* ldu, double* v, int* ldv, double* work,
                        int* lwork, double* rwork, int* lrwork, int* iwork, int* info);
