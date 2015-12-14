/*
  Global libraries included
 */
#include <stdio.h>
#include <stdlib.h>
/*
  Local libraries included
 */
#include "util.h"
#include "dgemm.h"

inline int coord_cm(const int i, const int j, const CBLAS_TRANSPOSE Trans, const int ld){
  if (Trans == CblasTrans){
    return i*ld+j;
  } else {
    return j*ld+i;
  }
}

void dgemm(const CBLAS_ORDER Order, const CBLAS_TRANSPOSE TransA,
                 const CBLAS_TRANSPOSE TransB, const int M, const int N,
                 const int K, const double alpha, const double *A,
                 const int lda, const double *B, const int ldb,
                 const double beta, double *C, const int ldc){
    assert(Order == CblasColMajor);
    assert(beta == 1.0);

    int i, j, k;
    for(j = 0; j<N; j++)
        for(i = 0; i<M; i++)
        {
            double c = 0;
            for(k = 0; k<K; k++)
	      c += A[coord_cm(i,k,TransA,lda)]*B[coord_cm(k,j,TransB,ldb)];
            C[coord_cm(i,j,CblasNoTrans,ldc)] += alpha*c;
        }
}
