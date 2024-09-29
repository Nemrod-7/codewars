#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef const char* string;

char *strike (string src, string attak) {
    char *field = strdup(src);
    const size_t end = strlen(attak);
    printf("[%i]", end);
    for (size_t i = 0; i < end; i++) {
        if (attak[i] == '*') {
            field[i] = '_';

            if (i == 0) {
                field[i+1] = '_';
            } else {
                field[i-1] = field[i+1] = '_';
            }
        }
    }

    return field;
}
string count (string src) {
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
string alphabet_war2(string *force, string *attak, int nforce, int nattak) { // reinforces massacre

    const int size = strlen(force[0]);
    char *field = malloc((size + 1) * sizeof(char)), *forc2[nforce];
    memset (field,'_',size);

    for (int i = 0; i < nforce; i++){
        forc2[i] = malloc(size * sizeof(char));
        memcpy(forc2[i], force[i], size * sizeof(char));
    }

    for (int i = 0; i < nattak; i++) {
        reinforce(field, nforce, forc2);
        field = strike (field, attak[i]);
    }

    field[size] = '\0';
    reinforce(field, nforce, forc2);

    printf("[%s]\n", field);
    return field;
}
char *alphabet_war3(string field) { // nuclear strike

    const int size = strlen(field);
    char *res = malloc(size * sizeof(char)), *ptr = res;

    int point[size];
    bool shelter = false;

    for (int i = 0; i < size; i++) {
        if (field[i] == '[') shelter = true;
        point[i] = shelter == true ? 3 : 1;
        if (field[i] == ']') shelter = false;
    }

    for (int i = 0; i < size; i++) {
        if (field[i] == '#') {
            for (int j = i + 1; j < size; j++) {
                point[j]--;
                if (field[j] == ']') break;
            }

            for (int j = i - 1; j >= 0; j--) {
                point[j]--;
                if (field[j] == '[') break;
            }

            for (int j = 0; j < size; j++) {
                if (point[j] < 2) point[j] = 0;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (point[i] > 0 && field[i] >= 'a' && field[i] <= 'z') {
            *ptr++ = field[i];
        }
        // printf("%i ", point[i]);
    }
    *ptr++ = '\0';
    return res;
}

int main () {

    const int nforce = 10, nattak = 9;
    const char *force[10] =
    {"g964xxxxxxxx", "myjinxin2015", "steffenvogel", "smile67xxxxx", "giacomosorbi",
        "freywarxxxxx", "bkaesxxxxxxx", "vadimbxxxxxx", "zozofouchtra", "colbydauphxx" };
    const char *attak[10] =
    {"* *** ** ***", " ** * * * **", " * *** * ***", " **  * * ** ", "* ** *   ***",
        "***", "**", "*", "*" };

    alphabet_war2(force, attak, 10, 9); /* => codewarsxxxx */

    int hash[20] = {[0 ... 19] = 2};


    printf("\nexit\n");
}
