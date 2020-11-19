#include <stdbool.h>

#define BUFFSIZE 100000000

#define CODE *(arr + i)
#define NEXT *(arr + (i + 1))

#define PTR(x) (((x) == '>') || ((x) == '<'))
#define BYT(x) (((x) == '+') || ((x) == '-'))
#define IO(x) (((x) == '.') || ((x) == ','))
#define LOOP(x) (((x) == '[') || ((x) == ']'))
#define IS_BRNFCK(x) (PTR(x) || BYT(x) || IO(x) || LOOP(x))

typedef enum {FWRD = -3,ADD,DEC,CENTER,INC,SUB,BACK,IN,OUT} command;
typedef unsigned long long lsize;

char c_code[9][2][16] = {       // ==   (115 / 9)
        {"if (*p) do {\n"},     // FRWD [ 14
        {"*p += ",";\n"},       // ADD  + 9 + 2
        {"p -= ",";\n"},        // DEC  < 8 + 2
        {"Error!"},             // CENTER 8
        {"p += ",";\n"},        // INC  > 8 +2
        {"*p -= ",";\n"},       // SUB  - 9 + 2
        {"} while (*p);\n"},    // BACK ] 17
        {"*p = getchar();\n"},  // IN   , 18
        {"putchar(*p);\n"}      // OUT  . 14
        };

command get (char c) {
    switch (c) {
        case '[' : return FWRD;
        case ']' : return BACK;
        case ',' : return IN;
        case '.' : return OUT;
        case '-' : return SUB;
        case '+' : return ADD;
        case '<' : return DEC;
        case '>' : return INC;
        default : return CENTER;
    }
}
char *reduce (char* arr, lsize *length) {

    lsize i = 0, loop = 0, size = 0;
    //lsize pad = 0;
    const lsize end = *length;
    char *buffer = malloc (end * sizeof (char));

    while (end > i) {

        if (IS_BRNFCK (CODE))
            if (size < 1)
                buffer[size++] = CODE;
            else {
                command now = get (buffer[size - 1]), next = get (CODE);

                if (now + next == CENTER && now != 3)
                    size--;
                else
                    buffer[size++] = CODE;
            }

        i++;
    }
    buffer[size] = '\0';

    return buffer;
}
bool validParentheses (const char* arr) {

    int pile = 0;
    char c;

    while ((c = *arr++)) {
      if (pile < 0) return false;
      if (c == '[') pile++;
      if (c == ']') pile--;
    }

    return ((pile == 0) ? true : false);
}
const char* brainfuck_to_c (const char* source) {

    if (validParentheses (source) == false) {
        char *result = malloc (8);
        strcpy (result,"Error!");

        return result;
    }

    lsize size = strlen (source);
    char *arr = reduce (source, &size);

    command code;
    lsize i = 0, count = 0, loop = 0, tab = 0;
    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
    *output = '\0';

    size = 0;
    while (CODE) {
        tab = loop, code = get (CODE);

        if (BYT (CODE) || PTR (CODE)) {
            if (get (NEXT) != code) {
                while (tab-->0)
                    out += sprintf (out,"  ");

                out += sprintf (out,"%s%lu%s", c_code[code + 3][0], count + 1, c_code[code + 3][1]);
                count = 0;
            } else
                count++;
        }

        if (IO (CODE) || LOOP (CODE)) {
            if (CODE == ']') tab--, loop--;
            if (CODE == '[') loop++;

            while (tab-->0)
                out += sprintf (out,"  ");

            out += sprintf (out,"%s", c_code[code + 3][0]);
        }

        i++;
    }

    *out = '\0';

    return output;
}
