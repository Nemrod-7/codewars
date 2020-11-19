const char * getIssuer (const long number) {

  char mark;

  int cnt = 0;
  long n = number;

  while (n != 0) {
      if (n < 10000 && n == 6011) mark = 'D';
      if (n < 100) {
        if (n == 34 || n == 37) mark = 'A';
        if (n >= 51 && n <= 55) mark = 'M';
      }
      if (n < 10) {
        if (n == 4) mark = 'V';
      }
      n /= 10;
      cnt++;
  }

  if (cnt == 15 && mark == 'A')
    return "AMEX";
  if (cnt == 13 && mark == 'V')
    return "VISA";
  if (cnt == 16) {
    if (mark == 'D') return "Discover";
    if (mark == 'V') return "VISA";
    if (mark == 'M') return "Mastercard";
  }

  return "Unknown";
}
