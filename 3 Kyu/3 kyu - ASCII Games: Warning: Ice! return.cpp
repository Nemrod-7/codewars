#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <map>

using namespace std;

struct Vertex {
    int x, y, dist;
    char tile;
    bool visit;
    vector<char> route;
    Vertex *next;
};
struct point {
  int x, y;
  char id;
};
struct comp {
    bool operator()(Vertex const *a, Vertex const *b ) {
        return a->dist < b->dist;
    }
};

const vector<point> compass {{-1,0,'u'},{1,0,'d'},{0,-1,'l'},{0,1,'r'}};
const char wall{'#'}, start{'S'}, stop{'E'}, ice{' '}, ground{'x'};

class Graph {
    public:
        size_t width, height;
        vector<vector<Vertex>> Vmap;
        Vertex *enter, *exit;

        Graph(string src) {
            width = src.find ('\n'), height = src.size() / width;
            Vmap.resize (width, vector<Vertex> (height));

            int next = 0;
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; y++) {
                    Vmap[x][y] = {
                        .x = x, .y = y,
                        .dist = numeric_limits<int>::max(),
                        .visit = false,
                        .next = NULL };
                    if (src[next] == '\n') next++;

                    char floor = src[next];
                    if (src[next] == start) {
                        floor = ice;
                        Vmap[x][y].dist = 0;
                        enter = coord({x,y});
                    }
                    if (src[next] == stop) {
                        floor =  ground;
                        exit = coord({x,y});
                    }
                    Vmap[x][y].tile = floor;
                    next++;
                }
            }
        }
        Vertex *coord (point p) { return &Vmap[p.x][p.y];}
        bool isfree(point p) { return coord(p)->visit == false;}
};

vector<char> mkroute (Vertex *v, char dir) {

    vector<char> next;

    if (v->route.size()) {
        next = v->route;
        if (next.back() == dir)
            return next;
    }

    next.push_back (dir);
    return next;
}
char get_direction (Vertex *prev, Vertex *curr) {
    map <pair<int,int>,char> padkey {{{-1,0},'u'}, {{1,0},'d'},{{0,-1},'l'},{{0,1},'r'}};
    pair <int,int> p {(curr->x - prev->x), (curr->y - prev->y)};
    return padkey[p];
}
bool is_valid (Graph *G, point p) {
    if (p.x < 0 || p.y < 0 || p.x >= G->width || p.y >= G->height) return false;
    if (G->coord(p)->tile == wall) return false;
    return true;
}
bool has_reached (Vertex *curr, Vertex *exit) {
    return (curr->x == exit->x && curr->y == exit->y);
}
Vertex *slide (Graph *G, Vertex *v, point direction) {
    int alt = v->dist;
    Vertex *nextv = v;
    point move = direction;

    while (true) {
        v = nextv;
        point p {v->x + move.x, v->y + move.y};
        if (is_valid (G, p) == false) return v;
        nextv = G->coord(p);
        nextv->dist = alt++;
        nextv->visit = true;
        nextv->route = mkroute (v, direction.id);
        nextv->next = v;

        if (nextv->tile == ground) return nextv;
    }

}

std::vector<char> ice_maze_solver(const std::string &map) {

    Graph G(map);
    Vertex *u, *nextv;
    priority_queue<Vertex*, vector<Vertex*>, comp> Q;

    Q.push (G.enter);
    while (!Q.empty()) {

        u = Q.top();
        u->visit = true;
        Q.pop();

        if (has_reached (G.exit, u)) return u->route;

        for (auto direction : compass) {
            point p{u->x + direction.x ,u->y + direction.y};
            int alt = u->dist + 1;

            if (is_valid(&G, p) && G.isfree(p)) {

                if (u->tile == ice) {

                    if (u->next && u->next->tile == ground) {
                        if (get_direction (u->next, u) == direction.id)
                            nextv = slide(&G, u, direction);

                    } else
                        nextv = slide(&G, u, direction);


                } else {
                    nextv = G.coord (p);
                    if (alt < nextv->dist) {
                        nextv->dist = alt;
                        nextv->next = u;
                        nextv->route = mkroute (u, direction.id);
                    }
                }
                Q.push(nextv);
            }
        }
    }

    return {};
}

/////////////////////////////////Assert/////////////////////////////////////////
#include <chrono>
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

    Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

}
void Test() {

    std::string map = "    x \n"
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
    /*

    */



}
