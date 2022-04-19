#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <cmath>
#include <random>

using namespace std;

void display (vector<list<int>> graph) {
    for (int i = 1; i < graph.size(); i++) {
        cout << setw(2) << i << " -> ";
        for (auto &edge : graph[i]) {
          cout << setw(2) << edge << " ";
        }
        cout << endl;
    }

    cout << endl;
}

void mkpath (int start, vector<list<int>> &graph) {

    map<int, bool> visit;
    int k = start, nxt;

    random_device rd;
    mt19937 gen (rd());

    while (true) {
        visit[k] = true;

        cout << setw(2) << k << " ";
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

    return order;
}

int main () {

    int N = 20;
    vector<list<int>> graph (N + 1);

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

    //display (graph);
    size_t minv = 999;

    for (int i = 1; i < N; i++)
        minv = min (minv, graph[i].size());

    vector<int> start;
    for (int i = 1; i < N; i++) {
        if (graph[i].size() == minv)
            start.push_back (i);
    }

    mkpath (start.front(), graph);

}
