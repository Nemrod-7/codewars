#include <time.h>

char* dateNbDays(double a0, double a, double p) {

  struct tm date = {0};
  date.tm_year = 116, date.tm_mon = 0, date.tm_mday = 1;
  char *buffer = malloc (11 * sizeof(char));
  int cnt = 0;

    do {
      a0 += a0 * (p / 36000);
      cnt++;
    }
    while (a > a0);

    date.tm_mday += cnt;
    mktime (&date);
    strftime(buffer, 11, "%Y-%m-%d", &date);

return buffer;
}
