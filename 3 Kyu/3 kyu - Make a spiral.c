#include <stdbool.h>

typedef struct _point { unsigned x, y; } point;
const point direc[4] = {{0,1},{1,0},{0,-1},{-1,0}};

void clear (unsigned n, int grid[n][n]) {
    for (unsigned i = 0; i < n; i++)
        for (unsigned j = 0; j < n; j++)
            grid[i][j] = 0;
}
bool is_inside (unsigned n, point p) { return (p.x >= 0 && p.x < n && p.y >= 0 && p.y < n); }
void spiralize (unsigned n, int grid[n][n]) {

    int pos = 0, end =  n - 1;
    point p = {0,0}, nxt, sup;

    clear (n,grid);
    //printf ("%d %d\n", center.x, center.y);
    while (true) {
        nxt.x = p.x + direc[pos].x, nxt.y = p.y + direc[pos].y;
        sup.x = nxt.x + direc[pos].x, sup.y = nxt.y + direc[pos].y;
        grid[p.x][p.y] = 1;

        if (end == 0) {
            if (n % 2) grid[nxt.x][nxt.y] = 1;
            break;
        }
        /*
        */
        if (!is_inside (n, nxt) || (is_inside (n, sup) && grid[sup.x][sup.y] == 1) ) {
            pos += (pos != 3 ? 1 : -3);
            end--;
        }

        p.x += direc[pos].x, p.y += direc[pos].y;
    }
}
