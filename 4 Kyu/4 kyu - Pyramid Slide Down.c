#include <string.h>

int idx (int x, int y) { return x + y * (y + 1) / 2; }
int max (int x, int y) { return x > y ? x : y; }
int pyramid_slide_down (int size, const int tri[size], int rows) {

    int res[size];
    rows -= 1;
  
    memcpy (res, tri, size * sizeof(int));

    while (rows-->0) {
        for (int x = 0; x <= rows; x++) {
            int left = res[idx (x, rows + 1)], right = res[idx (x + 1, rows + 1)];

            res[idx (x,rows)] += max (left, right);
        }
    }

    return res[0];
}
