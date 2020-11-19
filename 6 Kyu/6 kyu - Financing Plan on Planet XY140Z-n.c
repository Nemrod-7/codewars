unsigned long long finance(unsigned long long n) {

  int i;
  unsigned long long sum = 0;

  for (i = 0; i <= n; ++i) {
    sum += i * (n + 2);
  }
return sum;
}
