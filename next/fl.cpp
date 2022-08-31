#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <tuple>
#include <map>
#include <limits>
#include <chrono>

using namespace std;

using point = pair<int,int>;
using vertex = tuple<int, int, vector<point>>;

const vector<char> alpha {'U','R','D','L'};
const vector<pair<int,int>> compas {{0,-1},{1,0},{0,1},{-1,0}};

struct vertex2 {
    int dist, state;
    vector<pair<int,int>> p;
    vector<vector<int>> visit;
    string route;
};

struct comp {
    bool operator()(const vertex2 &a, const vertex2 &b ) {
        return a.dist > b.dist;
    }
};

void display (vector<string> &floor, vector<vector<int>> &grid) {

    const int width = floor[0].size(), height = floor.size();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (floor[y][x] == '0') {
                cout << ' ';
            } else {
                cout << grid[y][x];
            }
        }
        cout << "\n";
    }
}

bool is_inside (const vector<string> &grid, int x, int y) {
    if (x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size()) {
        if (grid[y][x] != '0') return true;
    }
    return false;
}
bool direct (vector<pair<int,int>> bloc, int x, int y) {
    const int nx = (bloc[1].first - bloc[0].first), ny = (bloc[1].second - bloc[0].second);
    return nx == x && ny == y;
}

string dijsktra (vector<string> level, vector<vector<int>> hist, point start, point exit) {

    int cycle = 10;
    vertex2 ver = {1, 1, {start}, hist};
    priority_queue<vertex2,vector<vertex2>, comp> q1;

    q1.push (ver);

    while (cycle-->0) {

        auto [dist, size, p, visit, route] = q1.top();
        q1.pop();

        for (auto &it : p) {
            visit[it.second][it.first] = dist;
        }
        if (size == 1 && p[0] == exit) {
            return route;
        }

        display (level, visit);

        for (auto &[nx,ny] : compas) {
            int x1 = p[0].first + nx, y1 = p[0].second + ny;

            if (size == 1) {
                int x2 = p[0].first + nx * 2, y2 = p[0].second + ny * 2;

                if (is_inside (level, x2, y2) && !visit[y2][x2] ) {
                    vertex2 nextv = {dist + 1, 2, {{x1,y1},{x2,y2}}, visit};
                    q1.push (nextv);
                }

            } else if (size == 2) {
                int x2 = p[1].first + nx, y2 = p[1].second + ny;

                if (is_inside (level, x2, y2) && !visit[y2][x2] ) {

                    if (direct (p, nx, ny)) {
                        vertex2 nextv = {dist + 1, 1, {{x2,y2}}, visit};
                        q1.push (nextv);
                    } else if (is_inside (level, x1, y1)  && !visit[y1][x1] ) {
                        vertex2 nextv = {dist + 1, 2, {{x1,y1},{x2,y2}}, visit};
                        q1.push (nextv);
                    }
                }
            }
        }
    }

    return "";
}

void dijsktra2 (vector<string> level, vector<vector<int>> hist, point start, point exit) {

    int cycle = 15;
    queue<vertex> q1;
    q1.push ({1, 1, {start}});

    while (cycle-->0) {

        auto [size, index, p] = q1.front();
        q1.pop();

        for (auto &it : p) {
            hist[it.second][it.first] = index;
        }
        if (size == 1 && p[0] == exit) {
            break;
        }

        display (level, hist);

        for (auto &[nx,ny] : compas) {
            int x1 = p[0].first + nx, y1 = p[0].second + ny;
            int x2, y2;

            if (size == 1) {
                x2 = p[0].first + nx * 2, y2 = p[0].second + ny * 2;
            } else if (size == 2) {
                x2 = p[1].first + nx, y2 = p[1].second + ny;
            }

            if (is_inside (level, x2, y2)   && !hist[y2][x2]    ) {
                if (direct (p, nx, ny)) {
                    q1.push (vertex {1, index + 1, {{x2,y2}}});
                } else if (is_inside (level, x1, y1)   && !hist[y1][x1]    ) {
                    q1.push (vertex {2, index + 1, {{x1,y1},{x2,y2}}});
                }
            }
        }
    }
}

int main  () {

    vector<string> floor = {"1110000000","1B11110000","1111111110","0111111111","0000011X11","0000001110"};

    const int width = floor[0].size(), height = floor.size();

    pair<int,int> exit,  start;
    vector<vector<int>> grid (height, vector<int> (width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile = floor[y][x];

            if (tile == 'X') exit = {x,y};
            if (tile == 'B') start = {x,y};
        }
    }

    dijsktra (floor, grid, start, exit);

    //display (floor, grid);
}


void arkive () {

  /*
  if (is_inside (floor, x1, y1)  && !grid[y1][x1]) { // stand up inverse, is it necessary
      if (p[1].first + x == p[0].first && p[1].second + y == p[0].second) {
          grid[y1][x1] = index + 1;
          q1.push ({1, index + 1, {{x2,y2},{x1,y1}}});
      } else {

      }
  }
  */
}
