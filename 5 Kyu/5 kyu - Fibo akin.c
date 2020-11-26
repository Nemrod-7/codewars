#include <stdio.h>
#include <stdlib.h>

int u (int n) {
    static int U[100000] = {1,1,1}, k = 3;

    for (; k <= n; ++k)
        U[k] = U[k - U[k - 1]] + U[k - U[k - 2]];

    return U[n];
}
int lengthSupUK(int n, int k) {
    int cnt = 0;

    for (int i = 1; i <= n; i++)
       if (u(i) >= k) cnt++;

    return cnt;
}

int comp(int n) {
    int cnt = 0;

    for (int i = 2; i <= n; ++i)
        if (u(i) < u(i - 1)) cnt++;

    return cnt;
}
int main () {

  u(23);
  u(23);
  u(23);
}
