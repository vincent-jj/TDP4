#ifndef __UTIL_H__
#define __UTIL_H__

// WARNING Matrix is row major !
struct matrix{
  int size;
  double *table;
};

int random_matrix(struct matrix *m,
                   int nb_row);
int load_matrix(struct matrix *m,
		int nb_proc_row,
		char *filename);
int init_random_matrix(struct matrix *m,
                       int nb_row,
                       int nb_proc_row);
int print_matrix(struct matrix *m, FILE *file);

#endif // __UTIL_H__
