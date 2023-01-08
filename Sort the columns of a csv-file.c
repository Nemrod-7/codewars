#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

extern char* strdup(const char*);
struct _cell {
  char *str;
  int index;
};

struct _matrix {
    int width, height;
    char ***data;
};

int cmpfunc (const void *a, const void *b) {
  struct _cell *s1 = (struct _cell*)a, *s2 = (struct _cell*)b;
  return strcasecmp (s1->str, s2->str);
}

void sort_csv_columns (const char *csv, char *sorted) {

    char *mat[64][64], buff[64], *ptr = sorted;
    size_t width = 0, height = 0;
    size_t i = 0, j, len = strlen (csv);

    for (i = 0; i < len ; ) {
          sscanf (csv + i, "%[^\n]", buff);
          i += strlen (buff) + 1;
          //printf ("[%s]\n", buff);
          char *tok = strtok (buff, ";");
          width = 0;
          while (tok) {
              mat[height][width] = strdup (tok);
              tok = strtok (NULL, ";");
              width++;
          }
          height++;

    }

    struct _cell order[width];

    for (i = 0; i < width; i++) {
        order[i] = (struct _cell) {mat[0][i], i};
    }

    qsort (order, width, sizeof (struct _cell), cmpfunc);

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            ptr += sprintf (ptr,"%s;", mat[i][order[j].index]);
        }
        *(ptr - 1) = '\n';
    }
    *(ptr - 1) = '\0';
}

void do_test(const char *inpt, const char *expected) {
    char *actual = malloc(sizeof(char) * (strlen(inpt) + 2));
    sort_csv_columns (inpt, actual);
    printf ("[%s]\n", actual);
    free(actual);
}
void Test() {

    do_test("myjinxin2015;raulbc777;smile67;Dentzil;SteffenVogel_79\n17945;10091;10088;3907;10132\n2;12;13;48;11",
            "Dentzil;myjinxin2015;raulbc777;smile67;SteffenVogel_79\n3907;17945;10091;10088;10132\n48;2;12;13;11");

    do_test("Captain America;Hulk;IronMan;Thor\nhonorably;angry;arrogant;divine\nshield;greenhorn;armor;hammer\nSteven;Bruce;Tony;Thor",
            "Captain America;Hulk;IronMan;Thor\nhonorably;angry;arrogant;divine\nshield;greenhorn;armor;hammer\nSteven;Bruce;Tony;Thor");
}

int cmpf (const void *a, const void *b) {
  const char *aa = *(const char **)a, *bb = *(const char **)b;
  return strcmp(aa,bb);
}

int main () {

  const char *src = "myjinxin2015;raulbc777;smile67;Dentzil;SteffenVogel_79\n1;2;3;4;5\n1;2;3;4;5";
  
  char *csv = strdup(src);
  int height = 2, width = 1, i = 0;

  while (csv[i++] != '\n')
      if (csv[i] == ';') width++;

  while (csv[i++] != '\0')
      if (csv[i] == '\n') height++;

  char *mat[width][height];// = malloc (sizeof *mat);
  char *tok = strtok(csv, ";\n");

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          mat[x][y] = tok;
          tok = strtok (NULL, ";\n");
      }
  }

  //qsort(mat, width, sizeof **mat, cmpf);

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          char *cell = mat[x][y];
          printf("[%s]", cell);
      }
      printf("\n");
  }

  //Test();

}
