#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define POS(x) ((x) - 'a' + 1)

unsigned find_the_key
    (const char *msg, const unsigned char *code, size_t n) {

  bool match(const char *s,const unsigned char *dcrypt, unsigned k) ;

  int start = abs(POS(msg[0]) - code[0]);
  int i, num, next = 0;

  unsigned key = 0;

  for (i = 0; i < n ; ++i) {
    num = abs(POS(msg[i]) - code[i]);
    key = 10 * key + num;

    if (match (msg,code,key) == true)
      return key;

  }

  return NULL;
}
bool match(const char *s,const unsigned char *dcrypt, unsigned k) {

  char key[16], *code = malloc (strlen(s) * sizeof(char));
  int i, next = 0;
  bool flag = true;

  sprintf (key,"%u",k);

  for (i = 0; s[i] != NULL; ++i) {
    if (key[next] == NULL)
      next = 0;

    code[i] = POS(s[i]) + (key[next++] - '0');

    if (dcrypt[i] != code[i])
      flag = false;
  }
  return flag;
}
