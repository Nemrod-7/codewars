long zeros(long n) {

  int power = 5, cnt = 0;

  while (power <= n) {
    cnt += n / power;
    power *= 5;
  }

  return cnt;
}
