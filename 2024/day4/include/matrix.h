#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>

struct matrix_node {
  char letter;
};

typedef struct matrix_node** Matrix;

void read_matrix(FILE* input);

#endif
