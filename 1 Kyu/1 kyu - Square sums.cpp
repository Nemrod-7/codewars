#include <iostream>
#include <iomanip>
#include <vector>

#include <stack>

#include <map>
#include <cmath>
#include <random>
#include <algorithm>

#include <chrono>

using namespace std;

class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t index;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :
        Timer () {
            alpha = chrono::steady_clock::now ();
            index = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            update();
            cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << endl;
        }
        bool running (double total) {
            update();
            index++;
            if (elapsed.count() < total) return true;
                cout << "index :: " << index << endl;
            return false;
        }
};

using edge = pair<int,int>;
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
        static void path (vector<int> src) {

            for (auto &it : src)
                cout << it << " ";

            cout << endl;
        }
};


bool isquare (int a, int b) {
    int sum = a + b, sq = sqrt (sum);
    return (sq * sq == sum) ? true : false;
  }

void mkpath (int start, vector<vector<int>> &graph) {

    map<int, bool> visit;
    int k = start, nxt;

    random_device rd;
    mt19937 gen (rd());

    while (true) {
        visit[k] = true;

        cout << k << " ";
        auto it = graph[k].begin();

        while (it != graph[k].end()) {
            if (visit[*it]) {
                it = graph[k].erase (it);
            } else {
                it++;
            }
        }

        if (graph[k].size() == 0) break;

        uniform_int_distribution<> dist (0, graph[k].size() - 1);
        //cout << index << endl;
        it = next (graph[k].begin(), dist (gen));

        nxt = *it;
        graph[k].erase (it);
        k = nxt;
    }
    cout << " :: " << visit.size();

}
void search2 (int start, vector<vector<int>> graph) {

    vector<int> res;
    vector<bool> visit (graph.size() + 1);
    stack<pair<int, vector<int>>> q1;

    q1.push ({start, {}});

    while (!q1.empty()) {

        auto [u, path] = q1.top();
        q1.pop();

        path.push_back (u);

      //  Display::path (path);
        if (path.size() == graph.size() - 1) {
            res = path;
            break;
        }

        vector<int> poss;

        for (auto &nxt : graph[u]) {
            if (find (path.begin(), path.end(),nxt) == path.end()) {
                //poss.push_back (nxt);
                q1.push ({ nxt, path });
            }
        }

    }
    cout << " -> ";
    Display::path (res);

}
void search (int start, vector<vector<int>> graph) {

    string res;
    stack<pair<int,string>> q1;
    q1.push ({start, {}});

    while (!q1.empty()) {

        auto [u, path] = q1.top();
        q1.pop();

        path += u;

        if (path.size() == graph.size() - 1) {
            res = path;
            break;
        }

        for (auto &nxt : graph[u]) {
            if (path.find (nxt) == string::npos) {
                q1.push ({nxt, path});
            }
        }
    }

    for (auto &it : res) {
        cout << static_cast<int> (it) << ' ';
    }

    cout << endl;
}
void search3 (int start, vector<vector<int>> graph) {

    vector<int> res;
    vector<bool> base (graph.size() + 1);
    stack<tuple<int, vector<bool>, vector<int>>> q1;

    q1.push ({start, base, {}});

    while (!q1.empty()) {

        auto [u, visit, path] = q1.top();
        q1.pop();

        visit[u] = true;
        path.push_back (u);
        //Display::path (path);
        if (path.size() == graph.size() - 1) {
            res = path;
            break;
        }

        for (auto &nxt : graph[u]) {
            if (!visit[nxt]) {
                q1.push ({nxt, visit, path});
            }
        }
    }

    cout << " -> ";
    Display::path (res);

}
void hamiltonpath (vector<vector<int>> &adjlist, int v, vector<bool> visited, vector<int> &path, int N) {

  	if (path.size() == N) {
  		for (int i : path)
  			cout << i << " ";
  		cout << endl;

  		return;
  	}

  	for (int w : adjlist[v]) {
    		if (!visited[w]) {
      			visited[w] = true;
      			path.push_back(w);

      			hamiltonpath (adjlist, w, visited, path, N);

      			visited[w] = false;
      			path.pop_back();
    		}
    }
}
vector<int> squaresums (int N) {

    if (N < 15) return {};

    vector<int> order;
    vector<vector<int>> graph (N + 1);

    for (int a = 1; a < N; a++) {
        for (int b = a + 1; b < N + 1 ; b++) {
            int sum = a + b, sq = sqrt (sum);

            if (sq * sq == sum) {
                graph[a].push_back (b);
                graph[b].push_back (a);
                //cout << setw(2) << a << " " << setw(2) << b << " :: " << sum << endl;
            }
        }
    }
    size_t minv = 999;

    for (int i = 1; i < N; i++)
        minv = min (minv, graph[i].size());

    vector<int> start;

    for (int i = 1; i < N; i++) {
        if (graph[i].size() == minv)
            start.push_back (i);
    }

    vector<bool> visited (N + 1);
    order.push_back (start.front());
    visited[start.front()] = true;

    //hamiltonpath (graph, start.front(), visited, order, N);
    //search2 (start.front(), graph);
    search3 (start.front(), graph);
    return order;
}
int main () {

    Timer clock;
    int N = 15;

    int mid = N / 2;

    int a = 12, b = 13;

    squaresums (35);

    clock.stop();
    clock.get_duration();
}
