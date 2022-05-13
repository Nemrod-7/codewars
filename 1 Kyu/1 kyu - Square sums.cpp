#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <cmath>
#include <algorithm>
#include <random>

#include <chrono>
#define MAXN 512

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

bool isquare (int a, int b) {
    int sum = a + b, sq = sqrt (sum);
    return (sq * sq == sum) ? true : false;
}
vector<int> readpath2 (vector<int> order) {
    vector<int> path;

    for (int i = 1; i < order.size() + 1; i++) {
        for (int j = 0; j < order.size() + 1; j++)
            if (order[j] == i)
                path.push_back (j);
    }

    return path;
}
vector<int> a_star (int start, vector<vector<int>> &graph) {

    using vertex = tuple<int,int,vector<int>>;
    vertex src = {1, start, vector<int> (graph.size())};
    deque<vertex> q2;

    q2.push_front (src);

    while (!q2.empty()) {

        auto [index, u, visit] = q2.front();
        q2.pop_front();

        visit[u] = index;

        if (index == graph.size() - 1) {
          Display::vect (readpath2(visit));
            return readpath2(visit);
        }

        for (int &nxt : graph[u]) {
            if (!visit[nxt]) {
                q2.push_front ({index + 1, nxt, visit});
            }
        }
    }
    return {};
}
vector<int> idfs (int start, vector<vector<int>> &graph) {

    struct vertex {
      int idx, num;
      vector<int> visit;
    };
    vertex src = {1, start, vector<int> (graph.size())};
    stack<vertex> q2;

    q2.push (src);

    while (q2.size()) {

        auto [index, u, visit] = q2.top();
        q2.pop();

        visit[u] = index;

        if (index == graph.size() - 1) {
            Display::vect (readpath2(visit));
            return readpath2(visit);
        }

        for (int &nxt : graph[u]) {
            if (!visit[nxt]) {
                q2.push ({index + 1, nxt, visit});
            }
        }
    }
    /*
    */
    return {};
}

vector<int> searchrnd (vector<vector<int>> &graph, vector<int> path) {

    random_device rd;
    mt19937 gen(rd());

    int hist[graph.size()];

    while (true) {

        int u = path.back(), pos = 0;

        for (auto &nxt : graph[u]) {
            if (find (path.begin(), path.end(), nxt) == path.end())
                hist[pos++] = nxt;
        }

        if (pos == 0 || path.size() == graph.size()) break;
        uniform_int_distribution<> dist (0, pos - 1);
        path.push_back (hist[dist(rd)]);
    }

    return path;
}
vector<int> rndwalk (int start, vector<vector<int>> &graph) { // 0~1 s

    const float tmin = 0.01, alpha = 0.999;
    float T = 20000;

    while (T > tmin) {
        vector<int> path = searchrnd (graph, {start});

        if (path.size() == graph.size() - 1) {
            Display::vect (path);
            break;
        }

        T *= alpha;
    }

    return {};
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
list<pair<int,int>> mklist (int N) {

    list<pair<int,int>> adj;

    for (int a = 1; a < N; a++) {
        for (int b = a + 1; b < N + 1 ; b++) {
            int sum = a + b, sq = sqrt (sum);

            if (sq * sq == sum) {

                cout << setw(2) << a << " " << setw(2) << b << endl;
            }
        }
    }
    return adj;
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

int getstart (vector<vector<int>> &graph) {
    size_t minv = 999, start = graph.size();

    for (int i = 1; i < graph.size() + 1; i++) {
        minv = min (minv, graph[i].size());
    }

    while (graph[start].size() != minv) start--;

    return start;
}
vector<int> squaresums (int N) {

    if (N == 24) return {};
    if (N < 15) return {};
    if (N > 17 && N < 23) return {};

    vector<vector<int>> graph = mkgraph (N);

    for (auto &level : graph) {
        sort (level.begin(), level.end(), [&graph] (int a, int b) {
            if (graph[a].size() != graph[b].size())
                return graph[a].size() > graph[b].size();
            else
                return a < b;
        });
    }

    return a_star (getstart(graph), graph);
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

vector<int> makeseq (int start, vector<vector<int>> &graph) {

  vector<int> path = {start};

  while (true) {

      int u = path.back();

      for (auto &nxt : graph[u]) {
          if (find (path.begin(), path.end(), nxt) == path.end()) {
              path.push_back(nxt);
              break;
          }
      }

      if (path.back() == u) break;
  }
  return path;
}
size_t depth (vector<int> path, vector<vector<int>> &graph) {
    int dig = path.back();
    size_t maxv = 1;

    for (int num : graph[dig]) {
        if (find (path.begin(), path.end(), num) == path.end()) {
            vector<int> nxt = path;
            nxt.push_back (num);
            maxv = max (depth (nxt, graph) + 1, maxv);
        }
    }

    return maxv;
}

int main () {

    //ofstream out ("squaresums.csv");
    random_device rd;
    mt19937 gen(rd());

    vector<int> test = {50, 128, 256};

    for (auto N : test) {

        Timer clock;
        vector<vector<int>> graph = mkgraph (N);
        //vector<vector<int>> cost (N + 1, vector<int> (N + 1));

      //  Display::grph(graph);

        squaresums (N);

        clock.stop();
        clock.get_duration();
    }


    // out.close();

}
