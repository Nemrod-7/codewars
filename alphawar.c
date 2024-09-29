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
char *count (string src) {
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

char *alphabet_war(string src) {

    int size = strlen(src);
    char *field = strdup(src);

    const char mage[2] = {'t', 'j'};
    string side[2] = {"sbpw", "zdqm"};
    int flat[2] = {-2, +2};

    for (int i = 0; i < size; i++) { // iterate string

        for (int j = 0; j < 2; j++) { // side1 side 2
            int m1 = j, m2 = 1-j;

            if (field[i] == mage[m1]) {
                for (int k = 0; k < 4; k++) { // iterate warriors 
                    if (((i-2) >= 0) && (field[i-2] == mage[m2])) {

                    } else if (((i-1) >= 0) && (field[i-1] == side[m2][k])) {
                        field[i-1] = side[m1][k];
                    }

                    if ((i+2 <= size) && (field[i+2] == mage[m2])) {

                    } else if (((i+1) <= size) & (field[i+1] == side[m2][k])) {
                        field[i+1] = side[m1][k];
                    }
                }
            }
        }
    }

    return count(field);
}

void do_test (const char *fight, const char *expected) {
    const char *actual = alphabet_war(fight);

    if (strcmp(actual, expected) != 0) {
        printf("error: [%s] == [%s]\n", actual, expected);
    }
}



int main () {

    int hash[20] = {[0 ... 19] = 2};

     do_test("z",          "Right side wins!");
     do_test("tz",         "Left side wins!");
     do_test("jbdt",       "Let's fight again!");
     do_test("jz",         "Right side wins!");
     do_test("zt",         "Left side wins!");
     do_test("sj",         "Right side wins!");
     do_test("azt",        "Left side wins!");
     do_test("wololooooo", "Left side wins!");
     do_test("zdqmwpbs",   "Let's fight again!");
     do_test("ztztztzs",   "Left side wins!");   

    //do_test("tzj",        "Right side wins!");

    //for (int l = 0; l < 2; l++) {
    //    int i1 = i + flat[l], i2 = i + flat[l] / 2;

    //    //printf("[%c]", field[i2]);
    //    if ((i1 >= 0) && (i1 <= size) && field[i1] == mage[m2]) {

    //    } else if (((i2 >= 0) && (i2 <= size) &&  field[i2]) == side[m2][k]) {
    //        //printf("[%c]", i1);
    //       field[i2] = side[m1][k];
    //    }
    //}

     printf("\nexit\n");
}
