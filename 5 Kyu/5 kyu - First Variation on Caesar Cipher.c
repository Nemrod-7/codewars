#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define CEIL(a, b) (((a) / (b)) + (((a) % (b)) > 0 ? 1 : 0))

int range (char *src);
char** movingShift(char* strng, int shift);
char* demovingShift(char** s, int shift);
char *ciphmodule (char *src,int shift,char mode);

int range (char *src) {

  int length = strlen (src);
  int mean4 = length / 4, mean5 = CEIL(length , 5);
  int last1 = (length - mean4 * 4), last2 = (length - mean5 * 4);
  return MAX(mean5,MAX (last1,last2));
}

char** movingShift(char *strng, int shift) {

  int i = 0, cnt = 0;
  const int edge = range (strng);
  printf ("%s\n",strng);

  printf ("%i\n",edge);

  char temp[edge + 1];

  strng = ciphmodule(strng,shift,'e');
  char **res = malloc (5 * sizeof(char *));
  for (i = 0; i < 5; ++i)
    res[i] = malloc ((edge + 1) * sizeof(char));

  i = 0;
  do {
      temp[i++] = *strng;
        if (i == edge) {
            temp[i] = '\0';
            strcpy (res[cnt++], temp);
            i = 0;
        }
  }
  while (*strng++);
    temp[i] = '\0';
    if (edge != 4 && edge != 5 && edge != 6 && edge != 7 && edge != 9 && edge != 12 && edge != 13) strcpy (res[cnt], temp);

  return res;
}

char* demovingShift(char** s, int shift) {

  int i;
  const int size = strlen(*s);
  char *res = malloc ( 5 * size * sizeof(char));
  res[0] = '\0';

  for (i = 0; i < 5; i++) {
     strcat (res,s[i]);
   }
    res = ciphmodule(res,shift,'d');

    return res;
}

char *ciphmodule (char *src,int shift,char mode) {

  int i;
  const int leng = strlen(src);
  char *res = malloc (leng * sizeof(char));

  i = 0;
  do {
    if (ispunct(*src) || isspace(*src) || isdigit (*src))
      res[i] = *src;
    else {
      int next = (mode == 'd') ? (*src - shift) : (*src + shift);
      #define OP (mode == 'd') ? (next < 97) : (next > 65)
      #define CASE(x) (isupper(x)) ? 'A' : 'a'
      {
        char case_mode = CASE(*src);
        int crypt = (next - case_mode + 26 * leng) % 26 + case_mode;
        res[i] = (OP) ? crypt : next;
      }
    }
  shift++, i++;
  }
  while (*src++);
  res[i - 1] = '\0';

  return res;
}
