/*
  Global libraries included
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
/*
  Local libraries included
 */
#include "util.h"
//Uncomment the next line if you want to get shorter values when you try to randomize
// #define RAND_MAX 100

#define MAX_FLOAT 10.0

inline double random_f(int max_value){
  return ((float)(rand())/(float)(RAND_MAX)*2-1.0)*(max_value);
}

int random_matrix(struct matrix* mat, int nb_row){
  if (mat == NULL || mat->table == NULL)
    return EXIT_FAILURE;

  srand(time(NULL));

  int i,j;
  for(i=0; i<nb_row; i++){
    const int ind = i*mat->size;
    for(j=0; j<nb_row; j++){
     mat->table[ind+j] = random_f(MAX_FLOAT);
    }
  }
  return EXIT_SUCCESS;
}


int load_matrix(struct matrix* mat, int nb_proc_row, char* filename){
  FILE* file = fopen(filename, "r");
  if(file == NULL){
    perror("Cannot open matrix file");
    return EXIT_FAILURE;
  }

  int count;
  if (1 != fscanf(file,"%d", &count) || count<0){
    fclose(file);
    return EXIT_FAILURE;
  }

  // determination of the new matrix size
  mat->size = (count/nb_proc_row);
  mat->size *= nb_proc_row;
  if (count%nb_proc_row != 0){
    mat->size +=nb_proc_row;
  }

  mat->table = malloc(sizeof(double)*mat->size*mat->size);
  // nullify extra terms (in order to match with nb_proc_row multiples)
  int k,l;
  for (k=count; k<mat->size; k++){
    for (l=0; l<mat->size; l++){
      mat->table[k*mat->size+l] = 0.0;
    }
  }
  for (k=0; k<count; k++){
    for (l=count; l<mat->size; l++){
      mat->table[k*mat->size+l] = 0.0;
    }
  }

  // load value from file
  int i,j;
  for(i=0; i<count; i++){
    j = 0;
    if(1 != fscanf(file, "\n%lf", &mat->table[i*mat->size+j])){
      fclose(file);
      return EXIT_FAILURE;
    }
    for(j=1; j<count; j++){
      if(1 != fscanf(file, "%lf", &mat->table[i*mat->size+j])){
	      fclose(file);
	      return EXIT_FAILURE;
      }
    }
  }

  double f;
  if(EOF != fscanf(file,"%lf", &f))
    printf("WARNING: file longer than expected\n");
  fclose(file);
  return EXIT_SUCCESS;
}


int init_random_matrix(struct matrix* matrix, int nb_row, int nb_proc_row){
  if (matrix == NULL)
    return EXIT_FAILURE;

  srand(time(NULL));

  // determination of the new matrix size
  // WARNING HERE !! --> be aware that everything woll go wrong if your matrix is too small in comparison with the number of processes !!!
  // It mostly implies that nb row >> nb proc per row !!

  matrix->size = (nb_row/nb_proc_row);
  matrix->size *= nb_proc_row;
  if (nb_row%nb_proc_row != 0){
    matrix->size +=nb_proc_row;
  }

  matrix->table = (double *)malloc(sizeof(double)*matrix->size*matrix->size);

  // nullify extra terms (in order to match with nb_proc_row multiples)
  int k,l;
  for (k=nb_row; k<matrix->size; ++k){
    for (l=0; l<matrix->size; ++l){
      matrix->table[k*matrix->size+l] = 0.0;
    }
  }
  for (k=0; k<nb_row; k++){
    for (l=nb_row; l<matrix->size; l++){
      matrix->table[k*matrix->size+l] = 0.0;
    }
  }

  // complete with random float
  int i,j;
  for(i=0; i<nb_row; i++){
    for(j=0; j<nb_row; j++){
     matrix->table[i*matrix->size+j] = random_f(MAX_FLOAT);
    }
  }
  return EXIT_SUCCESS;
}


int print_matrix(struct matrix* matrix, FILE* fd){
  int i;
  fprintf(fd, "Matrix size :: %d\n\n", matrix->size);
  for(i = 0; i<(matrix->size*matrix->size); i++){
    fprintf(fd, "%f ", matrix->table[i]);
    if (((i+1) % matrix->size) == 0)
      fprintf(fd, "\n");
  }
  return EXIT_SUCCESS;
}
