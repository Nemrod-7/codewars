#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

struct Vertex {
    int x, y;
    int dist;
    int state;
    Vertex *next;
    //vector<struct Edge> ve;
};
int len, size;
const char PATH{'x'}, FREE{'.'},EXIT{'E'},WALL{'W'};
const vector<pair<int,int>> cardinal{{-1,0},{1,0},{0,-1},{0,1}};


void Test();
int path_finder(string maze) ;

int main () {

  auto start = chrono::high_resolution_clock::now();

  //path_finder(".");
  Test();

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  cout << "Process took " << elapsed.count()  << " ms" << endl;

}

bool is_inside (int pos) { return (pos >= 0 && pos < len);}
bool is_free (string maze, int pos) { return maze[pos] == FREE;}

int path_finder(string maze) {

    len = maze.size();
    size = maze.find('\n') + 1;
    if (len < 2) return 0;
    vector<vector<Vertex>> Vmap (size, vector<Vertex> (size));

    for (auto& c : maze) {
        Vmap[x][y] = {
            .x = x, .y = y,
            .dist = numeric_limits<int>::max(),
            .state = maze[c] == '#' ? WALL : FREE,
            .next = NULL };
        }
    }

    Vertex u, source{0,0,0};
    queue<Vertex> Q;

    Q.push(source);

    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        int alt = u.dist + 1;

        //if (u.x == maze.size()-1) break;

        for (auto pt : cardinal) {
            int coord = (pt.first + u.x) * (size) + (pt.second + u.x);
            if (is_inside (coord) && is_free (maze, coord)) {
                Q.push({pt.first, pt.second, alt});
                maze[coord] = PATH;
            }
        }
    }

    cout << maze << endl << endl;

    return -1;
}

void Test() {


  path_finder(".W.\n.W.\n...");
  path_finder(".W.\n.W.\nW..");
  path_finder("......\n......\n......\n......\n......\n......");
  path_finder("......\n......\n......\n......\n.....W\n....W.");
  path_finder("....W\n.W...\n.W...\nWW...\n...W.");
  path_finder("...W...W.W\nW..W...W..\nW..W....WW\nWWW..WW.WW\n.....WW.W.\n..WW.W...W\n.....W..W.\n.W........\nW...WWW...\n..W.W..W..");
  path_finder("......W.W.\n....W..W..\n..W.....W.\nW.......W.\n.WWWW.....\n.WW.WWW...\nWW........\nWWW......W\n......WWW.\nW..W...WW.");
  path_finder(".");

}
