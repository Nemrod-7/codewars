int nbDig(int n, int d) {

  int i, k = 0, cnt = 0;

  if (d == 0) cnt++;

  for (i = 0; i <= n; ++i) {
    k = (i * i);
      while ( k > 0) {
        if (k % 10 == d) cnt++;
          k /= 10;
      }
  }

return cnt;
}
