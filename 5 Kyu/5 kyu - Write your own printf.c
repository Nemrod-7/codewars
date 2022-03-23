#include <stdarg.h>
#include <stdlib.h>

#define STARTBRACE '{'
#define ENDBRACE '}'

union u_tag{
    int ival;
    char *cval;
};

int length (const char *src) {
    int count = 0;
    while (*src++) count++;
    return count;
}
void reverse (char *src) {

    int tmp, i = 0, j = length (src) - 1;

    while (i < j) {
        tmp = src[i];
        src[i] = src[j];
        src[j] = tmp;
        i++, j--;
    }
}
int itoa(int num, char *out) {

    int sign, next = 0;

    if ((sign = num) < 0)
        num = -num;

    do {
        out[next++] = num % 10 + '0';
    }
    while ((num /= 10) > 0);

    if (sign < 0)
        out[next++] = '-';

      out[next] = '\0';
      reverse (out);
    return next;
}
char *mr_asprintf (const char *src,...) {

    va_list list;
    va_start (list,src);
    union u_tag data;
    int next = 0;
    char valtemp[15];
    int allc = length(src);
    char *buffer = malloc (allc * sizeof (char));

    do {
        if (*src == STARTBRACE && *(src + 2) == ENDBRACE) {
            src++;
            switch (*src) {
                case 'i':
                    data.ival = va_arg(list,int);
                    allc += itoa (data.ival,valtemp);
                    for (int i = 0; valtemp[i] != '\0'; ++i)
                        buffer[next++] = valtemp[i];
                    break;
                case 's':
                    data.cval = va_arg(list,char *);
                    allc += length(data.cval);
                    for (int i = 0; data.cval[i] != '\0'; ++i)
                        buffer[next++] = data.cval[i];
                    break;
                default:
                    src++;
          }
          buffer = realloc (buffer,allc);
          src++;
      }
      else
        buffer[next++] = *src;
  }
  while (*src++ != NULL);

  va_end (list);

  return buffer;
}
