#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGIT 1000
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

enum {SUPER, EQUAL, INFER};

typedef struct _bint {
    int sign, size;
    int *dig;
} Bigint;

void Display (Bigint x) {
    if (x.sign == 1) printf ("-");

    for (int i = 0; i < x.size; i++)
        printf ("%i", x.dig[i]);

    printf (" ");
}

Bigint new (char *src) {
    char *ptr = src;
    while (*ptr == '0') ptr++;
    int size = strlen (src) - (ptr - src), i = 0;
    Bigint next = {0, size, malloc (size * sizeof (int))};

    while (*ptr)
        next.dig[i++] = *ptr++ - '0';

    return next;
  }
Bigint null () {
    Bigint nl = {0, 1, calloc (1, sizeof(int))};
    return nl;
  }
char *to_string (const Bigint a) {
    char *os = malloc (a.size + 1 * sizeof (char));
    int i = 0;

    for (; i < a.size; i++)
        os[i] = a.dig[i] + '0';

    os[i] = '\0';
    return os;
}
int compare (Bigint a, Bigint b) {
    if (a.size < b.size) return INFER;
    if (a.size > b.size) return SUPER;

    for (int i = 0; i < a.size; i++) {
        if (a.dig[i] < b.dig[i]) return INFER;
        if (a.dig[i] > b.dig[i]) return SUPER;
    }

    return EQUAL;
}

Bigint add (const Bigint a, const Bigint b) {

    int i = a.size, j = b.size, size = MAX (i,j);
    int rem = 0, num;
    int ans[MAX_DIGIT] = {0}, k = 0;

    while (size-->0) {
        num = (i > 0 ? a.dig[--i] : 0) + (j > 0 ? b.dig[--j] : 0) + rem;
        rem = num / 10;
        ans[k++] = num % 10;
    }

    if (rem > 0) ans[k++] = rem;
    Bigint res = {0, k, malloc (k * sizeof (int))};

    i = 0;
    while (k-->0) res.dig[i++] = ans[k];

    return res;
}
Bigint mul (const Bigint a, const Bigint b) {
    int ans[MAX_DIGIT] = {0};
    int i,j, size;

    if ((a.size == 1 && a.dig[0] == 0)|| (b.size == 1 && b.dig[0] == 0))
        return null ();

    for (i = a.size - 1; i >= 0; i--) {
        for (j = b.size - 1; j >= 0; j--) {
            int num = ans[i + j + 1] + a.dig[i] * b.dig[j];
            ans[i + j + 1] = num % 10;
            ans[i + j] += num / 10;
        }
    }

    size = a.size + b.size;

    int offset = 0;
    while (offset < size - 1  && ans[offset]==0)
        offset++;

    Bigint res = {0, size - offset, malloc ((size) * sizeof (int))};

    for (i = 0; i < size - offset; i++)
        res.dig[i] = ans[i + offset];

    return res;
}
Bigint sub (const Bigint a, const Bigint b) {

    if (compare (a, b) == EQUAL) return null ();
    int i = a.size, j = b.size, k = 0, size = MAX (i, j), carry = 0;
    int *aa = a.dig, *bb = b.dig, ans[MAX_DIGIT] = {};
    int minus = 0;

    if (compare (a, b) == INFER) {
        i = b.size, j = a.size;
        aa = b.dig, bb = a.dig;
        minus = 1;
    }

    while (size-->0) {
        int dig = (i > 0 ? aa[--i] : 0) - (j > 0 ? bb[--j] : 0) - carry;

        if (dig < 0) {
            dig += 10, carry = 1;
        } else {
            carry = 0;
        }
        ans[k++] = dig;
    }

    Bigint res = {minus, k, malloc (k * sizeof (int))};

    i = 0;
    while (k-->0) res.dig[i++] = ans[k];

    return res;
}

char *integerSquareRoot (char *src) {
    const int size = strlen (src);
    const Bigint one = new ("1"), ten = new ("10"), twe = new ("20");

    Bigint root = null (), carr = null ();
    char num[MAX_DIGIT] = {}, buff[MAX_DIGIT] = {};
    if (size % 2) num[0] = '0';

    strcat (num, src);
    int i = 0;
    // printf ("%i ", strlen (num));

    for (; i < size; i += 2) {
        sprintf (buff, "%s%c%c",to_string (carr), num[i], num[i + 1]);
        Bigint x = null (), n = mul (root, twe);
        Bigint ref = new (buff);
        //Display (ref);
        while (compare (mul (add (n, x), x), ref) > SUPER) {
            x = add (x, one);
        }
        //printf ("%i ", compare (mul (add (n, x), x), ref));
        x = sub (x, one);
        root = add (mul (root, ten), x);
        carr = sub (ref, mul (add (n, x), x));
    }


    return to_string (root);
}

int main () {

    char *root = integerSquareRoot ("23232328323215435345345345343458098856756556809400840980980980980809092343243243243243098799634");

    printf ("\nroot -> [%s]\n", root);
    //Bigint res = sub (new ("799826107820075186796709"), new ("609686192373951474847964"));
    //printf ("%i", 8 * 2);
    //Display (res);

    return 0;
}
