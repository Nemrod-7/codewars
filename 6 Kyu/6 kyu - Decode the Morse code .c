#include <stdlib.h>

const char* morse[55] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-.-.-", "--..--", "..--..", ".----.", "-.-.--", "-..-.", "-.--.", "-.--.-", ".-...", "---...", "-.-.-.", "-...-", ".-.-.", "-....-", "..--.-", ".-..-.", "...-..-", ".--.-.", "...---..."};
const char* ascii[55] = {"A",  "B",    "C",    "D",   "E", "F",    "G",   "H",    "I",  "J",    "K",   "L",    "M",  "N",  "O",   "P",    "Q",    "R",   "S",   "T", "U",   "V",    "W",   "X",    "Y",    "Z",    "0",     "1",     "2",     "3",     "4",     "5",     "6",     "7",     "8",     "9",     ".",      ",",      "?",      "'",      "!",      "/",     "(",     ")",      "&",     ":",      ";",      "=",     "+",     "-",      "_",      "\"",     "$",       "@",      "SOS"};

#define BUFFSIZE 1024
#define FOUND 0
#define CODE *(morse_code + index)
#define NEXTCODE *(morse_code + (index + 1))
#define IS_MORSE(x) (((x) == '.') || ((x) == '-'))

char* decode_morse(const char* morse_code) {

    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
    char *code = strdup (morse_code), *token = strtok (code," ");
    int index = 0;

    while (token) {

        if (*(token - 1) == ' ' && out != output)
            out += sprintf (out," ");

        for (index = 0; index < 55; index++ )
            if (strcmp (token, morse[index]) == FOUND)
                out += sprintf (out,"%s",ascii[index]);

        token = strtok (NULL," ");
    }

    return output;
}
