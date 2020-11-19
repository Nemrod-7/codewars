#include <stdlib.h>

#define SPACE " "
#define BUFFSIZE 1024

char *encrypt_this(const char *str) {

    int i, j, lastchar;
    char string[BUFFSIZE];
    char *buffer = malloc (BUFFSIZE * sizeof(char));
    char *next, *word, temp;

    strcpy (string,str);

    next = buffer;
    word = strtok (string,SPACE);

    for (i = 0; word != NULL; ++i) {
        lastchar = strlen(word) - 1;
        for (j = 0; word[j] != '\0'; ++j) {
            if (j == 0) {
                next += sprintf (next,"%i",word[j]);
            } else
            if (j == 1) {
                temp = word[j];
                next += sprintf(next,"%c",word[lastchar]);
            } else
            if (j == lastchar) {
                next += sprintf(next,"%c",temp);
             } else {
                next += sprintf(next,"%c",word[j]);
           }
      }
        next += sprintf(next,"%c",' ');
        word = strtok (NULL, SPACE);
    }
    buffer[strlen(buffer) - 1] = '\0';

    return buffer;
}
