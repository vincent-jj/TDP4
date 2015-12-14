#ifndef __FOX_H__
#define __FOX_H__

#include <mpi.h>

/*
  Structure of the grid
@struct grid
-->
  MPI_Comm communicator_row  : Communicator to broadcast to the current rows
  INT rank_i                 : row rank
  INT rank_j                 : column rank
  INT n                      : row's number of block
  INT process_above          : rank of above's process
  INT process_under          : rank of under's process
-->
 */
struct grid{
  MPI_Comm communicator_row;
  int rank_i;
  int rank_j;
  int n;
  int process_above;
  int process_under;
};

/*
  Create the grid for fox multiplication
  @args :
  nb_proc_row is the number of proc per row
  comm is the carthesian communicator
  grid is the grid to init
  rank is the calling process rank
 */
void init_fox_mult_marix(/*IN*/int nb_proc_row,
                         /*IN*/MPI_Comm *communicator,
                         /*OUT*/struct grid *grid,
                         /*IN*/int rank);

/*
  parallel matrix multiplication C = A * B using fox algorithm
  A(i,j), B(i,j) and C(i,j) are the distributed blocks of the matrix
  n is the size of blocks
  comm is the communicator associated to the grid
  grid represents the topology of the commutation grid
  (means how the other blocks are distributed)
 */
void mult_fox_mpi(int n,
                  const double *Aij,
                  const double *Bij,
                  double *Cij,
                  const struct grid * grid,
                  MPI_Comm communicator);

/*
  Frees dynamically allocated ressources for the grid
  (means communicators, topology, etc...)
  WARNING : Distributed matrix(ces) can't be free using this function
 */
int finalize_fox_mult(struct grid *grid);

#endif // __FOX_H__
