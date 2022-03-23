#include <stddef.h> /* NULL */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define GETNUM(num,x) ((num) = (num) * 10 + (x) - '0')

int getnum (const char *src) {
  int num = 0;
  for (int i = 0; src[i] != '\0';++i)
      if (isdigit (src[i]))
          GETNUM(num,src[i]);

  return num;
}
void format (char *src) {

  int i = strlen(src);
  src[i] = ' ';
  src[++i] = '\0';
  if (src[0] == ' ')
      for (i = 0; src[i] != '\0';++i) {
          src[i] = src[i + 1];
      }
}
char *gettext (char *src) {

  char *pch = strstr (src,"goto");
  static int gotoline = 0;
  int numline = 0, i = 0;

  if (pch)
      gotoline = getnum(pch);
  else {
    char *line = malloc (strlen (src) * sizeof (char));
    *line = NULL;

      do {
          if (isdigit (*src)) {
              GETNUM(numline,*src);
          } else {
              if (gotoline == 0) {
                  line[i++] = *src;
              }
              if (gotoline != 0)
                  if (numline == gotoline) {
                      gotoline = 0;
                  }
          }
      } while (*src++);

      format (line);

    return line;
  }
  return NULL;
}
char *parse (const char *src) {

  int j = 0;
  char buffer[64], *ptr;
  char *res = malloc (strlen (src) * sizeof (char));
  *res = NULL;
  do {

    if (*src == '\n') {
        buffer[j] = '\0';
        j = 0;
        if ((ptr = gettext(buffer)) != NULL){
            strcat (res,ptr);
          }
    }
    else {
        buffer[j++] = *src;
    }
  }
  while (*src++);

  res[strlen(res) - 1] = '\0';

  return res;
}
