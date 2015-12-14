#ifndef __DGEMM_H__
#define __DGEMM_H__

// TODO :Recuperer dgemm du TD précédent !

#include "cblas.h"
#define CBLAS_TRANSPOSE enum CBLAS_TRANSPOSE //CblasNoTrans || CblasTrans (|| CblasConjTrans)
#define CBLAS_ORDER enum CBLAS_ORDER //CblasColMajor (|| CblasRowMajor)
#define CBLAS_SIDE enum CBLAS_SIDE //CblasLeft || CblasRight
#define CBLAS_UPLO enum CBLAS_UPLO //CblasUpper || CblasLower
#define CBLAS_DIAG enum CBLAS_DIAG //CblasNonUnit || CblasUnit

void dgemm(const CBLAS_ORDER Order, const CBLAS_TRANSPOSE TransA,
           const CBLAS_TRANSPOSE TransB, const int M, const int N,
           const int K, const double alpha, const double *A,
           const int lda, const double *B, const int ldb,
           const double beta, double *C, const int ldc);

#endif // __DGEMM_H__
