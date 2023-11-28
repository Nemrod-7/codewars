#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

//using point = pair<int,int>;
struct point {int x,y; };

struct vertex {
    int id;
    vector<string> path;
};

class Display {
    public :
        static string board (const vector<vector<int>> &grid) {
            int height = grid.size(), width = grid[0].size();
            stringstream os;
            os << '\n';
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int cell = grid[y][x];
                    if (cell < 0) {
                        if (cell == -1) {
                            os << "[" << setw(2) << "-" << "]";
                        } else {
                            os << "[" << setw(2) << "+" << "]";
                        }
                    } else {
                        os << "[" << setw(2) << cell << "]";
                    }
                }
                os << endl;
            }

            return os.str();
        }
        static void point (const point &p) {
            cout << "[" << p.x << "," << p.y << "]\n";
        }
        static string grph (const vector<vector<vertex>> &graph) {
            stringstream os;
            int height = graph.size(), width = graph[0].size();

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    //os += to_string(graph[y][x].id);
                    os << "[" << setw(2) << graph[y][x].id << "]";
                }
                os << '\n';
            }

            return os.str();
        }
};

point operator + (const point& a, const point& b) { return {a.x + b.x, a.y + b.y}; }
bool is_inside (point p, int width, int height) { return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height; }

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
            }
        }
    }
}

bool update (vector<vector<int>> &grid, vector<vector<vertex>> &graph, int cycle) {
    const int height = grid.size(), width = grid[0].size();
    vector<point> stack;

    for (int y = 0; y < graph.size(); y++) {
        for (int x = 0; x < graph[0].size(); x++) {
            if (graph[y][x].id <= cycle) {
                stack.push_back({x,y});
                graph[y][x].path.push_back("");
            }
        }
    }

    while (!stack.empty()) {
        point curr = stack.back();
        auto [id, path] = graph[curr.y][curr.x];
        int cell = grid[curr.y][curr.x];
        stack.pop_back();

        if (cell == -2)
            return true;

        for (int i = 0; i < 4; i++) {
            int j = (i + 2) % 4;
            point nxp = curr + dir[i];

            if (is_inside (nxp, width, height)) {
                int next = grid[nxp.y][nxp.x];
                bool out = cell < 0 ? 0 : (cell >> i&1), door = next < 0 ? 0 : (next >> j&1);
                vertex *vtx = &graph[nxp.y][nxp.x];

                if (out == 0 && door == 0 && vtx->id == 99) {
                    vtx->id = cycle + 1;
                    vtx->path = path;
                    vtx->path.back() += alp[i];
                    stack.push_back(nxp);
                }
            }
        }
    }

    return false;
}
vector<string> maze_solver (vector<vector<int>> grid) {

    int cycle = 0;
    const int height = grid.size(), width = grid[0].size();
    point exit;
    vector<vector<vertex>> graph (height, vector<vertex> (width, {99}));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = grid[y][x];

            if (cell == -1) graph[y][x].id = 0;
            if (cell == -2) exit = {x,y};
        }
    }

    cout << Display::board(grid) << endl;

    while (cycle < 20) {
        if (update(grid, graph, cycle) == true) {
            // for (string &route : graph[exit.y][exit.x].path) {
            //     cout << "[" << route << "]";
            // }
            return graph[exit.y][exit.x].path;
        }
        rotate (grid);
        cycle++;
    }

    cout << endl;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int cell = graph[y][x].id;
            cout << "[" << setw(2) << cell << "]";
        }
        cout << endl;
    }
    /*
    */

    return {};
}
void dotest (pair<vector<vector<int>>, vector<string>> test) {
    vector<vector<int>> board = test.first;
    vector<string> exp = test.second, res = maze_solver(board);

    string h1, h2;
    for (auto it : exp) h1 += it;

    for (auto it : res) h2 += it;

    if (h1 != h2) {
        cout << h1 << " " << h2 << "\n";
    }

}

int main () {

    vector<vector<int>> grid;

    grid = {
        {  4,  2,  5,  4},
        {  4, 15, 11,  1},
        { -1,  9,  6,  8},
        { 12,  7,  7, -2}};


    for (int i = 0; i < 4; i++) {
        cout << Display::board(grid) << endl;
        rotate (grid);
    }

    cout << "\nend\n";
    grid = {
        { 9,12, 7, 6, 9, 1, 1,15, 5, 1},
        {13, 5, 4, 9,11,13, 2,11, 4,13},
        {14, 7, 5,10,15, 8, 5, 9, 7, 6},
        { 5, 6, 5, 1,13,10, 9,10, 7,10},
        {10, 4, 1, 9,13,15, 0, 4,11,15},
        {-1, 5,11, 5,14, 7,13, 8, 9,12},
        { 9,10, 0,13, 5, 3, 6, 9, 2, 3},
        {14, 4, 4, 2,15, 9, 2,11, 2,-2}};

    grid ={
        { 7, 2,15,14,11, 5,13,13},
        { 2, 5, 3,13,13,10, 4,14},
        {-1,14, 7, 3,14, 8, 7,12},
        {11, 5, 2, 7, 6, 5, 7,-2},
        {14,13, 7, 5, 0, 4, 5,13} };

    // maze_solver(grid);
}
////////////////////////////////////////////////////////////////////////////////
void Test () {

    /* rotation rule :
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
    vector<vector<int>> grid = {
        {  4,  2,  5,  4},
        {  4, 15, 11,  1},
        { -1,  9,  6,  8},
        { 12,  7,  7, -2}
    };

    vector<pair<vector<vector<int>>, vector<string>>> test {
        {{ {4,2,5,4}, {4,15,11,1}, {-1,9,6,8}, {12,7,7,-2} }, {"NNE", "EE", "S", "SS"}},
            {{ {6,3,10,4,11}, {8,10,4,8,5}, {-1,14,11,3,-2}, {15,3,4,14,15}, {14,7,15,5,5} }, {"", "", "E", "", "E", "NESE"}},
            {{ {9,1,9,0,13,0}, {14,1,11,2,11,4}, {-1,2,11,0,0,15}, {4,3,9,6,3,-2} }, {"E", "SE", "", "E", "E", "E"}},
            {{ {-1,6,12,15,11}, {8,7,15,7,10}, {13,7,13,15,-2}, {11,10,8,1,3}, {12,6,9,14,7} }, {}},
            {{ {6,3,0,9,14,13,14}, {-1,14,9,11,15,14,15}, {2,15,0,12,6,15,-2}, {4,10,7,6,15,5,3}, {7,3,13,13,14,7,0} }, {}}
    };

}
