/*
  Global libraries included
 */
#include <stdio.h>
#include <stdlib.h>
/*
  Local libraries included
 */
#include "util.h"

#define RANDOM_FLOAT(max) ((float)(rand())/(float)(RAND_MAX)*2-1.0)*(max)
#define MAX_FLOAT 10.0

int random_matrix(struct matrix* mat, int nb_row){
  if (mat == NULL || mat->table == NULL)
    return EXIT_FAILURE;

  srand(time(NULL));

  int i,j;
  for(i=0; i<nb_row; i++){
    const int ind = i*mat->size;
    for(j=0; j<nb_row; j++){
     mat->table[ind+j] = RANDOM_FLOAT(MAX_FLOAT);
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


int init_random_matrix(struct matrix* mat, int nb_row, int nb_proc_row){
  if (mat == NULL)
    return EXIT_FAILURE;

  srand(time(NULL));

  // determination of the new matrix size
  int count = nb_row;
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

  // complete with random float
  int i,j;
  for(i=0; i<count; i++){
    for(j=0; j<count; j++){
     mat->table[i*mat->size+j] = RANDOM_FLOAT(MAX_FLOAT);
    }
  }
  return EXIT_SUCCESS;
}


int print_matrix(struct matrix* mat, FILE* file){
  int i;
  // print the size
  fprintf(file, "%d\n", mat->size);
  // print the values
  for(i = 0; i<(mat->size*mat->size); i++){
    fprintf(file, "%f ", mat->table[i]);
    if (((i+1) % mat->size) == 0)
      fprintf(file, "\n");
  }
  return EXIT_SUCCESS;
}
