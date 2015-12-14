#ifndef __DISPATCH_H__
#define __DISPATCH_H__

#include <mpi.h>
enum dispatch{MPI_GATHER, MPI_SCATTER};

/*
  Create a new communicator with the number of process
  WARNING : @param nb_proc_row not allocated in the function !
  @param nb_proc_row : number of processors used per row (or column)
  WARNING : @param new_communicator not allocated in the function !
  @param new_communicator : communicator with only processors used to ccalculate
 */
int communicator_dispatch(int nb_proc_tot,
                          int *nb_proc_row,
                          MPI_Comm *new_communicator,
                          int *rank);

/*
  Function scattering and gathering the matrix
  @param nb_proc_row and @param nb_in_block used to calculate sizes of blocks
  to transmit to other processes
  @param communicator : grid communicator
  @param type : gathering or scattering the matrix
 */
int matrix_dispatch_process(int nb_proc_row,
                            int nb_in_block,
                            MPI_Comm *communicator,
                            double *bufs,
                            double *bufr,
                            enum dispatch mpi_type);

#endif // __DISPATCH_H__
