#include <iostream>
#include <iomanip>
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

enum state {none, raised, vertical, horizontal};
const vector<char> letter {'U','R','D','L'};
const vector<pair<int,int>> dir {{0,-1},{1,0},{0,1},{-1,0}};

struct vertex {
    double dist;
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
				static void depth (vector<vector<int>> depth) {
						 for (int y = 0; y < depth.size(); y++) {
								 for (int x = 0; x < depth[y].size(); x++) {

										 cout << setw(3) << depth[y][x] << ' ';
								 }
								 cout << endl;
						 }

				}
				static void wrfile (vector<string> level, string name) {

						ofstream ofs (name + ".csv");

						for (int y = 0; y < level.size(); y++) {
								for (int x = 0; x < level[y].size(); x++) {
										if (level[y][x])
												ofs << level[y][x] << ";";
										else
												ofs << " " << ";";
								}
								ofs << "\n";
						}
						ofs.close();
				}
};
class Assert {
		public :
				static void That (vector<string> act, vector<string> exp) {

				}
};
void Test ();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double distance (const point &a, const point &b) {
		return abs (a.first - b.first) + abs (a.second - b.second);
		return hypot (a.first - b.first, a.second - b.second);
}
bool is_inside (const vector<string> &grid, int x, int y) {
    if (x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size()) {
        if (grid[y][x] != '0') return true;
    }
    return false;
}
string dijsktra (vector<string> level, point start, point exit) {

    priority_queue<vertex,vector<vertex>, comp> q1;
		//map<vector<string>,bool> visited;
		map<point,bool> visited;
		level[start.second][start.first] = 1;
		vertex source = {0, {start}, level};
		q1.push (source);

		int cycle = 0;

		while (!q1.empty()) {
        auto [dist, p, past, route] = q1.top();
        q1.pop();

				cycle++;
				//Display::point(p[0]);
				if (cycle == 1800) {
						Display::board (past);
						//cout << route << " => " << cycle << " cycles\n";
						break;
				}
        if (p[0] == exit) {
						Display::board (past);
						cout << route << " => " << cycle << " cycles\n";
            return route;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = dir[i].first, ny = dir[i].second;
						const int x1 = p[0].first + nx, y1 = p[0].second + ny;

            double alt = route.size() + distance ({x1,y1}, exit);
						string path = route + letter[i];

            if (is_inside (level, x1, y1)) {

								point node = {x1,y1};
								auto grid = past;
								grid[y1][x1] = 1;

								if (!visited[node]) {
										visited[node] = true;
										vertex nextv = {alt, {{x1,y1}}, grid, path};
										q1.push (nextv);
								}
            }
				}
		}

    return "";
}
vertex a_star3 (vector<string> level, point start, point exit) {

    vertex source = {0, {start,start}, level};
    priority_queue<vertex,vector<vertex>, comp> q1;
		map<vector<string>,bool> visited;
		vector<vector<int>> depth (level.size(), vector<int> (level[0].size()));

		q1.push (source);

		int cycle = 0;

		while (!q1.empty()) {
				vertex curr = q1.top();
        auto [dist, p, past, route] = curr;
				int state = p[0] == p[1] ? raised : 2;
        q1.pop();

				cycle++;

				if (cycle == 2500) {
						//source = curr;
						break;
				}
        if (state == raised && p[0] == exit) {
            return curr;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = dir[i].first, ny = dir[i].second;
            const int dx = p[1].first - p[0].first, dy = p[1].second - p[0].second;

						const int x1 = p[0].first + nx, y1 = p[0].second + ny;
						const int x2 = (state == raised) ? p[0].first + nx * 2 : p[1].first + nx;
						const int y2 = (state == raised) ? p[0].second + ny * 2 : p[1].second + ny;


            if (is_inside (level, x2, y2) && is_inside (level, x1, y1)) {

								int id;
								auto grid = past;
								vector<point> block;
								const string path = route + letter[i];
								const double alt = route.size() /* +  distance ({x2,y2}, exit) */  ;
																																					 
                if (state == raised || (dx != nx && dy != ny)) {
										id = abs (x2 - x1) != 0 ? 2 : 3;
                    block = {{x1,y1},{x2,y2}};
                } else {
                    id = raised;
                    if (dx == nx && dy == ny) {
                        block = {{x2,y2},{x2,y2}};
                    } else {
                        block = {{x1,y1},{x1,y1}};
                    }
                }

                for (auto [x,y] : block)
                    grid[y][x] = id;

								int dep = depth[y2][x2];
								if (!visited[grid]  ) {

										depth[y1][x1]++;
										depth[y2][x2]++;
										visited[grid] = true;
										q1.push ({alt, block, grid, path});
								}
            }
				}
		}
						cout << cycle << "unsolved => \n";
						Display::depth (depth);

    return source;
}
string a_star2 (vector<string> level, point start, point exit) {

		priority_queue<vertex,vector<vertex>, comp> q1;
		//map<vector<string>, bool> visited;
		level[start.second][start.first] = 1;
		vertex source = {0, {start,start}, level};

		vector<vector<string>> visited;
		for (int i = 0; i < 4; i++) {
				visited.push_back(level);
		}

		q1.push (source);

		int cycle = 0;

		while (!q1.empty()) {
				auto [dist, p, past, route] = q1.top();
				int state = p[0] == p[1] ? raised : 2;
				q1.pop();

				cycle++;

				Display::board (past);
				if (cycle == 15) {
						cout << route << " => " << cycle << " cycles\n";
						//break;
				}
				if (state == raised && p[0] == exit) {
						Display::board (past);
						cout << route << " => " << cycle << " cycles\n";
						return route;
				}

				for (int i = 0; i < 4; i++) {
						const int nx = dir[i].first, ny = dir[i].second;
						const int dx = p[1].first - p[0].first, dy = p[1].second - p[0].second;

						const int x1 = p[0].first + nx, y1 = p[0].second + ny;
						const int x2 = (state == raised) ? p[0].first + nx * 2 : p[1].first + nx;
						const int y2 = (state == raised) ? p[0].second + ny * 2 : p[1].second + ny;

						const double alt = route.size() + distance ({x2,y2}, exit);
						const string path = route + letter[i];

						if (is_inside (level, x2, y2) && is_inside (level, x1, y1)) {
								vector<point> block;
								auto grid = past;
								int id;

								if (state == raised || (dx != nx && dy != ny)) {
										id = abs (x2 - x1) != 0 ? 2 : 3;
										block = {{x1,y1},{x2,y2}};
								} else {
										id = raised;
										if (dx == nx && dy == ny) {
												block = {{x2,y2},{x2,y2}};
										} else {
												block = {{x1,y1},{x1,y1}};
										}
								}

								int valid = 0;
								for (auto [x,y] : block) {
										if (visited[id][y][x] == id) {
												valid++;
										}
								}

								if (cycle == 15) {
										//Display::board(visited[id]);
										//cout << valid << ' ';
								}
								if (valid != 2) {
										for (auto [x,y] : block) {
												visited[id][y][x] = id;
												grid[y][x] = id;
										}
										q1.push ({alt, block, grid, path});
								}
						}
				}
		}


		return "";
}

vertex a_star (vector<string> level, point start, point exit) {

    vertex source = {0, {start,start}, level};
    priority_queue<vertex,vector<vertex>, comp> q1;
		map<vector<point>,bool> visited;

		vector<vector<int>> depth (level.size(), vector<int> (level[0].size()));

		q1.push (source);

		int cycle = 0;

		while (!q1.empty()) {
				vertex curr = q1.top();
        auto [dist, p, past, route] = curr;
				int state = p[0] == p[1] ? raised : 2;
        q1.pop();

				cycle++;

				if (cycle == 2500) {
						//source = curr;
						break;
				}
        if (state == raised && p[0] == exit) {
            return curr;
        }

        for (int i = 0; i < 4; i++) {
            const int nx = dir[i].first, ny = dir[i].second;
            const int dx = p[1].first - p[0].first, dy = p[1].second - p[0].second;

						const int x1 = p[0].first + nx, y1 = p[0].second + ny;
						const int x2 = (state == raised) ? p[0].first + nx * 2 : p[1].first + nx;
						const int y2 = (state == raised) ? p[0].second + ny * 2 : p[1].second + ny;


            if (is_inside (level, x2, y2) && is_inside (level, x1, y1)) {

								int id;
								auto grid = past;
								vector<point> block;
								const string path = route + letter[i];
								const double alt = route.size() /* +  distance ({x2,y2}, exit) */  ;
																																					 
                if (state == raised || (dx != nx && dy != ny)) {
										id = abs (x2 - x1) != 0 ? 2 : 3;
                    block = {{x1,y1},{x2,y2}};
                } else {
                    id = raised;
                    if (dx == nx && dy == ny) {
                        block = {{x2,y2},{x2,y2}};
                    } else {
                        block = {{x1,y1},{x1,y1}};
                    }
                }

                for (auto [x,y] : block)
                    grid[y][x] = id;

								int dep = depth[y2][x2];
								if (!visited[block]  ) {

										depth[y1][x1]++;
										depth[y2][x2]++;
										visited[block] = true;
										q1.push ({alt, block, grid, path});
								}
            }
				}
		}
						cout << cycle << "unsolved => \n";
						Display::depth (depth);

    return source;
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

    //return dijsktra(level, start, exit);
    return a_star (level, start, exit).route;
}
vertex blox_solverdeb (vector<string> level) {

    const int width = level[0].size(), height = level.size();

    pair<int,int> exit,  start;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char tile = level[y][x];

            if (tile == 'X') exit = {x,y};
            if (tile == 'B') start = {x,y};

        }
    }

    //return dijsktra(level, start, exit);
    return a_star (level, start, exit);
}

int main  () {

    auto start = chrono::high_resolution_clock::now();

		vector<string> level2 = {"000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"};

		vector<string> level3 = {"00011111110000", "00011111110000", "11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"};
		vector<string> level5 = {"01100000000000000000", "11110000011001111111", "11111111111001111111", "1B111111111001110011", "11110000011111110011", "11110000011111110011", "00000000000000000011", "11111111101110011011", "11X11011101111111111", "11110011111110011000"};

	//	blox_solver (level5);
		Test();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms\n" << endl;
}

bool dotest (string route, vector<string> result) {

		for (auto res : result) {
				if (route == res) {
						return true;
				}
		}

		return false;
}

void Test () {

  vector<vector<string>> level {
		{"1110000000", "1B11110000", "1111111110", "0111111111", "0000011X11", "0000001110"},
		{"000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"},
		{"00011111110000", "00011111110000", "11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"},
		{"11111100000", "1B111100000", "11110111100", "11100111110", "10000001111", "11110000111", "11110000111", "00110111111", "01111111111", "0110011X100", "01100011100"},
		{"000001111110000", "000001001110000", "000001001111100", "B11111000001111", "0000111000011X1", "000011100000111", "000000100110000", "000000111110000", "000000111110000", "000000011100000"},
		{"01100000000000000000", "11110000011001111111", "11111111111001111111", "1B111111111001110011", "11110000011111110011", "11110000011111110011", "00000000000000000011", "11111111101110011011", "11X11011101111111111", "11110011111110011000"},
		{"00000000011110000", "00000011111111100", "00000011111001110", "11101111100001111", "1B1111100000011X1", "11101111000000111", "00000011100110110", "00000011111111110", "00000001111111100"}
};

vector<vector<string>> result =
    {{"RRDRRRD","RDDRRDR","RDRRDDR"},
    {"ULDRURRRRUURRRDDDRU","RURRRULDRUURRRDDDRU"},
    {"ULURRURRRRRRDRDDDDDRULLLLLLD"},
    {"DRURURDDRRDDDLD"},
    {"RRRDRDDRDDRULLLUULUUURRRDDLURRDRDDR","RRRDDRDDRDRULLLUULUUURRDRRULDDRRDDR","RRRDRDDRDDRULLLUULUUURRDRRULDDRRDDR"},
		{"RRRRRRDRRRURURRRDDDDLLLLLDLURDLLLUULLDL","RRRRRRDRRRURURRRDDDDLLLLLDLLLURDLUULLDL"},
		{"RRRDRDRDRURRRURU","RRRDRDRDRRRURURU"}
		};


		for (int i = 0; i < level.size(); i++) {
				vertex node = blox_solverdeb(level[i]);

				if (dotest (node.route, result[i]) == false) {
						cout << "level " << i + 1 << endl;
						Display::board(node.hist);
						cout << "result : " << node.route << endl;
						cout << "expect : ";
						for (auto res : result[i]) cout << res << " ";
						cout << endl;
				}
		}

}
/*


*/
