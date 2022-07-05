#include <stdlib.h>
#include <math.h>

typedef struct _pair { long long first, snd; } Pair;
Pair *mk_pair (long long a, long long b) {
    Pair *p = malloc (sizeof (Pair));
    p->first = a, p->snd = b;
    return p;
}
Pair** listSquared(long long m, long long n, int* length) {

    Pair **res = malloc ((n - m) * sizeof (Pair *));
    int index = 0;

    for (long long i = m; i <= n; i++) {
        long long sum = 0;
        for (long long j = 1; j <= i; j++) {
            if (i % j == 0)
                sum += (j * j);

        }
        long long s2 = sqrt (sum);

        if (s2 * s2 == sum)
          res[index++] =  mk_pair (i, sum);
    }

    *length = index;
    return res;
}
