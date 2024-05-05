#include <stdio.h>

#define DELTA(N) (((N) &1) ? -1 : 1)

void showmatrix(const int n, const int mat[n][n]) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("[%i]",mat[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

int determinant (const size_t n, const int mat[n][n]) {
    if (n == 1) return mat[0][0];
    if (n == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    const int n1 = n - 1;
    int sum = 0;

    for (int i = 0; i < n; i++) {
        int an = mat[0][i], sub[n1][n1];

        for (int j = 0; j < n1; j++) {
            for (int k = 0, ix = 0; k < n; k++) {
                if (k != i) {
                    sub[j][ix++] = mat[j + 1][k];
                }
            }
        }

        sum += an * determinant (n1, sub) * DELTA(i);
    }

    return sum;
}


int main () {

    //determinant(n, mat);

    /* a b c a b
     * d e f d e
     * g h i g h
     * 
     * {0,0},{1,1},{2,2} | {0,0},{1,1},{2,2}
     * {1,0},{2,1},{3,2} | {1,0},{2,1},{0,1}
     * {2,0},{3,1},{4,2} | {2,0},{0,1},{1,2}
     */ 

    //test();


}
