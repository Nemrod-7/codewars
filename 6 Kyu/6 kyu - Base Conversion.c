#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

typedef const char * Entry;

char *convert(Entry , Entry , Entry );

char *convert(Entry input, Entry source, Entry target) {

  uint64_t num = 0, converted[64];
  uint8_t SBase = strlen (source), TBase = strlen (target), i, next;
  char *buffer = malloc (64 * sizeof(char));

  if (TBase < 2)
    return 0;

  do {
      #define FORMAT(x) (num += c - (x) + strchr (source,(x)) - source)
      const char c = *input;
      num *= strlen (source);
        if (isupper(c)) FORMAT('A');
        if (islower(c)) FORMAT('a');
        if (isdigit(c)) FORMAT('0');
  }
  while(*++input);

  if (num == 0) {
    buffer[0] = target[0];
    buffer[1] = '\0';
    return buffer;
  }

  next = 0;
  while (num > 0) {
    converted[next++] = num % TBase;
    num /= TBase;
  }

  buffer[next] = '\0', i = 0;
  while (next-->0)
    buffer[i++] = target[converted[next]];

    return buffer;
}
