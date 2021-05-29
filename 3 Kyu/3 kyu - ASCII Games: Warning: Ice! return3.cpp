#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef vector<char> test_t;
class Assert {
    public :
      static void That (test_t actual, test_t expression) {
        if (actual != expression) {
          cout << "actual : ";
          for (auto it : actual)
              cout << it;

          cout << "\nexpect : ";
          for (auto it : expression)
              cout << it;
          cout << endl;
        }
    }
};
test_t Equals (test_t entry) { return entry;}
void Test();
////////////////////////////////////////////////////////////////////////////////

struct Vertex {
    int x, y;
    char floor;
    int dist, moves;
    bool visit;
    string path;
    Vertex *prev;

    void move (const Vertex &v) {
        x += v.x, y += v.y;
    }
};

struct comp {
    bool operator()(Vertex const *a, Vertex const *b ) {
        if (a->moves == b->moves)
            return a->dist > b->dist;
        else
            return a->moves > b->moves;
    }
};

const vector<Vertex> compass {{-1,0,'l'},{1,0,'r'},{0,-1,'u'},{0,1,'d'}};
const char wall{'#'}, start{'S'}, stop{'E'}, ice{' '}, ground{'x'};

Vertex nul {0,0,wall,-1};

class Graph {
    private :
        vector<vector<Vertex>> Vmap;
    public :
        Vertex nul {0,0,wall,-1};
        size_t width, height;
        Vertex *source, *exit;

        Graph (const string &src = "") {
            width = src.find ('\n'), height = src.size() / width;

            if (width != string::npos)
                Vmap.resize (height, vector<Vertex>(width));

            Vertex p = {.floor = 0,.dist = numeric_limits<int>::max(),.moves = 0, .visit = 0,.path = {}, .prev = &nul};

            for (p.y = 0; p.y < height; p.y++) {
                for (p.x = 0; p.x < width; ++p.x) {
                    p.floor = src[p.y * (width + 1) + p.x];
                    Vmap[p.y][p.x] = p;

                    if (p.floor == start) source = &Vmap[p.y][p.x];
                    if (p.floor == stop) exit = &Vmap[p.y][p.x];
                }
            }

            source->dist = 0;
            source->floor = ice;

            exit->floor = ground;
        }

        bool is_exit (Vertex &p) { return p.x == exit->x && p.y == exit->y; }
        bool is_free (Vertex &p) {
          if (is_inside (p) == false) return false;
          return (Vmap[p.y][p.x].visit == false);
        }
        bool is_inside (Vertex &p) { return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height; }
        Vertex *operator[] (Vertex &p) { return is_inside(p) ? &Vmap[p.y][p.x] : &nul; }
};
class Display {
    public :
        static void graph (Graph &G) {
            Vertex p;
            for (p.y = 0; p.y < G.height; p.y++) {
                for (p.x = 0; p.x < G.width; ++p.x) {
                    cout << G[p]->floor;
                }
                cout << endl;
            }
            cout << endl;
        }
        static void route (Vertex &p) {
            cout << '[';
            for (auto &it : p.path) cout << it;
            cout << "]\n";
        }
        static void stack (priority_queue<Vertex*, vector<Vertex*>, comp> Q) {
            while (!Q.empty()) {
                cout << Q.top()->moves << ' ';
                //route ();
                Q.pop();
            }
        }
        static void path (Graph &G) {
            Vertex p;
            const int m = G.height + 1, n = G.width + 1;

            for (p.y = -1; p.y < m; p.y++) {
                for (p.x = -1; p.x < n; p.x++) {
                    char tile = G[p]->floor;
                    if (tile == wall) cout << tile;
                    else if (G[p]->dist < 100) cout << '.';
                    else cout << G[p]->floor;
                }
                cout << endl;
            }
            cout << endl;
        }
        static void point (Vertex p) {
            cout << '[' << p.x << ',' << p.y << ']' << endl;
        }
      };

int distance (Vertex &a, Vertex &b) { return abs (a.x - b.x) + abs (a.y - b.y); }
bool equals (const Vertex &a, const Vertex &b) { return a.x == b.x && a.y == b.y; }

bool is_valid (Graph &G, Vertex *u, const Vertex &direction) {

    Vertex nxt {u->x + direction.x ,u->y + direction.y};
    Vertex from {u->x - u->prev->x, u->y - u->prev->y};

    if (G[nxt]->floor == wall) return false;
    if (G[nxt]->visit == true) return false;
    //if (equals (*u->prev, nxt) == true) return false;
    if (u->floor == ice && u->prev->floor == ground && equals (direction, from) == false) {
        //u->path.pop_back();
        return false;
    }
    /*
    */
    return true;
}
std::vector<char> ice_maze_solver (const std::string &map) {

    Graph G (map);
    Vertex *u, *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;

    Q.push (G.source);
    int index = 9;

    while (!Q.empty()) {

        u = Q.top();
        u->visit = true;
        Q.pop();

        if (G.is_exit (*u)) {
            return {u->path.begin(), u->path.end()};
        }
        Display::stack(Q);
        for (auto &direction : compass) {
            Vertex nxt {u->x + direction.x ,u->y + direction.y};
            Vertex *curr = u, *prev = u;

            int alt = u->dist + 1;
            string path;

            if (u->floor == ice && u->path.back() == direction.floor) {
                path = u->path;
            } else {
                path = u->path + direction.floor;
            }

            if (is_valid (G, u, direction)) {
                nextv = G[nxt];
                while (curr->floor == ice) {
                    prev = curr, curr = G[nxt];
                    nextv = curr;

                    nxt.move (direction);
                    if (curr->floor == ground || G[nxt]->floor == wall) break;
                    curr->dist = alt++;

                }

                if (alt < nextv->dist) {
                    nextv->dist = alt;
                    nextv->moves = u->dist + 1;
                    nextv->path = path;
                    nextv->prev = prev;
                    Q.push (nextv);
                }
            }
        }
    }

    return {};
}

int main () {

  std::string map;

  map = "E#   #\n"
        "      \n"
        "#     \n"
        "  #   \n"
        " #    \n"
        " S    ";
  std::cout << std::endl << "Tiebreak by least number of movess first" << std::endl << map << std::endl;
   Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'r','u','l','u'})));

  // Test();
}

void Test() {

    std::string map;
    map = "    x \n"
          "  #   \n"
          "   E  \n"
          " #    \n"
          "    # \n"
          "S    #";
    std::cout << "A simple spiral" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','r','d','l','u','r'})));
    map = " #    \n"
          "x   E \n"
          "      \n"
          "     S\n"
          "      \n"
          " #    ";
    std::cout << std::endl << "Slippery puzzle has one-way paths" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','r'})));

    map = "E#    \n"
          "      \n"
          "      \n"
          "      \n"
          "      \n"
          " #   S";
    std::cout << std::endl << "The end is unreachable" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>()));

    map = "E#   #\n"
          "      \n"
          "#     \n"
          "  #   \n"
          " #    \n"
          " S    ";
    std::cout << std::endl << "Tiebreak by least number of movess first" << std::endl << map << std::endl;
     Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'r','u','l','u'})));
    map = "    E \n"
          "     #\n"
          "      \n"
          "# #   \n"
          "    # \n"
          " #  S ";
    std::cout << std::endl << "Then by total distance traversed" << std::endl << map << std::endl;
     Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','r','u','r'})));


     map = "E#xxx\n"
          "x#x#x\n"
          "x#x#x\n"
          "x#x#x\n"
          "xxx#S";
    std::cout << std::endl << "Covering all tiles with x should reduce the problem to simple pathfinding" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({ 'u','u','u','u','l','l','d','d','d','d','l','l','u','u','u','u'})));

    map = {32,32,32,32,32,32,32,32,35,32,32,32,32,32,10,32,32,32,35,32,32,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,35,32,32,32,32,10,32,35,32,32,32,32,32,32,32,32,32,32,32,32,10,35,32,32,32,32,32,32,32,35,32,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,35,10,32,32,32,32,32,32,35,32,32,32,32,32,32,32,10,32,32,35,32,32,32,32,32,32,32,32,32,32,69,10,32,32,32,32,32,32,32,32,32,32,32,32,32,35,10,32,32,32,32,32,32,32,35,32,32,32,32,32,32,10,32,32,32,32,32,35,32,32,32,35,32,32,32,32,10,32,32,32,32,32,32,32,32,32,32,32,32,32,32,10,35,35,35,35,35,35,35,35,35,35,35,35,35,83};
    //ice_maze_solver(map);
    std::cout << std::endl << "Test " << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','l','u','r','u','r','d','l','u','l','d','r','d','r','u','r'})));
    /*
    /*
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

    std::cout << std::endl << "Pokemon Gold (again), Mahogany Town Gym" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','u','u','u','l','u','r','d','l','u','r'})));
    */

}

string mkpath (Vertex *v, char dir) {

    string prev;

    if (v->path.size()) {
        prev = v->path;
        if (prev.back() == dir)
            return prev;
    }

    prev.push_back (dir);
    return prev;
  }
Vertex *slide (Graph &G, Vertex *u, Vertex direction) {

    int alt = u->dist + 1;
    Vertex *nextv, nxt = {u->x + direction.x, u->y + direction.y}, curr;
    string path = u->path.back() != direction.floor ? u->path + direction.floor : u->path;

    while (G[nxt]->floor == ice) {
        curr = nxt;
        nextv->dist = alt++;
        G[nxt]->visit = true;
        nxt.move (direction);
    }

    nextv = G[nxt]->floor == wall ? G[curr] : G[nxt];
    nextv->moves = u->dist + 1;
    nextv->path = u->path + direction.floor;

    return nextv;
}
std::vector<char> ice_maze_solver2 (const std::string &map) {

    Graph G (map);
    Vertex *u, *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;
    Q.push (G.source);

    while (!Q.empty()) {

        u = Q.top();
        u->visit = true;
        Q.pop();

        if (G.is_exit (*u)) {
            Display::route (*u);
            return {u->path.begin(), u->path.end()};
        }
        Display::path (G);
        //Display::showpath (G);
        for (auto direction : compass) {
            Vertex nxt {u->x + direction.x ,u->y + direction.y};
            int alt = u->dist + 1;

            if (G.is_free (nxt) && G[nxt]->floor != wall) {
                if (u->floor == ice) {
                    /*
                    while (G[u]->floor == ice) {
                        u = G[nxt];
                        u->dist = alt;
                        nxt.x = u->x + direction.x, nxt.y = u->y + direction.y;
                    }
                    */
                    //Vertex sld {nxt->x + direction.x, nxt->y + direction.y};
                    if (u->prev && u->prev->floor == ground) {
                        Vertex from {u->x - u->prev->x, u->y - u->prev->y};

                    if (equals (direction, from))
                         nextv = slide (G, u, direction);
                    } else {
                         nextv = slide (G, u, direction);
                    }
                    /*
                    */
                    //nextv = slide (G, u, direction);
                } else {
                    nextv = G[nxt];

                    if (alt < nextv->dist) {
                      nextv->dist = alt;
                      nextv->moves = alt;
                      nextv->path = u->path + direction.floor;
                      nextv->prev = u;
                    }
                }
                Q.push (nextv);
            }
        }
    }
    //Display::graph (G);
    return {};
}
std::vector<char> ice_maze_solver3 (const std::string &map) {

  Graph G (map);
  Vertex *u, *nextv;
  priority_queue<Vertex*, vector<Vertex*>, comp> Q;
  Q.push (G.source);
  int index = 8;
  while (!Q.empty()) {

    u = Q.top();
    u->visit = true;
    Q.pop();

    if (G.is_exit (*u)) {
      //Display::route (*u);
      return {u->path.begin(), u->path.end()};
    }
    Display::path (G);
    //Display::showpath (G);
    for (auto direction : compass) {
      Vertex nxt {u->x + direction.x ,u->y + direction.y};
      Vertex *curr = u, *prev = u;

      int alt = u->dist + 1;
      string path = u->path + direction.floor;
      bool flag = false;

      if (G.is_free (nxt) && G[nxt]->floor != wall && !equals (*curr->prev, nxt)) {
        if (curr->floor == ground) {
          nextv = G[nxt];

          if (alt < nextv->dist) {
            nextv->dist = alt;
            nextv->moves = u->dist + 1;
            nextv->path = u->path + direction.floor;
            nextv->prev = prev;
          }
        }
        if (u->floor == ice) {
          nextv = G[nxt];
          //if (is_icev (u, direction)) {
            while (curr->floor == ice) {
              prev = curr;
              curr = G[nxt];
              curr->visit = true;
              curr->dist = alt++;

              nextv = curr;
              nxt.move (direction);
              if (G[nxt]->floor == wall) break;
            }
            nextv->moves = u->dist + 1;
            nextv->prev = prev;
            //} else
            //    nextv = &nul;
          }
          Q.push (nextv);
        }
      }
    }
    //Display::graph (G);
    return {};
  }
