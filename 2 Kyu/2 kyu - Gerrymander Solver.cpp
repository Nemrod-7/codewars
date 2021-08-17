#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Vertex {
    public :
        int x, y;
        int dist;
        char data;

        int distance (Vertex &a) { return abs (x - a.x) + abs (y - a.y); }
};

const vector<Vertex> compass {{1,0},{0,1},{-1,0},{0,-1}};
const vector<Vertex> corner {{1,1},{-1,1},{-1,-1},{1,-1}};

class Graph {
    private :
        Vertex nullv {0,0,-1,' '};
        string data;
    public :
        vector<vector<Vertex>> Vmap;
        vector<Vertex> Vmap2;
        vector<Vertex*> obj;
        vector<int> clust;

        Graph (const string &src) {
            data = src;
            Vmap.resize (5, vector<Vertex> (5));
            clust.resize(5);

            for (int y = 0; y < 5; ++y) {
                for (int x = 0; x < 5; ++x) {
                    Vertex V = {x, y, y, src[y * 6 + x]};

                    if (src[y * 6 + x] == 'O') {
                        obj.push_back(&Vmap[y][x]);
                        clust[y]++;
                    }

                    Vmap[y][x] = V;
                }
            }
        }

        bool is_inside (Vertex p) { return (p.x >= 0 && p.y >= 0 && p.x < 5 && p.y < 5); }
        Vertex *operator[] (Vertex p){ return is_inside(p) ? &Vmap[p.y][p.x] : &nullv;}
};

Vertex *nearest (vector<Vertex*> &points, Vertex *p) {
  Vertex *out;
  int min = 999, dist;

  for (auto &V : points) {
      dist = V->distance (*p);

      if (V != p && dist < min) {
          min = dist;
          out = V;
      }
  }

  return out;
}

bool inters (Graph &G, Vertex *V) {
    int id = V->dist;

    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4, k = (i + 2) % 4, l = (i + 3) % 4;

        Vertex N = {V->x + compass[i].x, V->y + compass[i].y};
        Vertex E = {V->x + compass[j].x, V->y + compass[j].y};
        Vertex S = {V->x + compass[k].x, V->y + compass[k].y};
        Vertex W = {V->x + compass[l].x, V->y + compass[l].y};
        Vertex NE = {V->x + corner[i].x, V->y + corner[i].y};

        if (G[N]->dist == id && G[S]->dist == id && G[E]->dist != id && G[W]->dist != id) return false;
        if (G[N]->dist == id && G[E]->dist == id && G[NE]->dist != id) return false;
    }
    return true;
}

vector<Vertex *> get_clust (Graph &G, int id) {
    vector<Vertex *> cluster;
    Vertex p;

    for (p.y = 0; p.y < 5; ++p.y) {
        for (p.x = 0; p.x < 5; ++p.x) {
            Vertex *V = G[p];

            if (V->dist == id)
                cluster.push_back(V);
        }
    }
    return cluster;
}

void scan_cluster (Graph &G, Vertex *p) {
    int id = p->dist;
    Vertex *nxtv, *heur;
    vector<Vertex *> actual = get_clust (G, id), voter;

    for (auto &dir : compass) {
        Vertex nxt = {p->x + dir.x, p->y + dir.y};
        nxtv = G[nxt];

        if (nxtv && inters (G, nxtv) && (nxtv->dist != p->dist)) {
            voter.push_back (nxtv);
        }
    }

    if (voter.size() == 0) return;

    bool seq2 = false;
    int toswap = voter.front()->dist;
    voter.front()->dist = id;

    for (auto &V : actual)
        if (inters (G, V)) {
            for (auto &dir : compass) {
                Vertex nxt = {V->x + dir.x, V->y + dir.y};
                if (G.is_inside (nxt)) {
                    nxtv = G[nxt];
                    if (inters (G, nxtv) && (nxtv->dist == toswap) && (nxtv != voter.front())) {

                          heur = V;
                          seq2 = true;
                    }
                }
            }
        }

    if (seq2)
        heur->dist = toswap;
    else
        voter.front()->dist = toswap;

}

int update_cnt (Graph &G) {

    Vertex p;
    int cnt = 0;
    fill (G.clust.begin(), G.clust.end(), 0);

    for (p.y = 0; p.y < 5; ++p.y)
        for (p.x = 0; p.x < 5; ++p.x)
            if (G[p]->data == 'O')
                G.clust[G[p]->dist]++;

    for (auto &it : G.clust)
        if (it > 2) cnt++;

    return cnt;
}
bool check (Graph &G) {

    Vertex p;
    int minv = 99;

    for (p.y = 0; p.y < 5; p.y++) {
        for (p.x = 0; p.x < 5; p.x++) {
          Vertex *V = G[p], *next = nearest (G.obj, V);

          if (V->data == 'O')
              minv = min (minv, V->distance (*next));
        }
    }
    return minv < 2 ? true : false;
}
string gerrymander (const string &src) {

    Graph G (src);
    if (check (G) == false) return "";

    random_device rd;
    mt19937 gen (rd());
    uniform_int_distribution<> rng (0, 4);

    string os;

    while (update_cnt(G) != 3)
        scan_cluster (G, G[{rng(gen),rng(gen)}]);

    Vertex p;
    for (p.y = 0; p.y < 5; ++p.y) {
        for (p.x = 0; p.x < 5; ++p.x) {
            os += G[p]->dist + '1';

        }

        if (p.y < 4)
            os += '\n';
    }

    return os;
}

int main () {

  const std::vector<std::string> tests = {
    "OOXXX\n"
    "OOXXX\n"
    "OOXXX\n"
    "OOXXX\n"
    "OOXXX",

    "XOXOX\n"
    "OXXOX\n"
    "XXOXX\n"
    "XOXOX\n"
    "OOXOX",

    "OXOOX\n"
    "XXOXO\n"
    "XOXXX\n"
    "XXOXX\n"
    "OXXOO",

    "XXOXO\n"
    "XOXOX\n"
    "OXOXO\n"
    "XOXOX\n"
    "XXOXX",// no solution

    "XXXXX\n"
    "OOOXO\n"
    "XXXOX\n"
    "OOOOO\n"
    "XXXXX"};

      string sol = gerrymander(tests[0]);

      cout << sol;
}
