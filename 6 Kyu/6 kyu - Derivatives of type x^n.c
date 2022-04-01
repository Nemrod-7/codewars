#include <stdio.h>
#include <string.h>

char *differentiate (char *derivative, const char *poly) {

    if (strrchr (poly, 'x') == NULL) {
        sprintf (derivative, "0");
        return derivative;
    }
    char *ptr = derivative;
    int dig = 1, exp = 1;

    if (poly[0] == 'x') {
        sscanf (poly, "x^%d", &exp);
    } else if (poly[0] == '-' && poly[1] == 'x') {
        dig = -1;
        sscanf (poly, "-x^%d", &exp);
    } else {
        sscanf (poly, "%dx^%d",&dig,&exp);
    }
  
    dig *= exp, exp -= 1;

    if (exp) {
        if (dig == -1) {
            ptr += sprintf (ptr, "-");
        } else if (dig < -1 || dig > 1) {
            ptr += sprintf (ptr, "%d", dig);
        }

        ptr += sprintf (ptr, "x");
        if (exp < -1 || exp > 1)
            ptr += sprintf (ptr, "^%d", exp);
    } else {
        ptr += sprintf (ptr, "%d", dig);
    }

    return derivative;
}
