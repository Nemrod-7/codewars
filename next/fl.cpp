#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <chrono>

using namespace std;

enum { up, c1, c2, c3, c4, dw};
const vector<pair<int,int>> compass {{0,-1},{1,0},{0,1},{-1,0}};

bool is_inside (const vector<string> &grid, int x, int y) {
    if (x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size()) {
        if (grid[y][x] != '0') return true;
    }
    return false;
}
int getsize (int cube) {

    if (cube == up || cube == dw) return 1;
    return 2;
}
int main  () {

  vector<string> floor = {"1110000000","1B11110000","1111111110","0111111111","0000011X11","0000001110"};

  const int width = floor[0].size(), height = floor.size();

  pair<int,int> exit;
  vector<vector<int>> grid (height, vector<int> (width));

  const vector<pair<int,int>> s1 {{0,-1},{1,0},{0,1},{-1,0}};
  const vector<pair<int,int>> s2 {{0,-2},{2,0},{0,2},{-2,0}};

  using vertex = pair<int,vector<pair<int,int>>>;
  queue<vertex> q1;

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          int tile = floor[y][x];

          if (tile == 'X') exit = {x,y};
          if (tile == 'B') {
              grid[y][x] = 1;
              vector<pair<int,int>> vf ({{x,y}});
              q1.push ({up, vf});
          }
      }
  }

  auto [state, p] = q1.front();
  q1.pop();

  if (state == up) {

      for (int i = 0; i < 4; i++) {

          int x1 = p[0].first + s1[i].first, y1 = p[0].second + s1[i].second;
          int x2 = p[0].first + s2[i].first, y2 = p[0].second + s2[i].second;

          if (is_inside (floor, x2, y2)) {
              grid[y1][x1] = 2;
              grid[y2][x2] = 2;

              q1.push ({c1, {{x1,y1},{x2,y2}}});
          }
      }
  }

  vertex u = q1.front();
  q1.pop();
  state = u.first, p = u.second;

  if (state == c1) {

      for (int i = 0; i < 4; i++) {

          int x1 = p[1].first + s1[i].first, y1 = p[1].second + s1[i].second;
          int x2 = p[0].first + s1[i].first, y2 = p[0].second + s1[i].second;

          if (is_inside (floor, x1, y1) && !grid[y1][x1]) { // stand up normal
              if (p[0].first + s1[i].first == p[1].first && p[0].second + s1[i].second  == p[1].second) {
                  grid[y1][x1] = 3;
                  q1.push ({c1, {{x1,y1},{x2,y2}}});
              } else  {
                  if (is_inside (floor, x2, y2) && !grid[y2][x2]) {
                      grid[y1][x1] = 4;
                      grid[y2][x2] = 4;
                      q1.push ({c1, {{x1,y1},{x2,y2}}});
                  }
              }
          }

          if (is_inside (floor, x2, y2) && !grid[y2][x2]) { // stand up inverse
              if (p[1].first + s1[i].first == p[0].first && p[1].second + s1[i].second  == p[0].second) {
                  grid[y2][x2] = 3;
                  q1.push ({c1, {{x1,y1},{x2,y2}}});
              } else {

              }
          }

      }

  }




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
