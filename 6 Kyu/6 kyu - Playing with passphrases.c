#include <stdlib.h>

#define IS_ALPHA(x) ((x) >= 'a' && (x) <= 'z' || (x) >= 'A' && (x) <= 'Z')
#define IS_DIGIT(x) (((x) >= '0') && ((x) <= '9' ))
#define IS_SPACE(x) (IS_ALPHA (x) == FALSE && IS_DIGIT(x) == FALSE)
#define IS_UPPER(x) (((x) >= 'A') && ((x) <= 'Z'))
#define TO_LOWER(x) (IS_UPPER(x) == TRUE ? (x) - 'A' + 'a': (x))

enum kBool {FALSE,TRUE};

char* playPass(char* s, int n) {

size_t ArrLen (char *source);
  char Reverse (char *source);
  const size_t length = ArrLen(s);
  char *mod = malloc (length * sizeof(s));
  int i = 0;

  do {
    if (IS_SPACE(s[i])) {
      mod[i] = s[i];
    } else {
      } if (IS_DIGIT(s[i])) {
        switch (s[i]) {
          case '0' : mod[i] = '9';break;
          case '1' : mod[i] = '8';break;
          case '2' : mod[i] = '7';break;
          case '3' : mod[i] = '6';break;
          case '4' : mod[i] = '5';break;
          case '5' : mod[i] = '4';break;
          case '6' : mod[i] = '3';break;
          case '7' : mod[i] = '2';break;
          case '8' : mod[i] = '1';break;
          case '9' : mod[i] = '0';break;
          default : mod[i] = s[i];break;
        }
    } else {
      if (IS_ALPHA(s[i])) {
        if ((s[i] + n) > 'Z') {
          mod[i] = s[i] - 26 + n;
        } else {
          mod[i] = s[i] + n;
        }
      }
    }
  }
  while (s[i++] != '\0');
  mod[i - 1] = '\0';

  for (i = 1; i < length; i += 2)
  mod[i] = TO_LOWER(mod[i]);


    Reverse (mod);

return mod;
}
char Reverse (char *source) {

    size_t ArrLen (char *source);
    const size_t length = ArrLen(source);
    int i = 0, j = length - 1;
    char swap;

    while (i < j) {
        swap = source[i];
        source[i] = source[j];
        source[j] = swap;
        i++ , j--;
    }
return *source;
}

size_t ArrLen (char *source) {
  size_t cnt = 0;
  while (*source++) cnt++;
return cnt;
}
