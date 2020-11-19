#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGIT 1000
#define NUM(x) ((x) - '0')

void reverse (char *src) {

  int tmp, i = 0, j = strlen (src) - 1;

  while (i < j) {
    tmp = src[i];
    src[i] = src[j];
    src[j] = tmp;
    i++, j--;
  }
}
char *multiply(char *a, char *b) {

  int lena = strlen (a), lenb = strlen(b), ans[MAX_DIGIT] = {0};
  int i,j, size;
  char arra[lena + 1], arrb[lenb + 1];

  if ((lena == 1 && a[0] == '0')|| (lenb == 1 && b[0] == '0'))
      return "0";

  strcpy (arra,a), strcpy (arrb,b);
  reverse (arra), reverse (arrb);

  for (i = 0; i < lena; ++i) {
      for (j = 0; j < lenb; ++j) {
          int num = NUM (arra[i]) * NUM (arrb[j]);
          ans[i + j] += num;
          ans[i + j + 1] = ans[i + j + 1] + ans[i + j] / 10;
          ans[i + j] %= 10;
      }
  }

  size = i + j;
  char *result = calloc (size + 1,sizeof(char));
  while (ans[size] == 0) size--;

  i = 0;
  do {
      result[i++] = ans[size] + '0';
  } while (size-->0);

    return result;
}
