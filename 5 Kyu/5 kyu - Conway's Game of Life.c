#include <stddef.h>
#include <stdbool.h>

typedef struct _point { int x, y; } point;

bool is_inside (point p, int width, int height) {
    return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
}
bool get_state (size_t row, size_t col, const bool world[row][col], const point cell) {

    int x = cell.x, y = cell.y;
    int neigh = 0, state = world[y][x];
    point p;

    for (p.y = y - 1; p.y <= y + 1; p.y++) {
        for (p.x = x - 1; p.x <= x + 1; p.x++) {
            if (is_inside (p, col, row) && (p.y != y || p.x != x)) {
                if (world[p.y][p.x]) neigh++;
            }
        }
    }

    if (state == false && neigh == 3)
        return true;

    if (state == true && (neigh == 2 || neigh == 3))
        return true;

    return false;
}
void evolve (size_t row, size_t col, const bool start[row][col], bool next[row][col]) {
    point p;

    for (p.y = 0; p.y < row; p.y++) {
        for (p.x = 0; p.x < col; p.x++) {
            next[p.y][p.x] = get_state (row, col, start, p);
        }
    }
}
