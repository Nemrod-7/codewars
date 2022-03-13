#include <stdlib.h>
#include <stdbool.h>

typedef struct _point { int x, y; } point;
const point direc[4] = {{0,1},{1,0},{0,-1},{-1,0}};

int **mkgrid (int n) {
  
    int **grid = malloc (n * sizeof (int*));
  
    for (int i = 0; i < n; i++)
        grid[i] = calloc (n , sizeof (int));
  
    return grid;
}
bool is_inside (int n, point p) { return (p.x >= 0 && p.x < n && p.y >= 0 && p.y < n); }
int **create_spiral (int n) {
    if (n < 1 ) return NULL;
    int cnt = 1, pos = 0, N = n * n;
    point p = {0,0}, next;
    int **grid = mkgrid (n);
    
    while (N-->0) {
        next.x = p.x + direc[pos].x, next.y = p.y + direc[pos].y;
        grid[p.x][p.y] = cnt++;

        if (!is_inside (n, next) || grid[next.x][next.y])
            pos += (pos != 3 ? 1 : -3);

        p.x += direc[pos].x, p.y += direc[pos].y;
    }
    return grid;
}
