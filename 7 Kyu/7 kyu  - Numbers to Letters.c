#include <stdlib.h>

char *switcher(const char **codes, size_t count) {

  int i;
  char *c = malloc (count * sizeof(char));

    for (i = 0; i < count; ++i)
    c[i] = atoi(codes[i]);

    for (i = 0; i < count; ++i)
      switch (c[i]) {
        case 29: c[i] = ' '; break;
        case 28: c[i] = '?'; break;
        case 27: c[i] = '!'; break;
        case 26: c[i] = 'a'; break;
        case 25: c[i] = 'b'; break;
        case 24: c[i] = 'c'; break;
        case 23: c[i] = 'd'; break;
        case 22: c[i] = 'e'; break;
        case 21: c[i] = 'f'; break;
        case 20: c[i] = 'g'; break;
        case 19: c[i] = 'h'; break;
        case 18: c[i] = 'i'; break;
        case 17: c[i] = 'j'; break;
        case 16: c[i] = 'k'; break;
        case 15: c[i] = 'l'; break;
        case 14: c[i] = 'm'; break;
        case 13: c[i] = 'n'; break;
        case 12: c[i] = 'o'; break;
        case 11: c[i] = 'p'; break;
        case 10: c[i] = 'q'; break;
        case  9: c[i] = 'r'; break;
        case  8: c[i] = 's'; break;
        case  7: c[i] = 't'; break;
        case  6: c[i] = 'u'; break;
        case  5: c[i] = 'v'; break;
        case  4: c[i] = 'w'; break;
        case  3: c[i] = 'x'; break;
        case  2: c[i] = 'y'; break;
        case  1: c[i] = 'z'; break;
  }
  c[i] = '\0';

return c;
}
