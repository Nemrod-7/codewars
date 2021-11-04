#include <stdlib.h>
#include <string.h>

#define MAX(a,b) (((a) > (b) )? (a) : (b))

char *lcs (const char *a, const char *b) {
    const int m = strlen(a), n = strlen(b);
    int i , j, index, score[m + 1][n + 1];

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                score[i][j] = 0;
            else if (a[i - 1] == b[j - 1])
                score[i][j] = score[i - 1][j - 1] + 1;
            else
                score[i][j] = MAX(score[i - 1][j], score[i][j - 1]);
        }
    }
    index = score[m][n];
    i = m, j = n;
    char *sub = malloc (index);
    sub[index] = '\0';

    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            sub[--index] = a[i - 1];
            i--, j--;
        } else if (score[i - 1][j] > score[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    return sub;
}
