#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

const char pad[] = "abcde123fghij456klmno789pqrst.@0uvwxyz_/| ";
typedef struct _point {int x, y;} point;

int distance (const point a, const point b) { return abs (a.x - b.x) + abs(a.y - b.y); }
bool shift (const char tile, const bool maj) { return ((isupper (tile) && !maj) || (islower(tile) && maj)); }
point getpos (const char search) {
    int idx = strlen (pad);

    while (idx-->0)
        if (pad[idx] == search)
            return (point) {idx % 8, idx / 8};

    return (point) {-1,-1};
}
int tv_remote (const char *src) {

    int i = 0, size = strlen (src), dist = 0;
    bool maj = false;
    point shft = getpos ('|'), prev = {0,0}, now;

    while (size > i) {
        now = getpos (tolower (src[i]));

        if (shift (src[i], maj)) {
            dist += distance (prev, shft) + distance (shft, now) + 2;
            maj ^= 1;
        } else {
            dist += distance (prev, now) + 1;
        }

        prev = now;
        i++;
    }

    return dist;
}
