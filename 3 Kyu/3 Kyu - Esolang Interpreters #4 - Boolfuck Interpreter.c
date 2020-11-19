#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFSIZE 1024
#define BUFFER 30000
#define MID BUFFER/2
#define CODE *(code + index)

unsigned loop (char *code, unsigned index) {
    int pile = 0;
    const bool fwd = CODE == '[' ? true : false;

    while (CODE) {
       if (CODE == '[') pile++;
       if (CODE == ']') pile--;
       if (pile == 0) return index;

       fwd == true ? index++ : index--;
    }
    return false;
}
char* boolfuck (char *code, char *input) {
    char *output = calloc (BUFFSIZE , sizeof (char)), *byte = output;
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
