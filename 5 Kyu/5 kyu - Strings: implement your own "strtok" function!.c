#include <stddef.h> // NULL
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFSIZE 64

bool check (const char ref, const char *entry) {

    for (int i = 0; entry[i] != '\0'; ++i)
        if (ref == entry[i])
            return true;

    return false;
}
char* my_strtok(char *src, const char *delims) {

    static int i, size;
    static char buffer[BUFFSIZE];
    char *token = malloc (BUFFSIZE * sizeof(char));
    bool delim;
    short next = 0;

    if (src != NULL) {
        i = 0, size = strlen (src);
        strcpy (buffer,src);
    }

    while (i <= size) {
        delim = check (buffer[i], delims);

        if (delim == false)
            token[next++] = buffer[i++];

        if (delim || i == size) {
            i++;

            if (next != 0) {
                token[next] = NULL;
                return token;
            }
        }
    }

    return NULL;
}
