#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <stack>
#include <cmath>
#include <algorithm>
#include <random>

#include <chrono>
#define MAXN 128

using namespace std;

class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t cycle;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :

        Timer() {
            alpha = chrono::steady_clock::now ();
            cycle = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            std::cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << std::endl;
        }
        bool running (double total) {
            update();
            cycle++;
            if (elapsed.count() < total) return true;
            cout << "cycles :: " ;

            string num = to_string (cycle);
            int size = num.size(), index = 0;

            while (size-->0) {
                cout << num[index++];
                if (size % 3 == 0) cout << ' ';
            }
            cout << endl;
            return false;
        }
};

class Display {
    public :
        static void grph (vector<vector<int>> graph) {
            for (int i = 1; i < graph.size(); i++) {
                cout << setw(2) << i << " -> ";
                for (auto &edge : graph[i]) {
                  cout << setw(2) << edge << " ";
                }
                cout << endl;
            }

            cout << endl;
        }
        static void vect (vector<int> src) {

            for (auto &it : src)
                cout << setw(2) << it << " ";

            cout << endl;
        }
};

random_device rd;
mt19937 gen(rd());

bool isquare (int a, int b) {
    int sum = a + b, sq = sqrt (sum);
    return (sq * sq == sum) ? true : false;
}
vector<int> readpath (int order[MAXN], int N) {
    vector<int> path;

    for (int i = 1; i < N + 1; i++) {
        for (int j = 0; j < N + 1; j++)
            if (order[j] == i)
                path.push_back (j);
    }

    return path;
}

vector<int> a_star (int start, vector<vector<int>> &graph) { // 8.324

    struct vertex { int num, idx, visit[MAXN]; };
    vertex src = {start, 1, {}};
    vector<vertex> q2;

    int cnt = 0, cnt2 = 0;

    q2.push_back (src);

    while (q2.size()) {

        auto [u, index, visit] = q2.back();
        q2.pop_back();

        visit[u] = index;

        if (index == graph.size() - 1) {
            Display::vect (readpath (visit, graph.size()));
            return readpath (visit, graph.size());
        }

        //vector<pair<int,int>> hist;
        for (int &nxt : graph[u]) {
            if (!visit[nxt]) {
                //hist.push_back({graph[nxt].size(), nxt});
                vertex nxtv = {nxt, index + 1};

                for (int i = 0; i < graph.size(); i++) {
                  nxtv.visit[i] = visit[i];
                }

                q2.push_back (nxtv);
            }
        }

        //sort (hist.begin(), hist.end(), greater());
        /*
        for (auto &it : hist) {
            vertex nxtv = {it.second, index + 1};

            for (int i = 0; i < graph.size(); i++) {
              nxtv.visit[i] = visit[i];
            }

            q2.push_back (nxtv);
        }
        */
    }
    /*
    */
    return {};
}
vector<int> a_star2 (int start, vector<vector<int>> &graph, vector<int> hist) { // 8.324

    struct vertex { int num, idx; vector<int> v; };
    vertex src = {start, 1, hist};
    vector<vertex> q2;

    q2.push_back (src);

    while (q2.size()) {

        auto [u, index, visit] = q2.back();
        q2.pop_back();

        visit[u] = index;

        if (index == graph.size() - 1) {
            return visit;
        }

        for (int &nxt : graph[u]) {
            if (!visit[nxt]) {
                q2.push_back ({nxt, index + 1, visit});
            }
        }
    }

    return {};
}
vector<int> rndwalk (int start, vector<vector<int>> &graph) { // 0~1 s

    const int size = graph.size() + 1;
    random_device rd;
    mt19937 gen(rd());

    int hist[size];
    const float tmin = 0.01, alpha = 0.999;
    float T = 20000;

    while (T > tmin) {
        int u = start, index = 1;
        int visit[size] = {};
        //fill (visit, visit + size, 0);

        while (true) {

            visit[u] = index++;

            if (index == graph.size()) {
                Display::vect (readpath (visit, graph.size()));
                return readpath (visit, graph.size());
            }

            int pos = 0;
            for (int &nxt : graph[u]) {
                if (!visit[nxt]) {
                    hist[pos++] = nxt;
                }
            }

            if (pos == 0) break;
            uniform_int_distribution<> dist (0, pos - 1);
            u = hist[dist(rd)];
        }

        T *= alpha;
    }

    return {};
}
bool dfs2 (int num, vector<vector<int>> &adj, int visit[MAXN], int index) { // 12.265

  	if (index == adj.size() - 1) {
        for (int i = 1; i < adj.size(); i++) {
            if (!visit[i]) return false;
        }

        Display::vect (readpath (visit, adj.size() - 1));
    		return true;
  	}
  	for (int nxt : adj[num]) {
    		if (!visit[nxt]) {
      			visit[nxt] = index + 1;

      			if (dfs2 (nxt, adj, visit, index + 1)) return true;

      			visit[nxt] = false;
    		}

    }

    return false;
  }

vector<vector<int>> mkgraph (int N) {
    vector<vector<int>> adjlist (N + 1);

    for (int a = 1; a < N; a++) {
        for (int b = a + 1; b < N + 1 ; b++) {
            int sum = a + b, sq = sqrt (sum);

            if (sq * sq == sum) {
                adjlist[a].push_back (b);
                adjlist[b].push_back (a);
                //cout << setw(2) << a << " " << setw(2) << b << " :: " << sum << endl;
            }
        }
    }
    return adjlist;
}
vector<vector<int>> mkadj (int N) {
  vector<vector<int>> adj (N + 1, vector<int> (N + 1));

  for (int i = 1; i < N; i++) {
      for (int j = i + 1; j < N + 1 ; j++) {
          int sum = i + j, sq = sqrt (sum);
          //cout << setw(2) << i << " " << setw(2) << j << " :: " << sum << endl;
          if (sq * sq == sum) {
              adj[i][j] = true;
              adj[j][i] = true;
          }
      }
  }
  return adj;
}

vector<int> squaresums (int N) {

    if (N == 24) return {};
    if (N < 15) return {};
    if (N > 17 && N < 23) return {};

    vector<vector<int>> graph = mkgraph (N);
    //Display::grph (graph);
    size_t minv = 999;

    for (int i = 1; i < N + 1; i++) {
        minv = min (minv, graph[i].size());
    }

    int start = N;
    while (graph[start].size() != minv) start--;

    return rndwalk (start, graph);
}
bool is_valid (vector<vector<int>> graph) {

    size_t minv = 999, hist[graph.size()] = {0};

    for (int i = 1; i < graph.size(); i++) {
        //cout << graph[i].size() << ' ';
        hist[graph[i].size()]++;
    }

    if (hist[0]) return false;
    if (hist[1] > 2) return false;

    return true;
}
void addedge (vector<vector<int>> &graph, int a) {
    if (a >= graph.size()) graph.resize (a + 1);
    for (int b = 1; b < a; b++) {
        int sum = a + b, sq = sqrt (sum);
        if (sq * sq == sum) {
            //cout << setw(2) << a << " " << setw(2) << b << " :: " << sum << endl;
            graph[a].push_back (b);
            graph[b].push_back (a);
        }
    }
}

int main () {

    Timer clock;
    //ofstream out ("squaresums.csv");
    int N = 50;

    auto graph = mkgraph (N);
    Display::grph (graph);

    auto history = a_star2 (50, graph, vector<int> (N + 1));

  //addedge (graph, 16);

//Display::grph (graph);
    //squaresums (N);
    /*

    for (int N = 50; N < 65; N++) {
        auto graph = mkgraph (N);


        cout << N << " --> " ;

          cout << endl;
    }
    */

    /*

    /*
    for (int i = 0; i < N; i++) {
        table[i][index] = start;
    }


    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
          cout << table[i][j] << ' ';
      }
      cout << endl;
    }

    //Display::path (search (1, graph));

    for (int i = 1; i < N + 1; i++) {
        path.push_back (i);
        visit[i] = true;

        cout << setw(2) << i << " --> ";
        dfs (i, graph, visit, path);

        visit[i] = false;
        path.clear ();

        cout << endl;
    }

    for (int i = 50; i < 51; i++) {
        cout << i << " --> ";
        for (int num : squaresums (i)) {
            cout << num << ';';
        }
        cout << '\n';
    }
    */
    // out.close();

    clock.stop();
    clock.get_duration();
}

/*
int choose (list<int> &hist) {
    uniform_int_distribution<> dist (0, hist.size() - 1);
    auto it = hist.begin();

    advance (it, dist(rd));
    int nxt = *it;
    hist.erase (it);
    return nxt;
}
*/
bool dfs (int num, vector<vector<int>> &adj, vector<int> visit, vector<int> &path) { // 42.013

  	if (path.size() == adj.size() - 1) {
        for (int i : path)
            cout << i << ' ';
        cout << endl;
    		return true;
  	}

    //cout << num << ' ';
  	for (int nxt : adj[num]) {
      //cout << nxt << ' ';
    		if (!visit[nxt]) {
      			visit[nxt] = true;
      			path.push_back (nxt);

      			if (dfs (nxt, adj, visit, path)) return true;

      			visit[nxt] = false;
      			path.pop_back();
    		}
        /*
        */
    }

    return false;
}

vector<int> search (int start, vector<vector<int>> graph) {
    struct vertex2 {
        int num;
        vector<int> visit, path;
    };
    vertex2 src = {start, vector<int>(graph.size() + 1), {}};
    stack<vertex2> q1;

    q1.push (src);

    while (!q1.empty()) {

        auto [u, visit, path] = q1.top();
        q1.pop();

        visit[u] = true;
        path.push_back (u);

        //Display::path (path);
        if (path.size() == graph.size() - 1) {
            return path;
        }

        for (int nxt : graph[u]) {
            if (!visit[nxt]) {
                q1.push ({nxt, visit, path});
            }
        }
    }

    return {};
  }
vector<int> search2 (int start, vector<vector<int>> graph) {

    stack<tuple<int, uint64_t, string>> q1;

    q1.push ({start, 0, {}});
    while (!q1.empty()) {

        auto [u, visit, path] = q1.top();
        q1.pop();

        visit |= 1UL << u;
        path += u;

        if (path.size() == graph.size() - 1) {
            for (auto &dig : path)
                cout << static_cast<int> (dig) << ' ';
            cout << '\n';
            return {};
        }

        for (int nxt : graph[u]) {
            //cout << ((visit >> nxt & 1));
            if ((visit >> nxt & 1UL) == 0) {
                q1.push ({nxt, visit, path});
            }
        }
    }
    cout << "not found" ;
    /*
*/
    return {};
  }
vector<int> search4 (int start, vector<vector<int>> &graph) {

      struct vertex { int num, idx, visit[128]; };
      vertex src = {start, 1, {}};
      stack<vertex> q1;
      int N = graph.size(), adj[N][N] = {{0}};
      size_t maxv= 0;

      for (int i = 1; i < N; i++) {
          maxv = max (maxv, graph[i].size());
          for (int j = 0; j < graph[i].size(); j++) {
              adj[i][j] = graph[i][j];
              //cout << adj[i][j] << ' ';
          }
          //cout << endl;
      }
      q1.push (src);

      while (!q1.empty()) {

          auto [u, index, visit] = q1.top();
          q1.pop();

          visit[u] = index;

          if (index == graph.size() - 1) {
              //Display::vect (readpath (visit, graph.size()));
              return readpath (visit, graph.size());
          }

          for (int i = 0; i < maxv; i++) {
              int nxt = adj[u][i];
              if (nxt && !visit[nxt]) {
                  vertex nxtv = {nxt, index + 1};

                  for (int i = 0; i < graph.size(); i++) {
                    nxtv.visit[i] = visit[i];
                  }

                  q1.push (nxtv);
              }
          }

      }
      /*
      */
      return {};
}
