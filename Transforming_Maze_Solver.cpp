#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

using point = pair<int,int>;

struct vertex {
    int id;
    vector<string> path;
};

class Display {
    public :
        static string graph (const vector<vector<int>> &grid) {
            int height = grid.size(), width = grid[0].size();
            stringstream os;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int cell = grid[y][x];
                    if (cell < 0) {
                        if (cell == -1) {
                            os << "[" << setw(2) << " " << "]";
                        } else {
                            os << "[" << setw(2) << "x" << "]";
                        }
                    } else {
                        os << "[" << setw(2) << cell << "]";
                    }
                }
                os << endl;
            }

            return os.str();
        }
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << "," << p.second << "]\n";
        }
        static string grph (const vector<vector<vertex>> &graph) {
            string os;
            int height = graph.size(), width = graph[0].size();

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    os += to_string(graph[y][x].id);
                }
                os += '\n';
            }

            return os;
        }

};

point operator+ (const point& a, const point& b) {
    return {a.first + b.first, a.second + b.second};
}

const vector<string> alp= {"E","S","W","N"};
const vector<point> dir {{1,0},{0,1},{-1,0},{0,-1}};

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

bool update2 (vector<vector<int>> &grid, vector<vector<vertex>> &graph, int move) {

    int height = grid.size(), width = grid[0].size();
    vector<point> stack;

    for (int y = 0; y < graph.size(); y++) {
        for (int x = 0; x < graph[0].size(); x++) {
            if (graph[y][x].id == 0) {
                stack.push_back({x,y});
                graph[y][x].path.push_back("");
            }
        }
    }

    while (!stack.empty()) {
        point curr = stack.back();
        auto [id, path] = graph[curr.second][curr.first];
        int cell = grid[curr.second][curr.first];
        stack.pop_back();

        if (cell == -2)
            return true;

        for (int i = 0; i < 4; i++) {
            int j = (i + 2) % 4;
            point nxp = curr + dir[i];

            if (is_inside (nxp, width, height)) {
                int next = grid[nxp.second][nxp.first];
                bool out = cell < 0 ? 0 : (cell >> i&1), door = next < 0 ? 0 : (next >> j&1);
                vertex *vtx = &graph[nxp.second][nxp.first];

                if (out == 0 && door == 0 && vtx->id == 9) {
                    vtx->id = 0;
                    vtx->path = path;
                    vtx->path.back() += alp[i];
                    stack.push_back(nxp);
                }
            }
        }
    }

    return false;
}
string maze_solver (vector<vector<int>> grid) {

    int height = grid.size(), width = grid[0].size();
    vector<vector<vertex>> graph (height, vector<vertex> (width, {9}));
    point exit;
    int move = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = grid[y][x];

            if (cell == -1) graph[y][x].id = 0;
            if (cell == -2) exit = {x,y};
        }
    }

    while (true) {
        if (update2(grid, graph, move) == true) {
            break;
        }

        rotate (grid);
        move++;
    }

    cout << Display::graph(grid) << endl;
    for (string &route : graph[exit.second][exit.first].path) {
        cout << "["<<route<<"]";
    }

    /*
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = graph[y][x].id;
            cout << cell;
        }
        cout << endl;:exit

    }
    */

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


  cout << "\nend\n";
}
////////////////////////////////////////////////////////////////////////////////
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
bool update (vector<vector<int>> &grid, vector<vector<vertex>> &graph, int move) {

    int height = grid.size(), width = grid[0].size();
    vector<point> stack ; //= mkstack (graph);

    for (int y = 0; y < graph.size(); y++) {
        for (int x = 0; x < graph[0].size(); x++) {
            if (graph[y][x].id == move) {
                stack.push_back({x,y});
            }
        }
    }

    while (!stack.empty()) {
        point curr = stack.back();
        auto [id, path] = graph[curr.second][curr.first];
        int cell = grid[curr.second][curr.first];
        stack.pop_back();

        if (cell == -2)
            return true;

        for (int i = 0; i < 4; i++) {
            int j = (i + 2) % 4;
            point nxp = curr + dir[i];

            if (is_inside (nxp, width, height)) {
                int next = grid[nxp.second][nxp.first];
                bool out = cell < 0 ? 0 : (cell >> i&1), door = next < 0 ? 0 : (next >> j&1);
                vertex *vtx = &graph[nxp.second][nxp.first];

                if (out == 0 && door == 0 && vtx->id == 9) {
                    vector<string> route = path;

                    if (route.size() == move) {
                        route.push_back(alp[i]);
                    } else {
                        route[move] += alp[i];
                    }

                    vtx->id = move + 1;
                    vtx->path = route;
                    stack.push_back(nxp);
                }
            }
        }
    }

    return false;
}

void Test () {

    using grid = vector<vector<int>>;

    pair<vector<vector<int>>, vector<string>> test;

    test = {{ {4,2,5,4}, {4,15,11,1}, {-1,9,6,8}, {12,7,7,-2} }, {"NNE", "EE", "S", "SS"}};
    test = {{ {6,3,10,4,11}, {8,10,4,8,5}, {-1,14,11,3,-2}, {15,3,4,14,15}, {14,7,15,5,5} }, {"", "", "E", "", "E", "NESE"}};
    test = {{ {9,1,9,0,13,0}, {14,1,11,2,11,4}, {-1,2,11,0,0,15}, {4,3,9,6,3,-2} }, {"E", "SE", "", "E", "E", "E"}};
    test = {{ {-1,6,12,15,11}, {8,7,15,7,10}, {13,7,13,15,-2}, {11,10,8,1,3}, {12,6,9,14,7} }, {}};
    test = {{ {6,3,0,9,14,13,14}, {-1,14,9,11,15,14,15}, {2,15,0,12,6,15,-2}, {4,10,7,6,15,5,3}, {7,3,13,13,14,7,0} }, {}};

}
