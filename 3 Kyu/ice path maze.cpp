#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include <iomanip>
#include <chrono>

using namespace std;

struct Vertex {
    int x, y;
    int dist, move;
    char tile, visit;
    vector<char> route;
    Vertex *next;
};
struct point {
  char id;
  short x, y;
};
struct comp {
    bool operator()(Vertex const *a, Vertex const *b ) {

        if (a->move == b->move)
            return a->dist > b->dist;
        else
            return a->move > b->move;
    }
};

const vector<point> compass {{'l',-1,0},{'r',1,0},{'u',0,-1},{'d',0,1}};
map <pair<int,int>,char> padkey {{{-1,0},'l'}, {{1,0},'r'},{{0,-1},'u'},{{0,1},'d'}};

const char wall{'#'}, start{'S'}, stop{'E'}, ice{' '}, ground{'x'}, path{'*'};

class Graph {
    public:
        size_t width, height;
        vector<vector<Vertex>> Vmap;
        Vertex *source, *exit;

        Graph(string src) {

            width = src.find ('\n'), height = src.size() / width;
            width += 2, height += 2;
            Vmap.resize (height, vector<Vertex> (width));
            int next = 0;
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; x++) {
                    Vmap[y][x] = {
                        .x = x, .y = y,
                        .dist = numeric_limits<int>::max(),
                        .move = 0,
                        .visit = false,
                        .next = NULL };

                    if (x == 0 || x == width - 1  ||
                        y == 0 || y == height - 1) {
                            Vmap[y][x].tile = wall;
                        } else {
                            if (src[next] == '\n') next++;
                            char floor = src[next];
                            if (src[next] == start) {
                                floor = ice;
                                Vmap[y][x].dist = 0;
                                source = coord({x,y});
                            }
                            if (src[next] == stop) {
                                floor = ground;
                                exit = coord({x,y});
                            }
                            Vmap[y][x].tile = floor;
                            next++;
                        }
                }
            }
        }
        bool isfree(pair<int,int> p) { return coord(p)->visit == false;}
        Vertex *coord (pair<int,int> p) { return &Vmap[p.second][p.first];}
        /*
        */

};
class Debug {
    public :
        static void display (Graph G) {
            for (int y = 0; y < G.height; ++y) {
                for (int x = 0; x < G.width; ++x) {
                    if (G.Vmap[y][x].visit)
                        cout << '.' << ' ';
                        //cout << setw(2) << Vmap[y][x].dist << ' ';
                    else {
                        cout << G.Vmap[y][x].tile << ' ';
                    }
                }
                cout << "\n";
            }
            cout << "\n";
        }
        static void showroute (Vertex *v) {
            for (auto it : v->route)
                cout << it;
            cout << endl;
        }
        static void show_vertex (Vertex *v) {
            cout << " <" << v->x << "," << v->y << "> ";
            cout << v->move << " || " << v->dist << " ";
            showroute (v);
        }
        static void show_moves (Graph G) {
            for (int y = 0; y < G.height; ++y) {
                for (int x = 0; x < G.width; ++x) {
                    if (G.Vmap[y][x].visit)
                        //cout << '.' << ' ';
                        cout << G.Vmap[y][x].move << ' ';
                    else {
                        cout << G.Vmap[y][x].tile << ' ';
                    }
                }
                cout << "\n";
            }
            cout << "\n";
        }
};

vector<char> mkroute (Vertex *v, char dir) {

    vector<char> next;

    if (v->route.size()) {
        next = v->route;
        if (v->tile == ice && next.back() == dir)
                return next;
    }

    next.push_back (dir);
    return next;
}
char get_direction (Vertex *prev, Vertex *curr) {
    pair <int,int> p {(curr->x - prev->x), (curr->y - prev->y)};
    return padkey[p];
}
bool is_valid (Graph *G, pair<int,int> p) {
    if (p.first < 0 || p.second < 0 || p.first >= G->width || p.second >= G->height) return false;
    if (G->coord(p)->tile == wall) return false;
    return true;
}
bool is_free (Graph *G, Vertex *now, Vertex *nxt) {
    map<char,int> cardinals {{'d',-2},{'l',-1},{'r',1},{'u',2}};
    int first = cardinals[get_direction (now, nxt)];
    int sec = cardinals[nxt->visit];
    if (nxt->visit == false) return true;

    if (nxt->visit != 'i')
        if (first != sec && first + sec != 0)
            return true;

        return false;
}
bool has_reached (Vertex *curr, Vertex *exit) {
    return (curr->x == exit->x && curr->y == exit->y);
}
Vertex *slide (Graph *G, Vertex *v, point direction) {
    int alt = v->dist;
    Vertex *prev , *next = v;
    //vector<char> route = mkroute (v, direction.id);
    //prev = next;
    while (true) {
        prev = next;
        pair<int,int> p {prev->x + direction.x, prev->y + direction.y};
        //Debug::show_vertex (prev);
        if (is_valid (G, p) == false) return prev;

        next = G->coord(p);
        next->dist = alt++;
        next->move = v->move + 1;
        next->visit = direction.id;
        next->route = mkroute (v, direction.id);
        next->next = prev;
        if (next->tile == ground) return next;

    }

}

std::vector<char> ice_maze_solver(const std::string &map) {

    Graph G(map);
    Vertex *u, *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;
    //queue<Vertex*> Q;
    Q.push (G.source);
    while (!Q.empty()) {

        u = Q.top();
        u->visit = 'i';
        Q.pop();
        Debug::show_vertex (u);

        if (has_reached (G.exit, u)) {

            //Debug::display(G);
            return u->route;
        }
            // return u->route;

        for (auto direction : compass) {
            pair<int,int> nexdir {u->x + direction.x , u->y + direction.y};
            int alt = u->dist + 1;

            if (is_valid(&G, nexdir)) {

                nextv = G.coord (nexdir);
                bool free = is_free(&G, u, nextv);

                if (free) {
                    if (u->tile == ice) {

                        if (u->next && u->next->tile == ground) {
                            if (get_direction (u->next, u) == direction.id) {
                                nextv = slide(&G, u, direction);
                                Q.push(nextv);
                            }

                        } else {
                            nextv = slide(&G, u, direction);
                            Q.push(nextv);
                        }

                    } else {
                        if (alt < nextv->dist) {
                            nextv->dist = alt++;
                            nextv->move = alt;
                            nextv->next = u;
                            nextv->route = mkroute (u, direction.id);
                            Q.push(nextv);
                        }
                    }

              }
           }
        }
    }

    /*

    */

    return {};
}

float moveistic (Vertex *curr, Vertex *dest) {
  return abs(curr->x - dest->x) + abs(curr->y - dest->y); //  manathan distance
  //sqrt ((curr->x - dest->x) * (curr->x - dest->x) + (curr->y - dest->y) * (curr->y - dest->y)); // euclidian distance
}
/////////////////////////////////Assert/////////////////////////////////////////
typedef vector<char> test_t;
class Assert {
  public :
  static void That (test_t actual, test_t expression) {
    if (actual != expression) {
      cout << "actual : ";
      for (auto it : actual)
      cout << it;

      cout << " expected : ";
      for (auto it : expression)
      cout << it;
      cout << endl;
    }
  }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
int main () {

    auto start = std::chrono::high_resolution_clock::now();

    //Test();
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

    std::cout << std::endl << "Pokemon Gold (again), Mahogany Town Gym" << std::endl << map << std::endl;
    Assert::That(ice_maze_solver(map), Equals(std::vector<char>({'u','u','u','u','l','u','r','d','l','u','r'})));
    //debug_maze (map);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

}
void Test() {
    std::string map;

    /*
    */
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

}
