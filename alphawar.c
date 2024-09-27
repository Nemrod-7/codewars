#include <stdio.h>
#include <string.h>


char *strike (const char *src) {
    char *field = strdup(src);
    const size_t end = strlen(src);

    for (size_t i = 0; i < end; i++) {
        if (src[i] == '*') {
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

const char *alphabet_war(const char *src) {

    const char *field = strike(src);
    printf("%s\n", field);
    return count(src);
}

int main () {

    const char *res;

    alphabet_war("s*zz");          /* => Right side wins!  */
    alphabet_war("*zd*qm*wp*bs*"); /* => Let's fight again! */
    alphabet_war("zzzz*s*");       /* => Right side wins!  */
    alphabet_war("www*www****z");  /* => Left side wins!  */


}
