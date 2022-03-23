#include <stdlib.h>
#include <stdbool.h>

#define BUFFSIZE 1024
#define BUFFER 30000
#define CODE *(sourcecode + index)

int main () {

    return 0;
}
unsigned loop (const char *sourcecode, int pos) {
    int pile = 0;
    unsigned index = pos;
    const bool fwrd = CODE == 'W' ? true : false;

    while (CODE) {
        if (CODE == 'W') pile++;
        if (CODE == 'E') pile--;
        if (pile == 0) return index;

        fwrd == true ? index++ : index--;
    }
}
char *poohbear (const char* sourcecode) {
    unsigned index = 0;
    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
    unsigned char tape[BUFFER] = {0}, *cell = tape;
    char tmp;

    while (CODE) {
        switch (CODE) {
            case '+' : (*cell)++; break;
            case '-' : (*cell)--; break;
            case '>' : cell++; break;
            case '<' : if (cell != tape) cell--; break;
            case 'c' : tmp =  *cell ; break;
            case 'p' : *cell = tmp; break;
            case 'W' : if (*cell == 0) index = loop (sourcecode,index); break;
            case 'E' : if (*cell != 0) index = loop (sourcecode,index); break;
            case 'P' : out += sprintf (out,"%c",*cell); break;
            case 'N' : out += sprintf (out,"%i",*cell); break;
            case 'T' : *cell *= 2; break;
            case 'Q' : *cell = *cell * *cell; break;
            case 'U' : *cell = sqrt (*cell); break;
            case 'L' : *cell += 2; break;
            case 'I' : *cell -= 2; break;
            case 'V' : *cell /= 2; break;
            case 'A' : *cell += tmp; break;
            case 'B' : *cell -= tmp; break;
            case 'Y' : *cell *= tmp; break;
            case 'D' : *cell /= tmp; break;
            default : break;
        }
        index++;
    }

    *out = '\0';
    return output;
}
