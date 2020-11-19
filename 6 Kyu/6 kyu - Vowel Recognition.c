#include <stdbool.h>
#include <string.h>

#define COUNT_SUB ((len - index) * (index + 1))

bool is_vowel (char c) {

    char vowels[] = "aeiouAEIOU";
    unsigned short int next = 10;

    while (next-->0)
        if (c == vowels[next])
            return true;

    return false;
}
unsigned long long vowel_recognition(const char *string) {

    const int len  = strlen (string);
    int index = len;
    unsigned long long int count = 0;

    while (index-->0)
        if (is_vowel (string[index]))
            count += COUNT_SUB;

  return count;
}
