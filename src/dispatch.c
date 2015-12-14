/*
  Global libraries included
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
/*
  Local libraries included
 */
#include "dispatch.h"

/*
    Definition of the prototype of @func(communicator_dispatch) declared in
    dispatch.h
 */
int communicator_dispatch(int nb_proc_tot, 
			  int *nb_proc_row, 
			  MPI_Comm *new_communicator,
			  int *rank){
  /*
    Verification pre processing
  */
  if(nb_proc_row == NULL || new_communicator == NULL){
    if(nb_proc_row == NULL){
      printf("Error in communicator_dispatch : nb_proc_row == NULL => True\n");
    }
    if(new_communicator == NULL){
      printf("Error in communicator_dispatch : new_communicator == NULL => True\n");
    }
    return EXIT_FAILURE;
  }

/* TODO : Check validity of this part */
  // Heavily used variables
  unsigned op, res, one;
  op = (unsigned) nb_proc_tot;
  res = 0;

  *nb_proc_row = (int)res;

  // Creation of the grid communicator
  int dimensions[] = {*nb_proc_row, *nb_proc_row};
  int periods[] = {1,1};

  MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 1, new_communicator);
  if(*new_communicator != MPI_COMM_NULL){
    MPI_Comm_rank(*new_communicator, rank);
  }
  else
    return EXIT_FAILURE;

  // WARNING : some processes might be not necessary to the execution :
  // Cause : nb_proc_tot != perfect square !

  return EXIT_SUCCESS;
}

/*
    Definition of the prototype of @func(matrix_dispatch_process) declared in
    dispatch.h
 */
int matrix_dispatch_process(int nb_proc_row,
			    int nb_in_block, 
			    MPI_Comm *communicator,
			    double *bufs,
			    double *bufr, 
			    enum dispatch mpi_type){
  MPI_Datatype blocktype;
  MPI_Datatype blocktype_two;
  int i, j;

  // Creation of the type representation for matrix block to process
  MPI_Type_vector(nb_in_block, nb_in_block,
                  nb_in_block*nb_proc_row, MPI_DOUBLE, &blocktype_two);
  MPI_Type_create_resized(blocktype_two, 0, sizeof(double), &blocktype);
  MPI_Type_commit(&blocktype);

  // TODO Dispatching processes

  int dispatches[nb_proc_row*nb_proc_row];
  int counting[nb_proc_row*nb_proc_row];

  for(i = 0; i < nb_proc_row ; ++i){
    for(j = 0; j < nb_proc_row ; ++j){
      dispatches[(i*nb_proc_row)+j] = i * nb_in_block * nb_in_block * nb_proc_row + j * nb_in_block;
      counting[(i*nb_proc_row) + j] = 1;
    }
  }

  switch(mpi_type){
    case MPI_SCATTER:
      MPI_Scatterv(bufs, counting, dispatches,
                   blocktype, bufr, nb_in_block*nb_in_block,
                   MPI_DOUBLE, 0, *communicator);
      break;
    case MPI_GATHER:
      MPI_Gatherv(bufs, nb_in_block*nb_in_block,
                  MPI_DOUBLE, bufr, counting,
                  dispatches, blocktype, 0, *communicator);
      break;
    default:
      printf("Unknown type for matrix_dispatch_process.\n Please choose between MPI_GATHER and MPI_GATHER !\n");
      MPI_Type_free(&blocktype);
      return EXIT_FAILURE;
      break;
  }

  MPI_Type_free(&blocktype);

  return EXIT_SUCCESS;
}
