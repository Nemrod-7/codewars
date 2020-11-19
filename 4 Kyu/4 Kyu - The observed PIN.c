#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char pad[10][5] = {"08","124","2135","326","4157","52468","6359","748","80579","968"};

const char **get_pins(const char *observed, size_t *count);

int main () {

    size_t count;
    const char** pins = get_pins("369", &count);


    for (int i = 0; i < count; i++)
        printf ("%s ", pins[i]);

    return 0;
}

char ***mk_cube (size_t x, size_t y, size_t z) {

    char ***datacube = malloc ((x) * sizeof (char **));

    for (int i = 0; i < x; ++i) {
        datacube[i] = malloc (y * sizeof (char **));
        for (int j = 0; j < y; ++j)
            datacube[i][j] = malloc (z * sizeof (char *));
    }

    return datacube;
}
const char **get_pins(const char *observed, size_t *count) {

      *count = 1;

      int len = strlen(observed), index, top = 1, next;

      for (index = 0; observed[index] != '\0'; index++) {
          int digit = observed[index] - '0';
          *count *= strlen (pad[digit]);
      }

      char ***stack = mk_cube (len, *count, 6);

      index = 0;
      while (len--> 0) {
          int num = observed[index] - '0', size = strlen (pad[num]);
  	      next = 0;
          for (int comb = 0 ; comb < top; comb++)
              for (int dig = 0; dig < size; dig++)
                  if (top <= 1) {
                      sprintf (stack[index][next++],"%c", pad[num][dig]);
  		            } else {
                      sprintf (stack[index][next++],"%s%c", stack[index - 1][comb], pad[num][dig]);
                  }
          if (index > 0) free (stack[index - 1]);
          top *= size;
          index++;
      }

      return stack[index - 1];
}
void free_pins(const char ** pins) {
    free (pins);
}
