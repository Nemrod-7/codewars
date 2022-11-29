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

// -std=c++17 -O2 -Wall -o solver && ./solver

using point = pair<int,int>;
//using vertex = tuple<int, int, vector<point>>;
enum state {none, vertical, horizontal};
const vector<char> letter {'U','R','D','L'};
const vector<pair<int,int>> dir {{0,-1},{1,0},{0,1},{-1,0}};

struct vertex {
    int dist;
    vector<point> p;
    vector<string> hist;
    string route;
};

struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return a.dist > b.dist;
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display {

    public :

        static void point (point p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }
  		  static void board (vector<string> &floor) {

  					const int width = floor[0].size(), height = floor.size();

  					for (int y = 0; y < height; y++) {
  							for (int x = 0; x < width; x++) {
  									if (floor[y][x] == '0') {
  											cout << ' ';
  									} else if (floor[y][x]  == 'B') {
  											cout << 'B';
  									} else if (floor[y][x] == '1') {
  											cout << '.';
  									} else {
  											cout << (int) floor[y][x];
  									}
  							}
  							cout << "\n";
  					}
  					cout << "\n";
        }

};
class Assert {
		public :
				static void That (vector<string> act, vector<string> exp) {

				}
};
void Test ();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int distance (const point &a, const point &b) {
    return abs (a.first - b.first) + abs (a.second - b.second);
}
bool is_inside (const vector<string> &grid, int x, int y) {
    if (x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size()) {
        if (grid[y][x] != '0') return true;
    }
    return false;
}

string dijsktra (vector<string> level, point start, point exit) {

    vertex source = {0, {start,start}, level};
    priority_queue<vertex,vector<vertex>, comp> q1;
		map<vector<string>,bool> visited;

		q1.push (source);

		int cycle = 0;

		while (!q1.empty()) {
        auto [dist, p, past, route] = q1.top();
				int state = p[0] == p[1] ? vertical : horizontal;
        q1.pop();

				cycle++;

				if (cycle == 1800) {
						//cout << route << " => " << cycle << " cycles\n";
						break;
				}
        if (state == vertical && p[0] == exit) {
            /*
            Display::board (past);
						cout << route << " => " << cycle << " cycles\n";
            */
            return route;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = dir[i].first, ny = dir[i].second;
            const int dx = p[1].first - p[0].first, dy = p[1].second - p[0].second;

						const int x1 = p[0].first + nx, y1 = p[0].second + ny;
						const int x2 = (state == vertical) ? p[0].first + nx * 2 : p[1].first + nx;
						const int y2 = (state == vertical) ? p[0].second + ny * 2 : p[1].second + ny;

            const int alt = route.size() + distance ({x2,y2}, exit);
						const string path = route + letter[i];

            if (is_inside (level, x2, y2) && is_inside (level, x1, y1)) {
                vector<point> block;
                auto grid = past;
                int id;

                if (state == vertical || (dx != nx && dy != ny)) {
										id = abs (x2 - x1) != 0 ? 2 : 3;
                    block = {{x1,y1},{x2,y2}};
                } else {
                    id = vertical;
                    if (dx == nx && dy == ny) {
                        block = {{x2,y2},{x2,y2}};
                    } else {
                        block = {{x1,y1},{x1,y1}};
                    }
                }

                for (auto [x,y] : block)
                    grid[y][x] = id;

								if (!visited[grid]) {
										visited[grid] = true;
										q1.push ({alt, block, grid, path});
								}
            }
				}
		}

    return "";
}
string blox_solver (vector<string> level) {

    const int width = level[0].size(), height = level.size();

    pair<int,int> exit,  start;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char tile = level[y][x];

            if (tile == 'X') exit = {x,y};
            if (tile == 'B') start = {x,y};
        }
    }

    return dijsktra (level, start, exit);
}

int main  () {

    auto start = chrono::high_resolution_clock::now();

		vector<string> dojo = { "001111111111111111111100", "001111111111111111111100", "001111111111111111111100", "001111111111111111111100", "001X1111111B111111111100", "001111111111111111111100", "001111111111111111111100", "001111111111111111111100", "001111111111111111111100" };

		vector<string> level1 = {"1110000000", "1B11110000", "1111111110", "0111111111", "0000011X11", "0000001110"};
		vector<string> level2 = {"000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"};
		vector<string> level3 = { "00011111110000", "00011111110000", "11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"};

		Test();
    //display (floor, grid);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms\n" << endl;
}

void Test () {

  vector<vector<string>> level {
{"1110000000", "1B11110000", "1111111110", "0111111111", "0000011X11", "0000001110"},
{"000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"},
{"00011111110000", "00011111110000", "11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"},
{"11111100000", "1B111100000", "11110111100", "11100111110", "10000001111", "11110000111", "11110000111", "00110111111", "01111111111", "0110011X100", "01100011100"},
{"000001111110000", "000001001110000", "000001001111100", "B11111000001111", "0000111000011X1", "000011100000111", "000000100110000", "000000111110000", "000000111110000", "000000011100000"} };

vector<vector<string>> result =
    {{"RRDRRRD","RDDRRDR","RDRRDDR"},
    {"ULDRURRRRUURRRDDDRU","RURRRULDRUURRRDDDRU"},
    {"ULURRURRRRRRDRDDDDDRULLLLLLD"},
    {"DRURURDDRRDDDLD"},
    {"RRRDRDDRDDRULLLUULUUURRRDDLURRDRDDR","RRRDDRDDRDRULLLUULUUURRDRRULDDRRDDR","RRRDRDDRDDRULLLUULUUURRDRRULDDRRDDR"}};

		for (int i = 0; i < level.size(); i++) {

				cout << "test " << i << endl;
				string route = blox_solver(level[i]);

				bool flag = false;
				for (auto res : result[i]) {
						if (route == res) {
								flag = true;
								break;
						}
				}

				if (flag == false) {
						cout << "result : " << route << endl;
						cout << "expect : ";
						for (auto res : result[i]) cout << res << " ";
						cout << endl;
				}
		}

}

string dijsktra2 (vector<string> level, point start, point exit) {

    vertex source = {0, {start,start}, level};
    priority_queue<vertex,vector<vertex>, comp> q1;
		map<vector<string>,bool> visited;

		q1.push (source);

		int cycle = 0;

		while (!q1.empty()) {
        auto [dist, p, past, route] = q1.top();
				int state = p[0] == p[1] ? vertical : horizontal;
        q1.pop();

				cycle++;

				if (cycle == 1800) {
						//Display::board (level, past);
						//cout << route << " => " << cycle << " cycles\n";
						break;
				}
        if (state == vertical && p[0] == exit) {
						Display::board (past);
            /*
						cout << route << " => " << cycle << " cycles\n";
            */
            return route;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = dir[i].first, ny = dir[i].second;
            const int dx = p[1].first - p[0].first, dy = p[1].second - p[0].second;

						const int x1 = p[0].first + nx, y1 = p[0].second + ny;
						const int x2 = (state == vertical) ? p[0].first + nx * 2 : p[1].first + nx;
						const int y2 = (state == vertical) ? p[0].second + ny * 2 : p[1].second + ny;

            int alt = route.size() + distance ({x2,y2}, exit);
						string path = route + letter[i];

            if (is_inside (level, x2, y2) && is_inside (level, x1, y1)) {
                vertex nextv;
                vector<point> block;
                auto grid = past;
                int id;

                if (state == vertical || (dx != nx && dy != ny)) {
										id = abs (x2 - x1) != 0 ? 2 : 3;
                    block = {{x1,y1},{x2,y2}};

                    grid[y2][x2] = id; // horizontal;
                    grid[y1][x1] = id; // horizontal;
                    nextv = {alt, {{x1,y1},{x2,y2}}, grid, path};
                } else {
                    if (dx == nx && dy == ny) {
                        id = vertical;
                        grid[y2][x2] = vertical;
                        block = {{x2,y2},{x2,y2}};
                        nextv = {alt, {{x2,y2},{x2,y2}}, grid, path};
                    } else {
                        id = vertical;
                        grid[y1][x1] = vertical;
                        block = {{x1,y1},{x1,y1}};
                        nextv = {alt, {{x1,y1},{x1,y1}}, grid, path};
                    }
                }

								if (!visited[grid]) {
										//Display::board (level, grid);
                    for (auto it : block) {
                        grid[it.second][it.first] = id;
                    }
										visited[grid] = true;
										q1.push (nextv);
								}
            }
				}
		}

    return "";
}
