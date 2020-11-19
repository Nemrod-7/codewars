#include <ctype.h>

int getAge(const char *inputString) {

int age = 0;

  age = age * 10 + (inputString[0] - '0');

return age;
}
