#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <map>

using namespace std;

struct Vertex {
    int x, y, dist;
    char floor;
    bool visit;
    vector<char> route;
    Vertex *next;
};

struct comp {
    bool operator()(Vertex const *a, Vertex const *b ) {
        return a->dist < b->dist;
    }
};

const vector<Vertex> compass {{-1,0},{1,0},{0,-1},{0,1}};
const char wall{'#'}, start{'S'}, stop{'E'}, ice{' '}, ground{'x'};

class Graph {
    private :
        Vertex nul {0,0,-1, wall};
        vector<vector<Vertex>> Vmap;
    public :
        size_t width, height;
        Vertex *source, *exit;

        Graph (const string &src = "") {
            width = src.find ('\n'), height = src.size() / width;

            if (width != string::npos)
                Vmap.resize (height, vector<Vertex>(width));

            Vertex p = {0, 0, numeric_limits<int>::max(), 0,0,{}, nullptr};

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
        Vertex *operator[] (Vertex &p) { return is_inside(p) ? &Vmap[p.y][p.x] : nullptr; }
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
        static void showroute (Graph &G) {
            Vertex p;
            for (p.y = 0; p.y < G.height; p.y++) {
                for (p.x = 0; p.x < G.width; ++p.x) {
                    if (G[p]->visit) cout << '.';
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
bool equals (Vertex &a, Vertex &b) { return a.x == b.x && a.y == b.y; }
Vertex *slide (Graph &G, Vertex *v, Vertex move) {
    int alt = v->dist + 1;
    Vertex *nextv = v;

    while (true) {
        v = nextv;
        Vertex p {v->x + move.x, v->y + move.y};

        if (G.is_inside (p) == false || G[p]->floor == wall) return v;

        nextv = G[p];
        nextv->dist = alt;
        nextv->visit = true;
        //nextv->route = mkroute (v, direction.id);
        nextv->next = v;

        if (nextv->floor == ground) return nextv;
    }

}

std::vector<char> ice_maze_solver(const std::string &map) {

    Graph G (map);
    Vertex *u, *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;

    Q.push (G.source);
    Display::graph (G);

    while (!Q.empty()) {

        u = Q.top();
        u->visit = true;
        Q.pop();

        if (G.is_exit (*u)) {
            Display::showroute (G);
            return u->route;
        }

        for (auto direction : compass) {
            Vertex nxt {u->x + direction.x ,u->y + direction.y};
            int alt = u->dist + 1;

            if (G.is_free (nxt) && G[nxt]->floor != wall) {

                if (u->floor == ice) {
                    if (u->next && u->next->floor == ground) {
                        Vertex pdir {u->x - u->next->x, u->y - u->next->y};
                        Vertex ndir {nxt.x - u->x, nxt.y - u->y};
                        //cout << p.x - u->x << ' ' << p.y - u->y << endl;
                        //Display::point (nxt);
                      if (equals (ndir, pdir))
                         nextv = slide (G, u, direction);
                    } else {
                         //nextv = slide (G, u, direction);
                         nextv = slide (G, u, direction);
                    }

                } else {
                    nextv = G[nxt];

                    if (alt < nextv->dist) {
                      nextv->dist = alt;
                      nextv->next = u;
                      //nextv->route = mkroute (u, direction.id);
                    }
                }
                Q.push (nextv);
            }
        }
    }


    //Display::graph (G);
    return {};
}

////////////////////////////////////////////////////////////////////////////////
int main () {
  /*
  string map =
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
  */
  string map =
    "    x \n"
    "  #   \n"
    "   E  \n"
    " #    \n"
    "    # \n"
    "S    #";

    ice_maze_solver (map);
    //Graph graph (map);
    //Display::graph (graph);



}

void Test() {

    std::string map = "    x \n"
                      "  #   \n"
                      "   E  \n"
                      " #    \n"
                      "    # \n"
                      "S    #";
    //std::cout << "A simple spiral" << std::endl << map << std::endl;
    // Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','r','d','l','u','r'})));
    map = " #    \n"
          "x   E \n"
          "      \n"
          "     S\n"
          "      \n"
          " #    ";
    //std::cout << std::endl << "Slippery puzzle has one-way routes" << std::endl << map << std::endl;
    // Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','r'})));
    map = "E#    \n"
          "      \n"
          "      \n"
          "      \n"
          "      \n"
          " #   S";
    //std::cout << std::endl << "The end is unreachable" << std::endl << map << std::endl;
    // Assert::That(ice_maze_solver(map), Equals(std::vector<char>()));
    map = "E#   #\n"
          "      \n"
          "#     \n"
          "  #   \n"
          " #    \n"
          " S    ";
    //std::cout << std::endl << "Tiebreak by least number of moves first" << std::endl << map << std::endl;
    // Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'r','u','l','u'})));
    map = "    E \n"
          "     #\n"
          "      \n"
          "# #   \n"
          "    # \n"
          " #  S ";
    //std::cout << std::endl << "Then by total distance traversed" << std::endl << map << std::endl;
    // Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'l','u','r','u','r'})));
    /*

    */



}
