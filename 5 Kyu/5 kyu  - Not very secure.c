#include <stdbool.h>
#include <ctype.h>

bool alphanumeric(const char* strin) {

  if (!isalnum (strin[0]))
    return false;

   for (int i = 1; strin[i] != '\0'; ++i)
     if (!isalnum (strin[i]))
       return false;

  return true;
}
