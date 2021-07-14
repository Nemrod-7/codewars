#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "Assert.hpp"


using namespace std;
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
      return a->dist > b->dist;
      /*
        if (a->moves == b->moves)
        else
            return a->moves > b->moves;
            */
    }
};

const vector<Vertex> compass {{-1,0,'l'},{1,0,'r'},{0,-1,'u'},{0,1,'d'}};
const char wall{'#'}, start{'S'}, stop{'E'}, ice{' '}, ground{'x'};

class Graph {
    private :
        vector<vector<Vertex>> Vmap;
    public :
        Vertex nul {0,0,wall,-1};
        size_t width, height;
        Vertex *source, *exit;

        Graph (const string &src = "") {
            width = src.find ('\n');
            //height = src.size() / width;
            height = count (begin(src), end(src), '\n') + 1;
            if (width != string::npos)
                Vmap.resize (height, vector<Vertex>(width));

            Vertex p = {.floor = 0,.dist = numeric_limits<int>::max(),.moves = 0, .visit = 0,.path = {}, .prev = &nul};

            for (p.y = 0; p.y < height; p.y++) {
                for (p.x = 0; p.x < width; ++p.x) {
                    p.floor = src[p.y * (width + 1) + p.x];
                    Vmap[p.y][p.x] = p;

                    if (p.floor == start) {
                        source = &Vmap[p.y][p.x];
                    }
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
        static void visited (Graph &G) {
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
        static void locate (Graph &G, Vertex *v) {
            Vertex p;
            const int m = G.height + 1, n = G.width + 1;

            for (p.y = -1; p.y < m; p.y++) {
                for (p.x = -1; p.x < n; p.x++) {

                    if (v->x ==p.x && v->y == p.y) cout << '+';
                    else cout << G[p]->floor;
                }
                cout << endl;
            }
            cout << endl;
        }
        static void path (Graph &G) {
            Vertex p;
            const int m = G.height + 1, n = G.width + 1;

            for (p.y = -1; p.y < m; p.y++) {
                for (p.x = -1; p.x < n; p.x++) {
                    char tile = G[p]->floor;
                    if (tile == wall) cout << tile;
                    else if (G[p]->dist < 100) cout << '+';
                    else cout << G[p]->floor;
                }
                cout << endl;
            }
            cout << endl;
        }
        static void route (Graph &G, Vertex *v) {
            Vertex p;
            const int m = G.height , n = G.width;
            vector<vector<int>> route (G.height, vector<int>(G.width));
            Vertex *root = v;

            while (root) {

                route[root->y][root->x] = true;
                root = root->prev;
            }

            for (p.y = 0; p.y < m; p.y++) {
                for (p.x = 0; p.x < n; p.x++) {

                    if (route[p.y][p.x] == true) cout << '.';
                    else
                    cout << G[p]->floor;
                }
                cout << endl;
            }
            cout << endl;
        }
        static void point (Vertex p) {
            cout << '[' << p.x << ',' << p.y << ']' << endl;
        }
};

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
float distance (Vertex *a, Vertex *b) {
    return max (abs(a->x - b->x), abs (a->y - b->y)); // diagonal distance
  //return hypot (a->x - b->x, a->y - b->y); // euclidian distance
  //  return abs (a->x - b->x) + abs (a->y - b->y); //  manathan distance
}

std::vector<char> debug (Graph &G, Vertex  *u) {

    Vertex *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;

    Q.push (u);

    while (!Q.empty()) {
        u = Q.top();
        u->visit = true;
        Q.pop();
        //Display::visited (G);
        Display::locate (G, u);
        if (G.is_exit (*u)) {
            return {u->path.begin(), u->path.end()};
        }

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
                    curr = G[nxt];
                    prev = curr;

                    nxt.move (direction);
                    if (curr->floor == ground || G[nxt]->floor == wall) break;
                    curr->dist = alt++;
                    nextv = G[nxt];
                }

                //cout << endl;
                if (alt < nextv->dist) {
                    nextv->dist = alt;
                    nextv->moves = u->moves + 1;
                    nextv->path = path;
                    nextv->prev = prev;
                    Q.push (nextv);
                }
            }
        }
    }

    //Display::path(G);

    return {};
}

std::vector<char> ice_maze_solver (const std::string &map) {

    Graph G (map);
    Vertex *u, *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;

    Q.push (G.source);

    //while (index-->0) {
    while (!Q.empty()) {

        u = Q.top();
        u->visit = true;
        Q.pop();

        if (G.is_exit (*u)) {
            //Display::visited(G);
            Display::path(G);
            return {u->path.begin(), u->path.end()};
        }
        //Display::point(*u);
        if (u->path == "uuuulurd") {
            break;
        }
        for (auto &direction : compass) {
            Vertex nxt {u->x + direction.x ,u->y + direction.y};
            Vertex *curr = u, *prev = u;

            string path;

            if (u->floor == ice && u->path.back() == direction.floor) {
                path = u->path;
            } else {
                path = u->path + direction.floor;
            }

            if (is_valid (G, u, direction)) {
                nextv = G[nxt];

                int alt = distance (nextv, G.exit);
                //int alt = u->dist + 1;
                while (curr->floor == ice) {
                    curr = G[nxt];
                    prev = curr;

                    nxt.move (direction);
                    if (curr->floor == ground || G[nxt]->floor == wall) break;
                    curr->dist = alt++;
                    nextv = G[nxt];
                }

                //cout << endl;
                if (alt < nextv->dist) {
                    nextv->dist = alt;
                    nextv->moves = u->moves + 1;
                    nextv->path = path;
                    nextv->prev = prev;
                    Q.push (nextv);
                }
            }
        }
    }
    //debug (G, u);

    return {};
}

int main () {

  std::string map;

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

  map = "      #  xE\n"
        "      #    \n"
        "           \n"
        "      #    \n"
        "      #    \n"
        "      # #  \n"
        "           \n"
        " #         \n"
        "         # \n"
        " S        #";

        ice_maze_solver (map);
  //std::cout << std::endl << "Pokemon Gold (again), Mahogany Town Gym" << std::endl << map << std::endl;
  //Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','u','u','u','l','u','r','d','l','u','r'})));

}
