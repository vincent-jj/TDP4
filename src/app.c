#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "dispatch.h"
#include "fox.h"
#define ERROR
#define DEFAULT_SIZE 10

int main(void){
  struct matrix A,B;
  //if(A.size != B.size){perror("Matrix created do not have the same size\n");return EXIT_FAILURE;}
  struct matrix C;
  struct matrix tmp_A, tmp_B, tmp_C;

  double t_init,t_final;
  struct grid grid;

  MPI_Comm communicator;
  int nb_row_proc;
  int nb_tot_proc; // MPI_Comm_size
  int rank; // MPI_Comm_rank
  int nb_per_block;
  // int master = 0;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_tot_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(communicator_dispatch(nb_tot_proc, &nb_row_proc, &communicator, &rank)==EXIT_FAILURE) {MPI_Finalize();perror("Error trying to dispatch communicator on grid\n");return EXIT_FAILURE;};

  init_fox_mult_matrix(nb_row_proc, &communicator, &grid, rank);
  if(!rank){
    init_random_matrix(&A, DEFAULT_SIZE, nb_row_proc);
    init_random_matrix(&B, DEFAULT_SIZE, nb_row_proc);
    init_random_matrix(&C, DEFAULT_SIZE, nb_row_proc);
    nb_per_block = A.size/nb_row_proc;
  }

  // Bcast size of row and column in a block
  MPI_Bcast(&nb_per_block, 1, MPI_INT, 0, communicator);

  // Initialize temporary matrix in each process
  init_random_matrix(&tmp_A, nb_per_block, 1);
  init_random_matrix(&tmp_B, nb_per_block, 1);
  init_random_matrix(&tmp_C, nb_per_block, 1);

  t_init = MPI_Wtime();

  matrix_dispatch_process(nb_row_proc, nb_per_block, &communicator, A.table, tmp_A.table, MPI_SCATTER);
  matrix_dispatch_process(nb_row_proc, nb_per_block, &communicator, B.table, tmp_B.table, MPI_SCATTER);

  mult_fox_mpi(nb_per_block, tmp_A.table, tmp_B.table, tmp_C.table, &grid, &communicator);

  matrix_dispatch_process(nb_row_proc, nb_per_block, &communicator, tmp_C.table, C.table, MPI_GATHER);

  MPI_Barrier(MPI_COMM_WORLD);
  if(!rank){
    t_final = MPI_Wtime() - t_init;
    printf("Number processes : %d -- Execution time : %lf \n", nb_tot_proc, t_final);
  }
  free(tmp_A.table);
  free(tmp_B.table);
  free(tmp_C.table);
  if(!rank) {print_matrix(&A, stdout);print_matrix(&B, stdout);print_matrix(&C, stdout); free(A.table); free(B.table); free(C.table);}

  finalize_fox_mult(&grid);
  MPI_Comm_free(&communicator);

  MPI_Finalize();
  return EXIT_SUCCESS;
}

/*
int main(int argc, char*argv[]){
  struct matrix A, B;

  // Reading args
  switch(argc){
    case 0:
      create_random_matrix(&A, DEFAULT_SIZE, 1);
      break;
    case 1:
      break;
    case 2:

      break;
    default:
      printf("Too many arguments. Please enter 0, 1 or 2 arguments corresponding to path to matrix files\n");
      return EXIT_FAILURE;
      break;
  }
  return EXIT_SUCCESS;
}
*/
