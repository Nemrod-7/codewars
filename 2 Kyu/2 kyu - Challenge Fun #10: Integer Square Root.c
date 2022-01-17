#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DIGIT 1000
#define TO_DIG(x) ((x) - '0')
#define TO_CHAR(x) ((x) + '0')
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

void swap (char *a, char *b) {

    char *tmp = a;
    a = b;
    b = tmp;
}

void reverse (char *src) {

  int tmp, i = 0, j = strlen (src) - 1;

  while (i < j) {
    tmp = src[i];
    src[i] = src[j];
    src[j] = tmp;
    i++, j--;
  }
}
int ismaller (const char *a, const char *b) {
    int lena = strlen(a), lenb = strlen(b);
    if (lenb > lena) return 2;
    if (lenb < lena) return 0;

    for (size_t i = 0; i < lenb; i++) {
        if (b[i] > a[i]) return 2;
        if (b[i] < a[i]) return 0;
    }
    return 1;
}
char *add (const char *a, const char *b) {

    int i = strlen (a), j = strlen (b), size = MAX (i,j);
    int rem = 0, num;
    char *os = malloc ((size + 1) * sizeof (char)), *ptr = os;

    while (size-->0) {
        num = (i > 0 ? TO_DIG (a[--i]) : 0) + (j > 0 ? TO_DIG (b[--j]) : 0) + rem;
        rem = num / 10;
        *ptr++ = TO_CHAR (num % 10);
    }

    if (rem > 0) *ptr++ = TO_CHAR (rem);
    *ptr = '\0';
    reverse (os);
    // printf ("%s", os);
    return os;
}
char *mul (const char *a, const char *b) {

    int lena = strlen (a), lenb = strlen(b), ans[MAX_DIGIT] = {0};
    int i,j, size;
    char arra[lena + 1], arrb[lenb + 1];

    if ((lena == 1 && a[0] == '0')|| (lenb == 1 && b[0] == '0'))
        return "0";

    strcpy (arra,a), strcpy (arrb,b);
    reverse (arra), reverse (arrb);

    for (i = 0; i < lena; ++i) {
        for (j = 0; j < lenb; ++j) {
            int num = TO_DIG (arra[i]) * TO_DIG (arrb[j]);
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
char *sub (const char *x, const char *y) {

  int carry = 0;
  _Bool minus = 0;
  char *a, *b;

  if (ismaller (x, y) == 2) {
      a = strdup (y), b = strdup (x);
      minus = 1;
  } else {
      a = strdup (x), b = strdup (y);
  }
  int lena = strlen (a), lenb = strlen (b), index = lena - lenb;
  reverse (a), reverse (b);
  char *os = malloc (lena * sizeof (char)), *ptr;

  ptr = &b[lenb];

  while (index-->0) *ptr++ = '0';

  ptr = os;
  for (size_t i = 0; i < lena; i++) {
      int dig = a[i] - b[i] - carry;

      if (dig < 0) {
          dig += 10, carry = 1;
      } else {
          carry = 0;
      }

      *ptr++ = TO_CHAR (dig);
  }

  ptr--;
  while (*ptr == '0') *ptr-- = '\0';
  if (minus) *++ptr = '-';
  reverse (os);

  return os;
}

char *trim (char *s) {
    char *ptr = s;
    while (*ptr == '0') ptr++;
    return ptr;
}
char *integerSquareRoot(char *src) {
    char *root = "0", *carr = "0";
    int size = strlen(src);
    char num[1200];// = malloc ((size + 1) * sizeof (char));
    char *ref = malloc (128);
    if (size % 2) {
        num[0] = '0';
    }
    strcat (num, src);

    for (size_t i = 0; i < size; i += 2) {
        char *x = "0", *n = mul (root, "20");
        sprintf (ref, "%s%c%c",carr, num[i], num[i + 1]);
        ref = trim (ref);
        char *rr = mul (add (n, x), x);

        while (ismaller (mul (add (n, x), x), ref) > 0) {
            x = add (x,"1");
        }
        x = sub (x, "1");
        //printf ("[%s][%s]\n",mul (add (n, x), x), ref);
        carr = sub (ref, mul (add (n, x), x));
        root = add (mul (root, "10"), x);
        printf ("[%s][%s]\n",carr, root);
        //printf ("[%s]", root);
    }

   return NULL;
}

int main () {

    integerSquareRoot ("1522756");
    return 0;
}
