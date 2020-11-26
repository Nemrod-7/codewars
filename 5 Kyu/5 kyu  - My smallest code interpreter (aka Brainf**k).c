#include <stdlib.h>
#include <stdbool.h>

#define CODE *(code + index)
#define BUFFSIZE 1024
#define OVERFLOW 255

unsigned find_next (const char *code,int pos) {
    int balance = 0;
    unsigned short index = pos;
    const bool fwrd = CODE == '[' ? true : false;

    while (CODE) {
        if (CODE == '[') balance++;
        if (CODE == ']') balance--;
        if (balance == 0) return index;

        fwrd == true ? index++ : index--;
    }
    return pos;
}



void brainfuck(const char *code, const char *input, char *output) {

  unsigned short index = 0,in = 0, out = 0;
  //unsigned char *output = malloc (BUFFSIZE * sizeof (char));
  unsigned char tape[BUFFSIZE] = {0}, *ptr = tape;

  while (CODE) {
      switch (CODE) {
          case '>': ptr++; break;
          case '<': ptr--; break;
          case '+': *ptr == OVERFLOW ? *ptr = '\0' : (*ptr)++; break;
          case '-': *ptr == '\0' ? *ptr = OVERFLOW : (*ptr)--; break;
          case '.': output[out++] = *ptr; break;
          case ',': *ptr = input[in++]; break;
          case '[': if (*ptr == '\0') index = find_next (code,index); break;
          case ']': if (*ptr != '\0') index = find_next (code,index); break;
    }
    index++;
 }

  output[out] = '\0';
  //return output;
}
