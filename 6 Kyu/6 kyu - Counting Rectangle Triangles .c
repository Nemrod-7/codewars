#include <stdlib.h>
#include <math.h>
typedef struct point_t { int x, y; } Point;

#define rnd(x) round(x * 1e5) / 1e5
int cmpmin (const void *a, const void *b) { return (*(double*)a >= *(double*)b );}
int ptcmp (const void *a, const void *b) {
    const Point *ia = a, *ib = b;
    return ia->x != ib->x ? ia->x > ib->x : ia->y > ib->y;
}

double dist (const Point p, const Point q) {
    double x = p.x - q.x, y = p.y - q.y;
    return sqrt (x * x + y * y);
}
_Bool is_right2 (Point a, Point b, Point c) {

    double edge[3] = {dist (a, b), dist (b, c), dist (a, c)};
    qsort (edge, 3, sizeof (double), cmpmin);
    if (edge[0] == 0) return 0;

    double a2 = edge[0] * edge[0], b2 = edge[1] * edge[1], c2 = edge[2] * edge[2];

    return (rnd (a2) + rnd (b2) == rnd (c2));
}
unsigned count_rect_triang (const Point *p, unsigned n) {

    unsigned size = 1, count = 0;
    Point map[n];
    qsort (p, n, sizeof (Point), ptcmp);
    map[0] = p[0];

    for (unsigned i = 1; i < n; i++)
        if (p[i].x != p[i - 1].x || p[i].y != p[i - 1].y)
            map[size++] = p[i];

    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = i  + 1; j < size ; j++) {
            for (unsigned k = j + 1; k < size; k++) {
                if (is_right2 (map[i], map[j], map[k]))
                    count++;
            }
        }
    }
    return count;
}
