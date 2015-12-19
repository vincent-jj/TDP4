/*
  Global libraries included
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*
  Local libraries included
 */
#include "fox.h"
#include "dgemm.h"
#include "util.h"
#include "cblas.h"
/*
  Implementation of init_fox_mult_matrix defined in fox.h
 */
void init_fox_mult_matrix(int nb_proc_row, MPI_Comm *communicator, struct grid * grid, int rank){
  int remaining_dimensions[]={0,1};
  // Grid subdivision in row communicator
  MPI_Cart_sub(*communicator, remaining_dimensions, &grid->communicator_row);
  grid->n = nb_proc_row;
  int coords[2];
  MPI_Cart_coords(*communicator, rank, 2, coords);
  grid->rank_i = coords[0];
  grid->rank_j = coords[1];
  // Determination of closest process rank
  coords[0]+=1;
  MPI_Cart_rank(*communicator, coords, &(grid->process_under));
  coords[1]-=2;
  MPI_Cart_rank(*communicator, coords, &(grid->process_above));
  return;
}

/*
  Implementation of finalize_fox_mult defined in fox.h
 */
int finalize_fox_mult(struct grid *grid){
  MPI_Comm_free(&grid->communicator_row);
  return EXIT_SUCCESS;
}

inline void nullify_table(double *table, int size){
  int i;
  for(i = 0; i < size; ++i)
    table[i] = 0.0;
  return;
}
/*
  Implementation of fox multiplication
 */
void mult_fox_mpi(int n, 
		  const double *Aij, 
		  const double *Bij, 
		  double *Cij,
		  const struct grid *grid, 
		  MPI_Comm communicator){
  int i,j,k,size,c; i = grid->rank_i; j = grid->rank_j; size = grid->n;
  int total_size = grid->n*grid->n;
  double *Aik = (double *)malloc(sizeof(double)*total_size);
  double *Bkj = (double *)malloc(sizeof(double)*total_size);
  double *next_Bkj = (double *)malloc(sizeof(double)*total_size);
  double *free_B;
  // Initialize Cij
  nullify_table(Cij,total_size);

  for(c = 0; c < grid->n; c++){
    k = ((grid->rank_i)+c)%(grid->n);
    double *Bkj_current = (k==grid->rank_i)?(double *)Bij:(double *)Bkj;
    MPI_Request bkj_send_request, bkj_recv_request;

    MPI_Isend(Bkj_current, grid->n*grid->n, MPI_DOUBLE, grid->process_above, 1, grid->communicator_row, &bkj_send_request);
    MPI_Irecv(next_Bkj, grid->n*grid->n, MPI_DOUBLE, grid->process_under, 1, grid->communicator_row, &bkj_recv_request);

    double *Aik_current = (k==grid->rank_j)?(double *)Aij:(double *)Aik;
    MPI_Bcast((double *)Aik_current, total_size, MPI_DOUBLE, k, grid->communicator_row);

    // Sequential multiplication on block using dgemm   Cij += Aik*Bkj
    dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, size, size, size, 1.0, (const double *)Aik_current, size, (const double *)Bkj_current, size, 1.0, Cij, size);

    MPI_Request request[] = {bkj_send_request, bkj_recv_request};
    MPI_Waitall(2, request, MPI_STATUSES_IGNORE);

    double *tmp = Bkj;
    free_B = Bkj;
    Bkj = next_Bkj;
  }
  //free(Aik);
  //free(free_B);
  //free(next_Bkj);
  return;
}
