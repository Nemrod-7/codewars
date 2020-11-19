#define ABSOLUTE(x) ((x < 0) ? (-x) : (x))

int intRac(long long n, long long guess) {

  long long prev = 0;
  int count = 0;

  while (ABSOLUTE((guess - prev)) >= 1) {
    prev = guess;
    guess = (guess + n / guess) / 2;
    count++;
  }

return count;
}
