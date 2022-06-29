#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool is_valid_ISBN_10 (const char *ISBN) {
  if (strlen(ISBN) != 10) return false;
  int sum = 0;
  for (int i = 0; i < 9; i++) {
      if (!isdigit(ISBN[i])) return false;
      sum += ((ISBN[i] - '0') * (i + 1));
  }
  sum += ISBN[9] == 'X' ? 100 : (ISBN[9] - '0') * 10;
  return sum % 11 == 0;
}
