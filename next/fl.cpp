#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <tuple>
#include <map>
#include <cmath>
#include <limits>
#include <chrono>

using namespace std;

using point = pair<int,int>;
using vertex = tuple<int, int, vector<point>>;

const vector<char> alpha {'U','R','D','L'};
const vector<pair<int,int>> compas {{0,-1},{1,0},{0,1},{-1,0}};

struct vertex2 {
    int dist, state;
    vector<point> p;
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
                if (grid[y][x])
                    cout << grid[y][x];
                else
                    cout << ".";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

int distance (const point &a, const point &b) {
    return abs (a.first - b.first) + abs (a.second - b.second);
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

    vertex2 ver = {distance (start, exit), 1, {start}, hist};
    priority_queue<vertex2,vector<vertex2>, comp> q1;
    map<vector<vector<int>>,bool> visited;

    q1.push (ver);

    while (!q1.empty()) {

        auto [dist, size, p, grid, route] = q1.top();
        q1.pop();

        visited[grid] = true;
        //display (level, grid);
        if (size == 1 && p[0] == exit) {
            cout << route << "\n";
            return route;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = compas[i].first, ny = compas[i].second;
            const int x1 = p[0].first + nx, y1 = p[0].second + ny;
            int x2, y2;

            if (size == 1) {
                x2 = p[0].first + nx * 2, y2 = p[0].second + ny * 2;
            } else {
                x2 = p[1].first + nx, y2 = p[1].second + ny;
            }

            int alt = distance ({x2, y2}, exit) + 1;

            if (is_inside (level, x2, y2)) {

                auto path = grid;
                path[y2][x2] = alt;

                if (direct (p, nx, ny)) {

                    if (!visited[path]) {
                        vertex2 nextv = {alt, 1, {{x2,y2},{x2,y2}}, path, route + alpha[i]};
                        q1.push (nextv);
                    }

                } else if (is_inside (level, x1, y1)) {
                    path[y1][x1] = alt;

                    if (!visited[path]) {
                        vertex2 nextv = {alt, 2, {{x1,y1},{x2,y2}}, path, route + alpha[i]};
                        q1.push (nextv);
                    }
                }
            }
        }
    }

  //display (level, path);

    return "";
}
string blox_solver (vector<string> level) {

    const int width = level[0].size(), height = level.size();

    pair<int,int> exit,  start;
    vector<vector<int>> grid (height, vector<int> (width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int tile = level[y][x];

            if (tile == 'X') exit = {x,y};
            if (tile == 'B') start = {x,y};
        }
    }

    dijsktra (level, grid, start, exit);

    return "";
}
int main  () {

    vector<string> level1 = {"1110000000","1B11110000","1111111110","0111111111","0000011X11","0000001110"};
    vector<string> level2 = {"000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"};

    blox_solver (level2);

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
string dijsktra2 (vector<string> level, vector<vector<int>> hist, point start, point exit) {

    int cycle = 50;
    vertex2 ver = {1, 1, {start}, hist};
    priority_queue<vertex2,vector<vertex2>, comp> q1;
    map<vector<vector<int>>,bool> visited;

    q1.push (ver);

    //while (!q1.empty()) {
    while (cycle-->0) {

        auto [dist, size, p, visit, route] = q1.top();
        q1.pop();

          for (auto &it : p) {
              visit[it.second][it.first] = dist;
          }
        if (size == 1 && p[0] == exit) {

            cout << route << "\n";
            return route;
        }
        display (level, visit);

        for (int i = 0; i < 4; i++) {
            const int nx = compas[i].first, ny = compas[i].second;
            const int x1 = p[0].first + nx, y1 = p[0].second + ny;
            int alt = dist + 1;

            if (size == 1) {
                int x2 = p[0].first + nx * 2, y2 = p[0].second + ny * 2;

                if (is_inside (level, x2, y2)  && !visit[y2][x2] ) {
                    vertex2 nextv = {alt, 2, {{x1,y1},{x2,y2}}, visit, route + alpha[i]};
                    q1.push (nextv);
                }

            } else if (size == 2) {
                int x2 = p[1].first + nx, y2 = p[1].second + ny;

                if (is_inside (level, x2, y2)   && !visit[y2][x2]  ) {

                    if (direct (p, nx, ny)) {
                        vertex2 nextv = {alt, 1, {{x2,y2}}, visit, route + alpha[i]};
                        q1.push (nextv);
                    } else if (is_inside (level, x1, y1)  && !visit[y1][x1] ) {
                        vertex2 nextv = {alt, 2, {{x1,y1},{x2,y2}}, visit, route + alpha[i]};
                        q1.push (nextv);
                    }
                }
            }
        }
    }

    return "";
}