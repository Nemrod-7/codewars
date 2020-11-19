#include <stdlib.h>

#define IS_DIGIT(x) (((x) >= '0') && ((x) <= '9'))
#define IS_POINT(x) ((x) == '.')
#define IS_NULL(x) ((x) == '0')
#define IS_GARBAGE(x) (!IS_DIGIT(x) && !IS_POINT(x))

typedef unsigned short int usint_t;

int is_valid_ip(const char * addr) {

  int i;
  usint_t ipv4[4] = {0,0,0,0};
  usint_t dot = 0, cnt = 0;
  char temp[4];

  i = 0;
  while (*addr != '\0') {
    if (IS_GARBAGE(*addr)) return 0;
      temp[i++] = *addr;
        if (IS_POINT(*addr)) {
            if (IS_NULL(temp[0])) return 0;
            if ((ipv4[cnt++] = atoi (temp)) > 255) return 0;
            i = 0;
            dot++;
    }
    addr++;
  }
  if (dot != 3) return 0;

  return 1;
}
