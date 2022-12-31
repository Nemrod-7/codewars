#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

class Display {
    public :
        static void graph (const vector<vector<int>> &grid) {
    int height = grid.size(), width = grid[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = grid[y][x];
            if (cell < 0) {
                if (cell == -1) {
                    cout << "[" << setw(2) << " " << "]";
                } else {
                    cout << "[" << setw(2) << "x" << "]";
                }
            } else {
                cout << "[" << setw(2) << cell << "]";
            }
        }
        cout << endl;
    }
}
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }

};
using point = pair<int,int>;

struct vertex {
    int id;
    string path;
};

point operator+ (const point& a, const point& b) {
    return {a.first + b.first, a.second + b.second};
}

const vector<char> alp = {'E','S','W','N'};
const vector<pair<int,int>> dir {{1,0},{0,1},{-1,0},{0,-1}};

void rotate (vector<vector<int>> &grid) {
    int height = grid.size(), width = grid[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = grid[y][x];

            if (cell > 0 && cell < 15) {
                if (cell < 8) {
                    grid[y][x] *= 2;
                } else {
                    grid[y][x] -= (15 - grid[y][x]);
                }
                /*
                switch (cell) {
                    case 8  : grid[y][x] = 1; break; // -7
                    case 9  : grid[y][x] = 3; break; // -6
                    case 10 : grid[y][x] = 5; break; // -5
                    case 11 : grid[y][x] = 7; break; // -4
                    case 12 : grid[y][x] = 9; break; // -3
                    case 13 : grid[y][x] = 11; break;// -2
                    case 14 : grid[y][x] = 13; break;// -1
                    default : grid[y][x] *= 2; break;
                }
                */
            }
        }
    }
}
bool is_inside (point p, int width, int height) {
    return p.first >= 0 && p.second >= 0 && p.first < width && p.second < height;
}
vector<point> mkstack (vector<vector<vertex>> &graph) {
    vector<point> stack;

    for (int y = 0; y < graph.size(); y++) {
        for (int x = 0; x < graph[0].size(); x++) {
            if (graph[y][x].id) {
                stack.push_back({x,y});
                //graph[y][x].id = 0;
            }
        }
    }
    return stack;
}
string update (vector<vector<int>> &grid, vector<vector<vertex>> &graph) {

    int height = grid.size(), width = grid[0].size();
    vector<point> stack = mkstack (graph);

    while (!stack.empty()) {
        point curr = stack.back();
        auto [id, path] = graph[curr.second][curr.first];
        stack.pop_back();

        for (int i = 0; i < 4; i++) {
            int j = (i + 2) % 4;
            point nxp = curr + dir[i];

            if (is_inside (nxp, width, height)) {
                int cell = grid[curr.second][curr.first], next = grid[nxp.second][nxp.first];
                int visit = graph[nxp.second][nxp.first].id;
                bool out = (cell >> i&1), door = (next >> j&1);
                vertex *vtx = &graph[nxp.second][nxp.first];

                if (cell == -2)
                    return path + alp[i];

                if (out == 0 && door == 0 && visit == 0) {
                    vtx->id = 1;
                    vtx->path = path + alp[i];
                    stack.push_back(nxp);
                }
            }
        }
    }

    return "";
}

string maze_solver (vector<vector<int>> grid) {

    int height = grid.size(), width = grid[0].size();
    vector<vector<vertex>> graph (height, vector<vertex> (width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = grid[y][x];

            if (cell == -1) {
                graph[y][x].id = 1;
                grid[y][x] = 0;
            }
            //if (cell < 0) grid[y][x] = 0;
        }
    }
    int index = 5;

    while (index-->0) {
        string path = update (grid, graph);
        Display::graph (grid);
        cout << endl;
        if (path.size()) {
            cout << path << endl;
            break;
        }
        rotate (grid);
    }


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = graph[y][x].id;
            cout << cell;
        }
        cout << endl;
    }
    return "nullptr";
}

int main () {
  enum {east, south, west, north};

  vector<vector<int>> grid = {
      {  4,  2,  5,  4},
      {  4, 15, 11,  1},
      { -1,  9,  6,  8},
      { 12,  7,  7, -2}
  };
  // maze_solver(&example); // ["NNE", "EE", "S", "SS"] <- one possible solution
  maze_solver(grid);




  /*
  */

  //fillgrid(grid, visit, cycle);

  cout << "\nend\n";
}
