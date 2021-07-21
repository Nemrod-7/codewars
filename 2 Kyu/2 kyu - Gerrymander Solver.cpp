#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Point {int x, y;};

const vector<Point> compass {{1,0},{0,1},{-1,0},{0,-1}};
const vector<Point> corner {{1,1},{-1,1},{-1,-1},{1,-1}};

class Vertex {
    public :
        int x, y;
        int dist;
        char data;

    int distance (Vertex &a) { return abs (x - a.x) + abs (y - a.y); }
};
class Graph {
    public :
        vector<vector<Vertex>> Vmap;
        vector<Vertex*> obj;
        vector<int> clust;

        Graph (const string &src) {
          
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

        Vertex *get (Point p) { return is_inside(p) ? &Vmap[p.y][p.x] : nullptr; }
        int getd (Point p) { return get(p) ? get(p)->dist : -1; }
        bool is_inside (Point p) { return (p.x >= 0 && p.y >= 0 && p.x < 5 && p.y < 5); }
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

        Point N = {V->x + compass[i].x, V->y + compass[i].y};
        Point E = {V->x + compass[j].x, V->y + compass[j].y};
        Point S = {V->x + compass[k].x, V->y + compass[k].y};
        Point W = {V->x + compass[l].x, V->y + compass[l].y};
        Point NE = {V->x + corner[i].x, V->y + corner[i].y};

        if (G.getd(N) == id && G.getd(S) == id && G.getd(E) != id && G.getd(W) != id) return false;
        if (G.getd(N) == id && G.getd(E) == id && G.getd(NE) != id) return false;
    }
    return true;
}

vector<Vertex *> get_clust (Graph &G, int id) {
    vector<Vertex *> cluster;
    Point p;

    for (p.y = 0; p.y < 5; ++p.y) {
        for (p.x = 0; p.x < 5; ++p.x) {
            Vertex *V = G.get(p);
          
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
        Point nxt = {p->x + dir.x, p->y + dir.y};
        nxtv = G.get(nxt);
      
        if (nxtv && inters (G, nxtv) && (nxtv->dist != p->dist)) {
            voter.push_back(nxtv);
        }
    }

    if (voter.size() == 0) return;

    bool seq2 = false;
    int toswap = voter.front()->dist;
    voter.front()->dist = id;

    for (auto &V : actual)
        if (inters (G, V)) {
            for (auto &dir : compass) {
                Point nxt = {V->x + dir.x, V->y + dir.y};
                if (G.is_inside (nxt)) {
                    nxtv = G.get (nxt);
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
    
    Point p;
    int cnt = 0;
    fill(G.clust.begin(), G.clust.end(), 0);
  
    for (p.y = 0; p.y < 5; ++p.y)
        for (p.x = 0; p.x < 5; ++p.x) 
            if (G.get(p)->data == 'O')
                G.clust[G.get(p)->dist]++;

    for (auto &it : G.clust)
        if (it > 2) cnt++;

    return cnt;
}
bool check (Graph &G) {

    Point p;
    int minv = 99;

    for (p.y = 0; p.y < 5; p.y++) {
      for (p.x = 0; p.x < 5; p.x++) {
        Vertex *V = G.get(p), *next = nearest (G.obj, V);

        if (V->data == 'O')
            minv = min (minv, V->distance (*next));
      }
    }
    return minv < 2 ? true : false;
}
string gerrymander (string src) {

    Graph G (src);
    if (check (G) == false) return "";

    random_device rd; 
    mt19937 gen (rd());
    uniform_int_distribution<> rng (0, 4);

    string os;

    while (update_cnt(G) != 3)
        scan_cluster (G, G.get ({rng(gen),rng(gen)}));
    
    Point p;
    for (p.y = 0; p.y < 5; ++p.y) {
        for (p.x = 0; p.x < 5; ++p.x) 
            os += G.get(p)->dist + '1';
        
        if (p.y < 4)
            os += '\n';
    }

    return os;
}
