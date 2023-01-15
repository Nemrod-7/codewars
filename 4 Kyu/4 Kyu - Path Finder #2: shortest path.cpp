#include <iostream>
#include <string>
#include <queue>

#include <chrono>

using namespace std;

void Test();
//const char PATH{'x'}, FREE{'.'},EXIT{'E'},WALL{'W'};
using point = pair<int,int>;

bool is_free (string &maze, int pos) { return pos >= 0 && pos < maze.size() && maze[pos] == '.'; }
int path_finder(string maze) {

    if (maze.size() < 2) return 0;
    int sze = maze.find('\n') + 1;
    const int directions[4] = {-1, 1, -sze, sze};
    priority_queue<point, vector<point>, greater<point>> q1;

    q1.push({0,0});

    while (!q1.empty()) {
        auto &[dist, coord] = q1.top();
        q1.pop();
        int alt = dist + 1;
        maze[coord] = 'x';
        if (coord == maze.size() - 1) return dist;

        for (int pos : directions) {
            int next = pos + coord;

            if (is_free (maze, next)) {
                q1.push ({alt, next});
            }
        }
    }

    return -1;
}

int main () {

  auto start = chrono::high_resolution_clock::now();

  // path_finder(".W.\n.W.\n...");
  Test();

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  cout << "Process took " << elapsed.count()  << " ms" << endl;
}

void Test() {


  path_finder(".WE\n.W.\n...");
  path_finder(".W.\n.W.\nW..");
  path_finder("......\n......\n......\n......\n......\n......");
  path_finder("......\n......\n......\n......\n.....W\n....W.");
  path_finder("....W\n.W...\n.W...\nWW...\n...W.");
  path_finder("...W...W.W\nW..W...W..\nW..W....WW\nWWW..WW.WW\n.....WW.W.\n..WW.W...W\n.....W..W.\n.W........\nW...WWW...\n..W.W..W..");
  path_finder("......W.W.\n....W..W..\n..W.....W.\nW.......W.\n.WWWW.....\n.WW.WWW...\nWW........\nWWW......W\n......WWW.\nW..W...WW.");
  path_finder(".");

}
