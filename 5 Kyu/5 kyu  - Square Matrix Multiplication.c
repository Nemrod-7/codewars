#include <stdio.h>
#include <stdlib.h>

int **matrix_multiplication(int **a, int **b, int n) {

  int c,d,e;
  int **res = malloc (n * sizeof(int *));

  for (c = 0; c < n; ++c) {
    res[c] = malloc (n * sizeof(int *));
    for (d = 0; d < n; ++d) {
      res[c][d] = 0;
      for (e = 0; e < n; ++e) {
          res[c][d] += (a[c][e] * b[e][d]);
      }
    }
  }

  return res;
}
