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
bool is_free (string maze, int coord) { return maze[coord] == FREE;}

int path_finder(string maze) {

    len = maze.size();
    size = maze.find('\n') + 1;
    if (len < 2) return 0;

    const int directions[4] = {-1, 1, -size, size};
    const vector<pair<int,int>> cardinal{{-1,0},{1,0},{0,-1},{0,1}};

    pair<int,int> u, source{0,0};
    queue <pair<int,int>> Q;

    Q.push(source);

    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        int alt = u.second + 1;

        if (u.first == maze.size()-1) break;

        for (int pos : directions) {
            int next = pos + u.first;
            if (is_inside (next) && is_free (maze, next)) {
                Q.push({next,alt});
                maze[next] = PATH;
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
