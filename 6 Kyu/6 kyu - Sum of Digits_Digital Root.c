int digital_root (int num) {

  int dig = 0;

  if (num < 10)
    return num;

   while (num > 0 ) {
    dig += num % 10;
    num /= 10;
  }

  return digital_root(dig);
}
