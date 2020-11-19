#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;
///////////////////////////////////Tests////////////////////////////////////////
typedef std::vector<int> test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            //if (actual != expression)
            //    cout<<"actual : "<<actual<<" expected : "<<expression<<endl;
        }
};
test_t Equals (test_t entry) {return entry;}
void Test();
///////////////////////////////////Graph///////////////////////////////////////
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

//////////////////////////////////Func def///////////////////////////////////////
int dijkstra (string src);

/////////////////////////////////////////////////////////////////////////////////
int main () {

    auto start = chrono::high_resolution_clock::now();


    Test();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;
}

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

vector<int> i_am_here(string path) {

    for (auto &n : path)
        cout << (int)n << endl;
    return {0, 0};
}
void Test () {

    Assert::That(i_am_here(""      ), Equals(std::vector<int>{0, 0}));
    Assert::That(i_am_here("RLrl"  ), Equals(std::vector<int>{0, 0}));
    Assert::That(i_am_here("r5L2l4"), Equals(std::vector<int>{4, 3}));

}
