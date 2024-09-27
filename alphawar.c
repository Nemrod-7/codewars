#include <stdio.h>
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

const char *alphabet_war(const char *src) {

    const char *field = strike(src, src);
    printf("%s\n", field);
    return count(field);
}

int main () {

  const int nforce = 10, nattak = 9;
  const char *force[10] =
          {"g964xxxxxxxx", "myjinxin2015", "steffenvogel", "smile67xxxxx", "giacomosorbi",
           "freywarxxxxx", "bkaesxxxxxxx", "vadimbxxxxxx", "zozofouchtra", "colbydauphxx" };
  const char *attak[10] =
     {"* *** ** ***", " ** * * * **", " * *** * ***", " **  * * ** ",
      "* ** *   ***", "***   ", "**", "*", "*" };

  const int size = strlen(force[0]);

   char *field = malloc(size * sizeof(char));
   memset (field,'_',size);

   char *forc2[nforce];

    // memcpy(forc2, force, nforce *  sizeof())

    for (int i = 0; i < nforce; i++){


        // for (int j = 0; force[i][j] != '\0'; j++) {
        //     if (force[i][j] != '_' && field[j] == '_') {
        //         field[j] = force[i][j];
        //         // force[i][j] = '_';
        //     }
        // }
        printf("%s\n", force[i]);
    }

    for (int i = 0; i < nattak; i++) {

        field = strike (field, attak[0]);
    }


    printf("%s\n", field);

    // alphabet_war("s*zz");          /* => Right side wins!  */
    // alphabet_war("*zd*qm*wp*bs*"); /* => Let's fight again! */
    // alphabet_war("zzzz*s*");       /* => Right side wins!  */
    // alphabet_war("www*www****z");  /* => Left side wins!  */


}
