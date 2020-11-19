#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGIT 1000

char *factorial(int n) {

  if (n < 0)
      return "";

  size_t x, i, tmp;
  size_t next = 0, carry = 0, ndigits = 1, buffer[MAX_DIGIT] = {1};

  for (x = 2; x <= n; x++) {

      for (i = 0; i < ndigits; ++i) {
          tmp = buffer[i] * x;
          buffer[i] = (tmp + carry) % 10;
          carry = (tmp + carry) / 10;

          if (i == ndigits - 1 && carry > 0)
                ndigits++;
      }

}

  char *result = calloc ((ndigits + 1),sizeof (char));

  while (ndigits-->0)
        result[next++] = buffer[ndigits] + '0';

  return result;
}
