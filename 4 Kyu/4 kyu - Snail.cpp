#include <iostream>
#include <vector>

using namespace std;

struct point {
  int x, y;
};

const vector<point> coord {{0,1},{1,0},{0,-1},{-1,0}};

bool is_inside (size_t height, size_t width, point p) {
    return (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height);
}
vector<int> snail(const vector<vector<int>> &snail_map) {

    vector<int> output;
    vector<vector<int>> grid = snail_map;
    size_t pos = 0, rows = snail_map[0].size(), cols = snail_map.size(), index = rows * cols;
    point p, next;
    while (index-->0) {
        next.x = p.x + coord[pos].x, next.y =  p.y + coord[pos].y;

        if (!is_inside (rows, cols, next) || grid[next.x][next.y] == -1)
            pos += (pos != 3 ? 1 : -3);

        output.push_back (snail_map[p.x][p.y]);
        grid[p.x][p.y] = -1;

        p.x += coord[pos].x, p.y += coord[pos].y;
    }

    return output;
}


int main () {

  vector<vector<int>> v = {{1,2,3}, {8,9,4}, {7,6,5}, {4,8,9}};


  return 0;
}
