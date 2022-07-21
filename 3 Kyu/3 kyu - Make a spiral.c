#include <stdio.h>
#include <stdbool.h>

typedef struct _point { unsigned x, y; } point;
const point direc[4] = {{0,1},{1,0},{0,-1},{-1,0}};

void clear (unsigned n, int grid[n][n]) {
    for (unsigned i = 0; i < n; i++)
        for (unsigned j = 0; j < n; j++)
            grid[i][j] = 0;
}
void spiralize (unsigned n, int grid[n][n]) {

    int pos = 0, end =  n - 1;
    point p = {0,0}, nxt, sup;
}

int main () {

    int size = 15;
    bool grid[15][15] = {{0}};
    int y = 0, x = 0;
    int dir = 0;
    int maxx = size - 1, maxy = size - 1;
    int minx = 0, miny = 0;

    while (1) {
        grid[y][x] = 1;
        
        switch (dir) {

            case 0 : if (x == maxx) { miny += 2, dir = 1; } else x++; break;
            case 1 : if (y == maxy) { maxx -= 2, dir = 2; } else y++; break
            case 2 : if (x == minx) { maxy -= 2, dir = 3; } else x--; break;
            case 3 : if (y == miny) { minx += 2, dir = 0; } else y--; break;

            if (minx > maxx && miny > maxy) break;
        }
    }

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            printf ("%i ", grid[y][x]);
        }
        printf ("\n");
    }

}
