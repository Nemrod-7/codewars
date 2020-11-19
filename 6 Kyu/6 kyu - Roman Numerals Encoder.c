#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_UPPER(x) (((x) >= 'A') && ((x) <= 'Z'))
#define NUMERAL ((n % mod)/(mod/10))

typedef const char * Link;

char * clean (char *src);

char *solution(int n) {

  const char num[10][5]={".","I","II","III","IV","V","VI","VII","VIII","IX"};
  const char dec[10][5]={".","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"};
  const char cen[10][5]={".","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"};
  const char mil[10][6]={".","M","MM","MMM","MMMM","VM","VIM","VIIM","VIIIM","IXM"};
  char *res = malloc (64 * sizeof(char));
  int next = 0, mod = 10000, i, j;
  int stack[4]={0,0,0,0};

  do {
    stack[next++] = NUMERAL;
  }
  while ((mod /= 10) > 1);

  Link roman[4] = {num[stack[3]],dec[stack[2]],cen[stack[1]],mil[stack[0]]};

  while (next--> 0)
    strcat (res,roman[next]);

  return clean (res);
}

char* clean (char *src) {
  int i,j;
  for (i = 0, j = 0; src[i] != '\0'; ++i)
    if (IS_UPPER(src[i]))
      src[j++] = src[i];
  src[j] = '\0';
  return src;
}
