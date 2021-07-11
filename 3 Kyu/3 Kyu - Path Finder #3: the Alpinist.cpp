#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <limits>

using namespace std;
using point = pair<int,int>;

struct vtx {
    int dist, visit, val, coord;

    bool operator<(const vtx &x) const{
        return dist > x.dist;
    }
};

bool is_free (string &maze, size_t pos) { return pos >= 0 && pos < maze.size() && isdigit (maze[pos]); }

int path_finder (std::string maze) {

  if (maze.size() < 2) return 0;

  const int size = maze.find('\n') + 1, directions[4] = {-1, 1, -size, size};

  vector<vtx> grph;
  priority_queue<vtx> q1;

  auto val = [](char c) {return c - '0';};

  for (int i = 0; i < maze.size(); i++)
      grph.push_back ({numeric_limits<int>::max(), false, maze[i] - '0', i});

  grph[0].dist = 0;
  q1.push(grph[0]);

  while (!q1.empty()) {
      size_t coord = q1.top().coord;
      vtx &u = grph[coord];

      q1.pop();
      u.visit = true;

      if (coord == maze.size() - 1) return u.dist;

      for (int pos : directions) {
          size_t next = pos + coord;
          vtx &nxt = grph[next];

          if (is_free (maze, next) && nxt.visit == false) {
              int alt = u.dist + abs (u.val - nxt.val);

              if (alt < nxt.dist) {
                  nxt.dist = alt;
                  q1.push (nxt);
              }
          }
      }
  }

  return -1;
}



int main () {

  std::string s7 =
  "000000\n"
  "000000\n"
  "000000\n"
  "000010\n"
  "000109\n"
  "001010";

  //cout << rnd;
  cout << path_finder (s7);
}

void Test () {
      std::string s1 =

    "000\n"
    "000\n"
    "000";

    std::string s2 =

    "010\n"
    "010\n"
    "010";

    std::string s3 =

    "010\n"
    "101\n"
    "010";

    std::string s4 =

    "0707\n"
    "7070\n"
    "0707\n"
    "7070";

    std::string s5 =

    "700000\n"
    "077770\n"
    "077770\n"
    "077770\n"
    "077770\n"
    "000007";

    std::string s6 =

    "777000\n"
    "007000\n"
    "007000\n"
    "007000\n"
    "007000\n"
    "007777";

    std::string s7 =

    "000000\n"
    "000000\n"
    "000000\n"
    "000010\n"
    "000109\n"
    "001010";

    /*
    Assert::That(path_finder(s1), Equals( 0));
    Assert::That(path_finder(s2), Equals( 2));
    Assert::That(path_finder(s3), Equals( 4));
    Assert::That(path_finder(s4), Equals(42));
    Assert::That(path_finder(s5), Equals(14));
    Assert::That(path_finder(s6), Equals( 0));
    Assert::That(path_finder(s7), Equals( 4));
    */
}
/////////////////////////////////arkive/////////////////////////////

int path_finder2(std::string maze) {

  if (maze.size() < 2) return 0;

  int size = maze.find('\n') + 1, alt;
  const int directions[4] = {-1, 1, -size, size};

  vector<pair<int, bool>> vertex (maze.size(), {numeric_limits<int>::max(), false});
  priority_queue<point, vector<point>, greater<point>> q1;

  auto val = [](char c) {return c - '0';};

  q1.push({0,0});
  vertex[0].first = 0;

  while (!q1.empty()) {
      size_t coord = q1.top().second;
      auto &[dist, visit] = vertex[coord];

      q1.pop();
      visit = true;

      if (coord == maze.size() - 1) return dist;

      for (int pos : directions) {
          int next = pos + coord;
          auto &[nxdist, vted] = vertex[next];

          if (is_free (maze, next) && vted == false) {
              alt = dist + abs (val (maze[coord]) - val(maze[next]));

              if (alt < nxdist) {
                  nxdist = alt;
                  q1.push ({alt,next});
              }
          }
      }
  }

  return -1;
}




struct Vertex {
    int x, y;
    int dist, edge;
    int state;
    Vertex *next;
    //vector<struct Edge> ve;
};
struct Edge {
    Vertex node[2];
    int cost;
};
struct comp {
    bool operator()(Vertex const *a, Vertex const *b ) {
        return a->dist > b->dist;
    }
};

enum status {FREE,VISIT,WALL,PATH};
const vector<pair<int,int>> directions{{-1,0},{1,0},{0,-1},{0,1}};

class Graph {
    public:
        int N;
        vector<vector<Vertex>> Vmap;
        Graph(string src) {
            N = src.find ('\n') + 1;
            Vmap.resize (N, vector<Vertex> (N));
            int next = 0;
            for (int x = 0; x < N - 1; ++x) {
                for (int y = 0; y < N - 1; y++) {
                    if (src[next] == '\n') next++;
                    Vmap[x][y] = {
                        .x = x, .y = y,
                        .dist = numeric_limits<int>::max(),
                        .edge = src[next++] - '0',
                        .state = 0,
                        .next = NULL };
                }
            }
            N--;
        }
        Vertex *coord (pair<int,int> p) { return &Vmap[p.first][p.second];}
        void mkpath(Vertex *v) {
            while (v) {
                v->state = PATH;
                v = v->next;
            }
        }
        void display () {
            for (int x = 0; x < N; ++x) {
                for (int y = 0; y < N; ++y) {
                    cout << Vmap[x][y].edge;
                    /*
                    int now = Vmap[x][y].state;
                    switch (now) {
                        case WALL: cout << "#"; break;
                        case FREE: cout << " "; break;
                        case VISIT:cout << " "; break;
                        case PATH: cout << "*"; break;
                    }
                    */
                }
                cout << "\n";
            }
            cout << "\n";
        }

};

bool is_inside (Graph *G, pair<int,int> p) {
    return (p.first >= 0 && p.second >= 0 && p.first < G->N && p.second < G->N);
}
bool reach (Vertex *curr, Vertex *exit) {
    return (curr->x == exit->x && curr->y == exit->y);
}
bool is_free (Graph *G, pair<int,int> V) {
    return G->coord(V)->state == FREE;
}
int dijkstra (string maze) {

    if (maze.size() < 2) return 0;
    //cout << maze;
    Graph G(maze);

    priority_queue<Vertex*, vector<Vertex*>, comp> Q;
    Vertex *source = G.coord({0,0}), *exit = G.coord({G.N-1,G.N-1});

    Vertex* u, *nextv;
    int alt;

    source->dist = 0;
    source->next = NULL;
    //G.display();

    Q.push(source);

    while (!Q.empty()) {

        u = Q.top();
        Q.pop();

        if(reach(u, exit))
            return exit->dist;

        u->state = VISIT;

        for (auto now : directions) {

            pair<int,int> p{u->x + now.first , u->y + now.second};

            if (is_inside (&G, p) && is_free (&G, p)) {
                nextv = G.coord (p);

                alt = u->dist + abs(u->edge - nextv->edge);

                if (alt < nextv->dist) {
                    nextv->dist = alt;
                    nextv->next = u;
                    Q.push (nextv);
                }
            }
        }
    }

    //G.mkpath(u);
    //G.display();
    return -1;

}
