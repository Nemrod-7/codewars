#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char *strike (const char *src, const char *attak) {
    char *field = strdup(src);
    const size_t end = strlen(attak);

    for (size_t i = 0; i < end; i++) {
        if (attak[i] == '*') {
            field[i] = '_';

            if (i == 0) {
                field[i+1] = '_';
            } else if (i == end - 1) {
                field[i-1] = '_';
            } else {
                field[i-1] = field[i+1] = '_';
            }
        }
    }

    return field;
}
const char *count (const char *src) {
    int lfs = 0, rfs = 0;

    for (int i = 0; src[i] != '\0'; i++) {
        switch(src[i]) {
            case 's': lfs += 1; break;
            case 'z': rfs += 1; break;
            case 'b': lfs += 2; break;
            case 'd': rfs += 2; break;
            case 'p': lfs += 3; break;
            case 'q': rfs += 3; break;
            case 'w': lfs += 4; break;
            case 'm': rfs += 4; break;
        }
    }

    if (lfs == rfs) {
        return strdup("Let's fight again!");
    } else if (lfs > rfs) {
        return strdup("Left side wins!");
    } else {
        return strdup("Right side wins!");
    }
}

void reinforce(char *field, int nforce, char *force[nforce]) {

    const int size = strlen(force[0]);

    for (int i = 0; i < nforce; i++){
        for (int j = 0; j < size; j++) {
            if (force[i][j] != '_' && field[j] == '_') {
                field[j] = force[i][j];
                force[i][j] = '_';
            }
        }
    }

}
const char *alphabet_war(const char **force, const char **attak, int nforce, int nattak) {

    const int size = strlen(force[0]);

    char *field = malloc(size * sizeof(char));
    memset (field,'_',size);

    char *forc2[nforce];

    for (int i = 0; i < nforce; i++){
        forc2[i] = malloc(size * sizeof(char));
        memcpy(forc2[i], force[i], size * sizeof(char));
    }


    for (int i = 0; i < nattak; i++) {
        reinforce(field, nforce, forc2);
        field = strike (field, attak[i]);
    }

    reinforce(field, nforce, forc2);

    printf("%s\n", field);
    return field;
}

int main () {

    const int nforce = 10, nattak = 9;
    const char *force[10] =
    {"g964xxxxxxxx", "myjinxin2015", "steffenvogel", "smile67xxxxx", "giacomosorbi",
        "freywarxxxxx", "bkaesxxxxxxx", "vadimbxxxxxx", "zozofouchtra", "colbydauphxx" };
    const char *attak[10] =
    {"* *** ** ***", " ** * * * **", " * *** * ***", " **  * * ** ", "* ** *   ***",
        "***         ", "**          ", "*           ", "*           " };

    //alphabet_war(force, attak, 10, 9); /* => codewarsxxxx */

    char *field = strdup("abcde[fg]hi#jkl[m]nop");


    for (int i = 0; field[i]!= '\0'; i++) {
        if (field[i] == '#') {
            bool shelter = false;

            for (int j = 0; field[j] != '\0'; j++) {
                if (field[j] == '[') shelter = true;
                if (shelter == false) field[j] = '_';
                if (field[j] == ']') shelter = false;
            }

            int j = i;
            while (j >= 0 ) {
                if (field[j] == '[' || field[j] == ']') {
                    field[j] = '_';
                    break;
                }
                j--;
            }
            j = i;

            while (field[j] != '\0') {
                if (field[j] == '[' || field[j] == ']') {
                    field[j] = '_';
                    break;
                }
                j++;
            }


        }
    }

    for (int i = 0; field[i]!= '\0'; i++) {

    }

    printf("%s\n", field);

    printf("\nexit\n");
}
