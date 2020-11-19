int findSum (int n) {

int sum = 0;

  do {
    if (n % 3 == 0 || n % 5 == 0)
      sum += n;
  }
  while (n-- > 0);
return sum;
}
