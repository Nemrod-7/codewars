#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#include <iomanip>
#include <chrono>
#include <thread>

#include "../../templates/Assert.hpp"

using namespace std;

using point = pair<int,int>;

point operator+ (const point& a, const point& b) {
    return {a.first + b.first, a.second + b.second};
}
void operator+= (pair<int,int> &a, const pair<int,int> &b) {
    a.first += b.first, a.second += b.second;
}

const char wall{'#'}, start{'S'}, stop{'E'}, ice{' '}, ground{'x'};
const vector<pair<pair<int,int>,char>> compass {{{-1,0},'l'},{{1,0},'r'},{{0,-1},'u'},{{0,1},'d'}};

struct vertex {
    int dist, move;
    pair<int,int> p;
    bool visit;
    string route;
    bool operator< (const vertex &x) const {
          if (x.move == 0) return true;
          return (move == x.move) ? dist < x.dist : move < x.move;
    }
};
struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return (a.move == b.move) ?  a.dist > b.dist : a.move > b.move;
    }
};

class graph {
    private :
        string maze;
    public :
        int width, height;
        point enter, exit;

        graph (string src = {}) : maze(src) {
            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;

            int d1 = src.find (start);
            enter = make_pair (d1 % width, d1 / width), maze[d1] = ice;

            int d2 = src.find (stop);
            exit = make_pair (d2 % width, d2 / width), maze[d2] = ground;
        }

        bool is_inside (const point &p) {
            return p.first >= 0 && p.second >= 0 && p.first < width - 1 && p.second < height;
        }
        bool is_free (const point &p) { return this->operator[](p) != wall; }
        char operator[] (const point &p) { return is_inside(p) ? maze[p.second * width + p.first] : wall; }
};

vector<char> ice_maze_solver (string src) {

    graph maze (src);

    priority_queue <vertex, vector<vertex>, comp> q1;
    map <pair<int,int>, vertex> vmap;

    q1.push ({0,0, maze.enter, false});

    while (!q1.empty()) { // (!q1.empty())
        auto [dist, move, u, visit, route] = q1.top();
        q1.pop();

        if (u == maze.exit)
            return { route.begin(), route.end() };

        visit = true;

        for (auto [direction, alpha] : compass) {
            point nxt = u + direction, tmp = nxt;
            int alt = dist + 1; // float alt = distance (nxt, maze.exit);

            while (maze[tmp] == ice && !vmap[tmp].visit) {
                alt++;
                tmp += direction;
                if (maze[tmp] != wall) nxt = tmp;
            }

            vertex nxtv = {alt, move + 1, nxt, false, route + alpha};

            if (maze.is_free (nxt) && !vmap[nxt].visit && nxtv < vmap[nxt]) {
                vmap[nxt] = nxtv;
                q1.push (nxtv);
            }
        }
    }

    return {};
}

class Display {
  public :

  static void visited (graph &G) {
    //cout << "\033c";
    int row = G.height + 1, col = G.width;
    for (int y = -1; y < row; y++) {
      for (int x = -1; x < col; x++) {
        /*
        if (G.visit[{x,y}])
        cout << ". ";
        else
        cout << G[{x,y}] << ' ';
        */
      }
      cout << endl;
    }
    cout << endl;
    //this_thread::sleep_for(500ms);
  }
  static void graph2 (graph &G) {
    int row = G.height + 1, col = G.width;
    for (int y = -1; y < row; y++) {
      for (int x = -1; x < col; x++) {
        cout << G[{x,y}] << ' ';
      }
      cout << endl;
    }
  }

  static void locate (graph &G, point p) {
    int row = G.height + 1, col = G.width;
    //cout << "\033c";
    for (int y = -1; y < row; y++) {
      for (int x = -1; x < col; x++) {
        if (p == pair<int,int> (x,y))
        cout << "* ";
        else
        cout << G[{x,y}] << ' ';
      }
      cout << endl;
    }
    cout << endl;
    this_thread::sleep_for(500ms);
  }

  template<class T> static void showqueue (T q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
      auto [dist, move, p] = q.top();
      cout << "[" << move << ' ' << setprecision(2) << dist << "  <" << p.first << ' ' << p.second << ">] ";
      q.pop();
    }
    std::cout << '\n';
  }

  static void point (point p) {
    cout << "[" << p.first << "," << p.second << "]\n";
  }
};

int main () {

    string map;

    Test();

    cout << "finished";
}

float distance (const point &a, const point &b) {
  //return hypot (a.first - b.first, a.second - b.second); // euclidian distance
  return abs (a.first - b.first) + abs (a.second - b.second); //  manathan distance
  return max (abs(a.first - b.first), abs (a.second - b.second)); // diagonal distance
}

void Test() {

  std::string map;

    map = "    x \n"
          "  #   \n"
          "   E  \n"
          " #    \n"
          "    # \n"
          "S    #";
    //std::cout << "A simple spiral" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','r','d','l','u','r'})));

    map = " #    \n"
          "x   E \n"
          "      \n"
          "     S\n"
          "      \n"
          " #    ";
    //std::cout << std::endl << "Slippery puzzle has one-way routes" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','r'})));

    map = "E#    \n"
          "      \n"
          "      \n"
          "      \n"
          "      \n"
          " #   S";
    //std::cout << std::endl << "The end is unreachable" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>()));


    map = "E#   #\n"
          "      \n"
          "#     \n"
          "  #   \n"
          " #    \n"
          " S    ";
    //std::cout << std::endl << "Tiebreak by least number of moves first" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'r','u','l','u'})));


    map = "    E \n"
          "     #\n"
          "      \n"
          "# #   \n"
          "    # \n"
          " #  S ";
    //std::cout << std::endl << "Then by total distance traversed" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','r','u','r'})));

    map = "E#xxx\n"
          "x#x#x\n"
          "x#x#x\n"
          "x#x#x\n"
          "xxx#S";
    //std::cout << std::endl << "Covering all tiles with x should reduce the problem to simple pathfinding" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({ 'u','u','u','u','l','l','d','d','d','d','l','l','u','u','u','u'})));

    map = {32,32,32,32,32,32,32,32,35,32,32,32,32,32,10,32,32,32,35,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,35,32,32,32,32,10,32,35,32,32,32,32,32,32,32,32,32,32,32,32,10,35,32,32,32,32,32,32,32,35,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,35,10,32,32,32,32,32,32,35,32,32,32,32,32,32,32,10,32,32,35,32,32,32,32,32,32,32,32,32,32,69,10,32,32,32,32,32,32,32,32,32,32,32,32,32,35,10,32,32,32,32,32,32,32,35,32,32,32,32,32,32,10,32,32,32,32,32,35,32,32,32,35,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,35,35,35,35,35,35,35,35,35,35,35,35,35,83};
    //ice_maze_solver(map);
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','l','u','r','u','r','d','l','u','l','d','r','d','r','u','r'})));

    map = "x       x #\n"
          "x x   x   #\n"
          "#         #\n"
          "  # x     E\n"
          "  x x     #\n"
          "  x   x   #\n"
          "          #\n"
          "S       x #";
      //cout << "Undertale, ice tic-tac-toe puzzle (edited)\n" << map << endl;
      Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'r','u','l','d','r','r','d','l','u','r','u','r'})));

    map =
    "x     x # \n"
    "     #  # \n"
    " ##x E    \n"
    " #    x   \n"
    " #  #     \n"
    " #  x    x\n"
    "x       # \n"
    "     #  # \n"
    " #x  x   x\n"
    " #     x  \n"
    " #      # \n"
    " # x  x # \n"
    " #x#xx#x# \n"
    " #x#xx### \n"
    "x xxxxxx x\n"
    "# xxSxxx #";

    //std::cout << std::endl << "Pokemon Gold (again), Mahogany Town Gym" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','u','u','u','l','u','r','d','l','u','r'})));

    map = {32,32,32,35,32,35,32,32,32,32,35,32,32,32,32,35,32,32,35,32,32,32,32,35,32,32,32,32,32,10,32,32,32,35,32,32,32,32,32,32,32,35,32,32,32,32,32,32,32,35,32,32,32,32,35,32,120,32,32,10,35,32,32,32,35,120,35,32,32,32,32,35,32,32,32,32,32,32,32,35,32,32,35,35,32,32,32,32,32,10,120,120,32,35,32,35,32,32,32,32,35,32,35,32,32,32,32,32,35,32,32,32,32,32,32,32,32,32,32,10,32,32,120,32,32,35,32,35,32,35,32,35,32,35,32,32,32,32,32,32,32,35,120,32,32,32,32,32,35,10,32,32,32,32,32,32,32,120,32,35,32,32,32,35,32,32,32,32,32,32,32,32,35,32,35,32,32,32,32,10,32,32,32,35,32,32,35,32,32,32,32,35,32,32,35,32,35,32,32,35,32,32,32,32,32,32,35,32,32,10,32,32,32,35,32,32,32,32,35,32,32,32,120,35,32,32,32,120,32,32,32,35,32,32,32,32,35,32,32,10,35,32,32,32,35,32,32,35,32,35,32,35,32,32,35,120,32,32,32,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,35,32,32,35,35,32,32,32,32,32,32,32,32,32,35,120,32,32,32,10,32,120,32,32,35,32,32,32,32,32,35,35,32,120,32,32,32,32,32,32,35,32,35,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,120,32,32,32,32,32,32,120,120,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,35,120,32,35,32,32,35,32,120,32,35,32,120,32,32,35,32,32,32,35,32,32,32,32,10,32,32,32,32,120,32,32,32,32,32,32,35,32,32,32,32,32,32,35,32,35,32,32,32,32,120,35,32,32,10,35,32,32,32,35,32,35,32,32,32,35,32,32,32,120,32,32,32,35,32,32,32,32,32,32,35,32,32,32,10,32,32,32,32,35,32,32,32,32,32,32,32,32,35,32,32,32,32,32,32,32,32,32,32,35,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,120,32,35,32,35,32,32,32,32,32,32,32,32,32,32,35,35,35,10,32,32,32,32,32,32,32,32,32,32,32,32,32,35,32,35,35,32,120,120,35,32,120,32,32,32,32,32,32,10,35,32,35,32,32,32,32,32,32,32,32,32,32,32,32,35,32,32,32,32,32,32,32,32,120,32,32,32,35,10,32,32,35,35,32,32,32,35,35,32,32,32,32,32,32,32,32,32,32,35,35,35,32,120,32,32,120,32,32,10,32,35,35,35,32,32,32,35,32,32,32,32,32,32,32,32,35,120,32,32,35,32,32,32,32,32,35,32,32,10,35,35,32,32,32,120,35,32,32,35,32,32,32,35,32,120,32,32,32,35,32,32,32,35,35,32,32,32,32,10,32,32,35,32,35,32,35,35,32,32,35,35,32,32,32,35,32,120,32,32,32,120,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,35,32,32,32,35,32,32,32,32,32,32,32,32,32,32,32,32,32,35,32,10,35,32,32,32,32,32,32,32,35,32,32,32,69,32,32,32,32,32,35,32,35,32,32,32,32,32,120,35,32,10,32,35,32,32,35,32,32,32,32,35,32,32,32,32,35,35,35,32,35,32,35,32,35,35,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,120,35,32,32,32,32,32,32,32,32,32,32,35,32,32,10,32,35,32,35,32,120,32,32,32,32,32,32,32,32,35,32,32,32,32,32,32,32,32,32,32,32,32,32,120,10,35,32,32,35,32,35,32,32,32,32,35,32,32,32,32,32,32,35,32,32,35,32,83,32,32,32,32,32,32,10,35,32,32,32,32,32,32,32,32,120,35,32,35,32,32,32,32,32,32,32,32,35,32,32,32,32,32,32,32,10,120,32,32,32,32,35,32,32,32,32,32,32,32,32,32,35,35,32,32,35,32,35,32,32,32,32,32,35,32,10,32,32,35,32,32,35,35,35,32,32,32,32,32,32,32,32,32,35,120,32,32,35,35,32,32,32,32,32,35,10,32,32,32,35,120,35,35,32,32,32,32,32,32,32,32,32,32,32,35,35,32,35,32,35,32,32,32,35,32};

    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','l','l','d','l'})));

    /*
    */

}
