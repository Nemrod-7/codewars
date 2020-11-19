#include <cstdlib>

#define BUFFSIZE 1024
#define BUFFER 30000
#define MID BUFFER/2
#define CODE *(code + index)

unsigned loop (const char *code, unsigned index) {
    int pile = 0;
    const bool fwd = CODE == '[' ? true : false;

    while (CODE) {
       if (CODE == '[') pile++;
       if (CODE == ']') pile--;
       if (pile == 0) return index;

       fwd == true ? index++ : index--;
    }
}
char *boolfuck (const char *code, const char *input) {

  char *output = new char [BUFFSIZE], *byte = output;
  unsigned index = 0, bit = 0, bin = 0;
  bool tape[BUFFER] = {0}, *data = &tape[MID] ;

  while (CODE) {
      switch (CODE) {
          case '+' : *data = ((*data == 0) ? 1 : 0); break;
          case ',' : if (bin == 8)
                        input++, bin = 0;

                     *data = *input >> bin++ &1;
                     break;

          case ';' : if (bit == 8)
                        byte++, bit = 0;

                     *byte += *data << bit++;  // little endian
                     break;

          case '<' : data--; break;
          case '>' : data++; break;
          case '[' : if (*data == NULL) index = loop (code,index); break;
          case ']' : if (*data != NULL) index = loop (code,index); break;
          default : break;
      }
      index++;
    }

  while (bit < 8)
        *byte += 0 << bit++;

    *(byte + 1) = '\0';

  return output;
}
