#include <stddef.h>
#include <stdbool.h>
#include <string.h>

extern void print_2d_arr (size_t rows, size_t cols, const bool universe[rows][cols]);

typedef struct _point { int x, y; } point;

int adj (int x, int lim) {
    if (x == -1) return lim - 1;
    if (x == lim) return 0;
    return x;
}
bool get_state (size_t row, size_t col, const bool world[row][col], const point cell) {

    int xx = cell.x, yy = cell.y;
    int neigh = 0, state = world[yy][xx];

    for (int i = yy - 1; i <= yy + 1; i++) {
        for (int j = xx - 1; j <= xx + 1; j++) {
            int x = adj (j, col), y = adj (i, row);

            if ((i != yy || j != xx) && world[y][x]) neigh++;
        }
    }

    if (state == false && neigh == 3) return true;
    if (state == true && (neigh == 2 || neigh == 3)) return true;

    return false;
}
void get_generation (size_t row, size_t col, const bool start[row][col], unsigned n, bool next[row][col]) {
    point p;
    bool cells[row][col];

    memcpy(&cells[0][0], &start[0][0], row * col * sizeof (start[0][0]));

    while (n-->0) {
        for (p.y = 0; p.y < row; p.y++) {
            for (p.x = 0; p.x < col; p.x++) {
                next[p.y][p.x] = get_state (row, col, cells, p);
            }
        }
        memcpy(&cells[0][0], &next[0][0], row * col * sizeof (next[0][0]));
    }
}
