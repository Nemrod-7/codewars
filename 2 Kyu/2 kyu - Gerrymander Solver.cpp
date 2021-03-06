#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {int x, y;};

const vector<Point> compass {{-1,0},{1,0},{0,-1},{0,1}};

class Vertex {
    public :
        int x, y;
        int dist, state;
        char data;

    int distance (Vertex &a) { return abs (x - a.x) + abs (y - a.y); }
    Vertex nearest (vector<Vertex*> &points) {
        Vertex V;
        int min = 999, dist;

        for (auto &p : points) {

            dist = distance (*p);
            if ((x != p->x) || (y != p->y)) {
                if (dist < min) {
                    min = dist;
                    V = *p;
                }
            }
        }
        return V;
    }
};
class Graph {
    public :
        vector<vector<Vertex>> Vmap;
        vector<Vertex> edge;
        vector<Vertex*> edge2;
        int clust[5];

        Graph (const string &src) {
            Vmap.resize (5, vector<Vertex> (5));
            for (int y = 0; y < 5; ++y) {

                Vertex V ;
                clust[y] = 0;
                for (int x = 0; x < 5; ++x) {
                    Vertex V = {x, y, y, 0, src[y * 6 + x]};
                    if (src[y * 6 + x] == 'O') {

                        edge2.push_back(&Vmap[y][x]);
                        clust[y]++;
                    }
                    Vmap[y][x] = V;
                }
            }
        }

        Vertex *get (Point p) { return &Vmap[p.y][p.x]; }
};
class Display {
    public :
        static void regions (const Graph &G) {
            for (int y = 0; y < 5; ++y) {
                for (int x = 0; x < 5; ++x) {
                    cout << G.Vmap[y][x].dist;
                }
                cout << endl;
            }
        }
        static void pt (const Point p) {
            cout << '[' << p.x << ',' << p.y << ']' << endl;
        }
        static void weights (Graph &G) {
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    Vertex V = G.Vmap[y][x];

                    if (V.data == 'O') {
                        Vertex next = V.nearest (G.edge2);
                        cout << V.distance (next) << ' ';
                    } else {
                        cout << "# ";
                    }
                }
                cout << endl;
            }
        }

};

bool is_inside (Point p) { return (p.x >= 0 && p.y >= 0 && p.x < 5 && p.y < 5); }

bool inters (Graph G, Point p) {

    int dist[5] = {0};

    for (auto &dir : compass) {
        Point nxt = {p.x + dir.x, p.y + dir.y};
        if (is_inside (nxt)) {

            Vertex *V = G.get(nxt); // Vertex V = G.Vmap[nxt.y][nxt.x];
            dist[V->dist]++;

            if (dist[V->dist] > 1) return false;
        }
    }
    return true;
}
Point nxt_intersection (Graph G, Point p) {
    int iy = p.y, ix = p.x;

    for (p.y; p.y < 5; p.y++) {
        p.x = (iy == p.y ? ix : 0);

        for (; p.x < 5; p.x++)
            if (inters (G, p))  return p;
    }
}

int main () {

    string region;

      region =
      "OXOOX\n"
			"XXOXO\n"
			"XOXXX\n"
			"XXOXX\n"
			"OXXOO";

    Graph G (region);
    Point p = {0, 0};

    //Vertex next = V.nearest (G.edge);

    Display::weights(G);

    /*
    region =
    "ABCDE\n"
    "FGEHI\n"
    "JKLMN\n"
    "OPQRS\n"
    "TUVWX";
    */

}
////////////////////////////////////////////////////////////////////////////////
