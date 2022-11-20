#include <iostream>
#include <fstream>
#include <chrono>

#include <vector>
#include <queue>
#include <tuple>
#include <map>
#include <cmath>
#include <limits>

using namespace std;

using point = pair<int,int>;
//using vertex = tuple<int, int, vector<point>>;
enum state {none, vertical, horizontal};

const vector<char> alpha {'U','R','D','L'};
const vector<pair<int,int>> compas {{0,-1},{1,0},{0,1},{-1,0}};

struct vertex {
    int dist, state;
    vector<point> p;
    vector<vector<int>> visit;
    string route;
};

struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return a.dist > b.dist;
    }
};

class Display {

    public :

        static void point (point p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }
        static void board (vector<string> &floor, vector<vector<int>> &grid) {

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
          //cout << "\n";
      }
};

int distance (const point &a, const point &b) {
    return abs (a.first - b.first) + abs (a.second - b.second);
}
bool is_inside (const vector<string> &grid, int x, int y) {
    if (x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size()) {
        if (grid[y][x] != '0') return true;
    }
    return false;
}

string dijsktra (vector<string> level, vector<vector<int>> hist, point start, point exit) {

    vertex ver = {distance (start, exit), vertical, {start}, hist};
    priority_queue<vertex,vector<vertex>, comp> q1;
    map<vector<vector<int>>,bool> visited;

    q1.push (ver);

		int cycle = 0;

    while (!q1.empty()) {

        auto [dist, state, p, grid, route] = q1.top();
        q1.pop();

				visited[grid] = true;
				cycle++;

				if (cycle == 100) {
            Display::board (level, grid);
						break;
				}

        if (state == vertical && p[0] == exit) {
            Display::board (level, grid);
            cout << route << " => " << cycle << " moves\n";
            return route;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = compas[i].first, ny = compas[i].second;
            const int x1 = p[0].first + nx, y1 = p[0].second + ny;
            const int dx = (p[1].first - p[0].first), dy = (p[1].second - p[0].second);
            int x2, y2;

            if (state == vertical) {
                x2 = p[0].first + nx * 2, y2 = p[0].second + ny * 2;
            } else {
                x2 = p[1].first + nx, y2 = p[1].second + ny;
            }
            int alt = dist + 1;//int alt = distance ({x2, y2}, exit) + 1;

            if (is_inside (level, x2, y2) && is_inside (level, x1, y1)) {
                vertex nextv;
                auto path = grid;

                if (state == vertical || (dx != nx && dy != ny)) {
                    path[y2][x2] = horizontal;
                    path[y1][x1] = horizontal;
                    nextv = {alt, horizontal, {{x1,y1},{x2,y2}}, path, route + alpha[i]};
                } else {
                    if (dx == nx && dy == ny) {
                        path[y2][x2] = vertical;
                        nextv = {alt, vertical, {{x2,y2},{x2,y2}}, path, route + alpha[i]};
                    } else {
                        path[y1][x1] = vertical;
                        nextv = {alt, vertical, {{x1,y1},{x1,y1}}, path, route + alpha[i]};
                    }
                }

                if (!visited[path]) {
                    q1.push (nextv);
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
            char tile = level[y][x];

            if (tile == 'X') exit = {x,y};
            if (tile == 'B') start = {x,y};
        }
    }

    dijsktra (level, grid, start, exit);

    return "";
}

int main  () {

    auto start = chrono::high_resolution_clock::now();

    vector<string> level1 = {"1110000000","1B11110000","1111111110","0111111111","0000011X11","0000001110"};
    vector<string> level2 = {"000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"};
    vector<string> level3 = {"00011111110000","00011111110000","11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"};

		vector<string> level0 = {

		"001111111111111111111100",
		"001111111111111111111100",
		"001111111111111111111100",
		"001111111111111111111100",
		"00111111111B111111111100",
		"001111111111111111111100",
		"001111111111111111111100",
		"00111111111111111111X100",
		"001111111111111111111100",
		"000000000000000000000000"
  };

    blox_solver (level1);
    //     {"ULDRURRRRUURRRDDDRU","RURRRULDRUURRRDDDRU"},
    //display (floor, grid);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms\n" << endl;
}
void Test () {

  vector<vector<string>> level {

{"1110000000",
 "1B11110000",
 "1111111110",
 "0111111111",
 "0000011X11",
 "0000001110"},

{"000000111111100",
 "111100111001100",
 "111111111001111",
 "1B11000000011X1",
 "111100000001111",
 "000000000000111"},

{"00011111110000",
 "00011111110000",
 "11110000011100",
 "11100000001100",
 "11100000001100",
 "1B100111111111",
 "11100111111111",
 "000001X1001111",
 "00000111001111"},

{"11111100000",
 "1B111100000",
 "11110111100",
 "11100111110",
 "10000001111",
 "11110000111",
 "11110000111",
 "00110111111",
 "01111111111",
 "0110011X100",
 "01100011100"},

{"000001111110000",
 "000001001110000",
 "000001001111100",
 "B11111000001111",
 "0000111000011X1",
 "000011100000111",
 "000000100110000",
 "000000111110000",
 "000000111110000",
 "000000011100000"}
   };

vector<vector<string>> result =
    {{"RRDRRRD","RDDRRDR","RDRRDDR"},
    {"ULDRURRRRUURRRDDDRU","RURRRULDRUURRRDDDRU"},
    {"ULURRURRRRRRDRDDDDDRULLLLLLD"},
    {"DRURURDDRRDDDLD"},
    {"RRRDRDDRDDRULLLUULUUURRRDDLURRDRDDR","RRRDDRDDRDRULLLUULUUURRDRRULDDRRDDR","RRRDRDDRDDRULLLUULUUURRDRRULDDRRDDR"}};

}
/////////////////////////////deprecated///////////////////////////////
