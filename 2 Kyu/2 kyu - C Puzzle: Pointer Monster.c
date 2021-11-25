#include <stdio.h>

char a[4][7] = {"Common", "Point", "Boost", "Better"};
char (*b[4])[7] = {a+3, a+1, a, a+2}; // __typeof__(*a) *b[] = {a+3, a+1, a, a+2};

char (*(*c (void))[4])[7] { // __typeof__(b) *c(void)
    return &b;
}

char (**d (void))[7] { // __typeof__ (*b) *(d(void))
    return c()[1] - 3;
}

char buf[256];
char *pointer_monster (char (**f (void))[7]) {
    int len;

    len  = sprintf(buf, "%s", *f()[0]);
    len += sprintf(buf + len, "%s ", *((**f)()-1)[0]+4);
    len += sprintf(buf + len, "%s", (*f())[0]-4);
    len += sprintf(buf + len, "%s", f()[1][2]+3);
    len += sprintf(buf + len, "%s", *((**f)()-1)[0]+4);

    return buf;
}
int main () {

    pointer_monster(d);
    printf ("%s", buf);
    //printf ("%s", d()[1][2]+3);
}
