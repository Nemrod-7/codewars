#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

struct Vertex {
    int x, y, w;
    int dist;
    int visit;
    Vertex *next;
};
struct comp {
    bool operator()(Vertex const *a, Vertex const *b ) {
        return a->dist > b->dist;
    }
};
class Graph {
    public:
        int height = 0, width = 0;
        vector<vector<Vertex>> Vmap;

        Graph(vector<string>& mountain) {
            height = mountain.size(), width = mountain.at(0).size();
            Vmap.resize (height, vector<Vertex> (width));

            for (int x = 0; x < height; ++x) {
                for (int y = 0; y < width; ++y) {
                    Vmap[x][y] = {
                        .x = x, .y = y, .w = (mountain[x][y] == '^' ? -1 : 0),
                        .dist = numeric_limits<int>::max(),
                        .visit = false
                    };
                }
            }
        }

        Vertex *coord (pair<int,int> p) { return &Vmap[p.first][p.second];}

        void display () {
            for (int x = 0; x < height; ++x) {
                for (int y = 0; y < width; ++y) {


                  if (Vmap[x][y].w > 0)
                      cout << Vmap[x][y].w;
                  else
                      cout << " ";

                }
                cout << "\n";
            }
            cout << "\n";
        }

};

////////////////////////////////////////////////////////////////////////////////
int peak_height(vector<string>& mountain);
void display (vector<string> Vmap);
////////////////////////////////////////////////////////////////////////////////
const vector<pair<int,int>> directions{{-1,0},{1,0},{0,-1},{0,1}};

int main () {

    auto start = chrono::high_resolution_clock::now();

    vector<string> mountain = {
               "^^^^^^        ",
               " ^^^^^^^^     ",
               "  ^^^^^^^     ",
               "  ^^^^^       ",
               "  ^^^^^^^^^^^ ",
               "  ^^^^^^      ",
               "  ^^^^        "};

    peak_height (mountain);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;

}
/*
bool is_inside (Graph *G, pair<int,int> p) {
    return (p.first >= 0 && p.second >= 0 && p.first < G->row && p.second < G->col);
}
bool is_free (Graph *G, pair<int,int> p) {
    return G->coord(p)->visit == false;
}
bool edge (Graph *G, Vertex *v) {

    if (v->x == 0 || v->y == 0 || v->x == G->row - 1 || v->y == G->col - 1)
        return true;

    int i = 4;
    while (i-->0) {
        pair<int,int> next = {v->x + directions[i].first, v->y + directions[i].second};
        Vertex *nxtv = G->coord (next);

        if (nxtv->w == -2)
            return true;
    }

    return false;
}
void reset (Graph *G) {

    for (int x = 0; x < G->row; ++x)
        for (int y = 0; y < G->col; ++y)
            G->Vmap[x][y].dist = numeric_limits<int>::max();
}
int dijkstra (Graph G, Vertex *source) {

    queue <Vertex*> Q;
    //priority_queue<Vertex*> Q;
    //priority_queue<Vertex*, vector<Vertex*>, comp> Q;
    Vertex *u, *nextv;
    int alt;

    source->dist = 0;
    Q.push(source);

    while (!Q.empty()) {

        u = Q.front();
        Q.pop();
        //cout << u->dist + 1 << endl;
        if (edge (&G, u)) break;
        //u->visit = true;
        for (auto now : directions) {
            pair<int,int> p{u->x + now.first , u->y + now.second};

            if (is_inside (&G, p)) {
                nextv = G.coord (p);

                alt = u->dist + 1;

                if (alt < nextv->dist) {
                    nextv->dist = alt;
                    Q.push (nextv);
                }
            }
        }
    }
    //cout << u->dist + 1;
    return u->dist + 1;
}
*/
bool frontier (vector<string>& mountain, pair<int,int> p) {

    if (p.first == 0 || p.second == 0 || p.first == mountain.size() - 1 || p.second == mountain[0].size() - 1)
        return false;

    int i = 4;
    while (i-->0) {
        pair<int,int> nxt = {p.first + directions[i].first, p.second + directions[i].second};

        if (mountain[nxt.first][nxt.second] == ' ')
            return false;
    }

    return true;
}
int peak_height(vector<string>& mountain) {

  int height = 0, found = 1;

    while(found--){

      for(int i = 0; i < mountain.size(); ++i) {
          for(int j = 0; j < mountain[i].size(); ++j) {
              if(mountain[i][j] == '^' && (
                i == 0 || j == 0 || i == mountain.size() - 1 || j == mountain[i].size() - 1 ||
                    mountain[i - 1][j] == ' ' ||
                    mountain[i][j + 1] == ' ' ||
                    mountain[i + 1][j] == ' ' ||
                    mountain[i][j - 1] == ' '))
                    mountain[i][j] = found = 1;
              cout << mountain[i][j];
          }
          cout << endl;
      }

      for(int i = 0; i < mountain.size(); ++i)
          for(int j = 0; j < mountain[i].size(); ++j)
              if(mountain[i][j] == 1)
                  mountain[i][j] = ' ';


      found && ++height;
    }
return height;

}
int peak_height2(vector<string>& mountain) {

    Graph G (mountain);
    int dist = 1;
    int height = mountain.size(), width = mountain[0].size();
    vector<vector<int>> Vmap (height, vector<int> (width));
    vector<string>grf = mountain;
    vector<string>temp = grf;
    bool valid;

    while (true) {

        valid = false;

        for(size_t x = 0; x < G.height; ++x) {
            for(size_t y = 0; y < G.width; ++y) {
                if (frontier (grf, {x,y}) == false) {
                    if (grf[x][y] == '^') {
                        valid = true;
                        temp[x][y] = ' ';
                        Vmap[x][y] = dist;
                    }
                }
            }
        }
        //display (grf);
        if (valid) {
            dist++;
        } else {
            break;
        }

        grf = temp;
    }

    for(size_t x = 0; x < height; ++x) {
        for(size_t y = 0; y < width; ++y) {
            if (Vmap[x][y] > 0)
                cout << Vmap[x][y];
            else
                cout << " ";
        }
        cout << "\n";
    }

    return dist;
}
void display (vector<string> Vmap) {

      int height = Vmap.size(), width = Vmap[0].size();

    for(size_t x = 0; x < height; ++x) {
        for(size_t y = 0; y < width; ++y) {
            if (Vmap[x][y] > 0)
                cout << Vmap[x][y];
            else
                cout << " ";
        }
        cout << "\n";
    }
}
