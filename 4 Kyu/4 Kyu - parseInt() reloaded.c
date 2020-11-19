#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FOUND 0

char base[5][20][10] = {
    {"zero","one","two","three","four","five","six","seven","eight","nine",
     "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
    {".",".","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"},
    {".","hundred"},
    {".","thousand"},
    {"million"}};

long parse_int (const char* entry) {

    char *string = strdup (entry), *token = strtok (string," ,.-");
    unsigned num = 0, sec = 0, index, cardinal, shift;
    bool mill = false;

    while (token != NULL) {
        for (index = 0; index < 5 ; ++index)
            for (cardinal = 0; cardinal < 20; ++cardinal)
                if (strcmp (token, base[index][cardinal]) == FOUND) {

                    if (index == 4) index = 6;

                    shift = pow (10,index);

                    if (mill == false)
                        num = (index < 2) ? num + (cardinal * shift) : num * shift;
                    else
                        sec = (index < 2) ? sec + (cardinal * shift) : sec * shift;

                    if (index == 3) mill = true;
                }

        token = strtok (NULL," ,.-");
   }
    num = num + sec;

    return num;
}
