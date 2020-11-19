#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char *alphabet_position(char *text) {

    char c, *result = malloc (strlen(text) * 2 * sizeof (char));
    char *tmp = result;

    *result = '\0';
    while ((c = *text++)) {
        char num = 0;
        if (isupper(c)) num = c - 64;
        if (islower(c)) num = c - 96;

        if (isalpha (c))
            tmp += sprintf (tmp,"%i ",num);
    }
  result[strlen (result) - 1] = '\0';
  return result;
}
