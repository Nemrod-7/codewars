#include <string>
#include <algorithm>

std::string lcs (const std::string &a, const std::string &b) {
    const int m = a.size(), n = b.size();
    int i , j, index, score[m + 1][n + 1];

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                score[i][j] = 0;
            else if (a[i - 1] == b[j - 1])
                score[i][j] = score[i - 1][j - 1] + 1;
            else
                score[i][j] = std::max(score[i - 1][j], score[i][j - 1]);
        }
    }

    index = score[m][n];
    i = m, j = n;
    std::string sub(index, 0);

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
