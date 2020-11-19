#include <vector>

using namespace std;

struct point { int x, y; };
const vector<point> direc {{0,1},{1,0},{0,-1},{-1,0}};

bool is_inside (int n, point p) {
    return (p.x >= 0 && p.x < n && p.y >= 0 && p.y < n);
}

std::vector<std::vector<int>> create_spiral(int n) {
    if (n < 1 ) return {};
    int cnt = 1, pos = 0, N = n * n;
    vector<vector<int>> grid (n, vector<int>(n));
    point p = {0,0}, next;

    while (N-->0) {
        next.x = p.x + direc[pos].x, next.y = p.y + direc[pos].y;
        grid[p.x][p.y] = cnt++;

        if (!is_inside (n, next) || grid[next.x][next.y])
            pos += (pos != 3 ? 1 : -3);

        p.x += direc[pos].x, p.y += direc[pos].y;
    }
    return grid;
}
