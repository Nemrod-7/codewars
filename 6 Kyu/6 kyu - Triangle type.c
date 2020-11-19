#include <stdlib.h>
enum {error, acute,right,obtuse};
int compare (const void * a, const void * b) { return ( *(int*)a - *(int*)b );}

int triangleType(int a, int b, int c) {

    int edge[3] = {a,b,c};
    qsort (edge, 3, sizeof (int), compare);

    int a2 = edge[0] * edge[0], b2 = edge[1] * edge[1], c2 = edge[2] * edge[2];

    if (a + b <= c  || a + c <= b || b + c <= a) return error;
    if (c2 < a2 + b2) return acute;
    if (c2 == a2 + b2) return right;
    if (c2 > a2 + b2) return obtuse;
}
