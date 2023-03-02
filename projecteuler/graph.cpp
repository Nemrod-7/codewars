#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include "base.hpp"

#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

void showadj (const vector<vector<int>> &adj) {
    int size = adj.size();
    vector<vector<int>> graph (size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (adj[i][j] != 0) {
                graph[i].push_back(j);
            }
        }
    }

    for (int i = 0; i < graph.size(); i++) {
        cout << static_cast<char> (i + 65) << " => ";

        for (int j = 0; j < graph[i].size(); j++) {
            cout << static_cast<char> (graph[i][j] + 65) << " ";
        }
        cout << "\n";
    }

}
pair<int,int> search (const vector<vector<int>> &adj, int val) {
    pair<int,int> pt;

    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj[i].size(); j++) {
            if (adj[i][j] == val) {
                pt = {i,j};
            }
        }
    }
    return pt;
}

bool connected (const vector<vector<int>> &adj) {
    const int size = adj.size();
    vector<int> visit (size);
    vector<int> stack {0};

    while (!stack.empty()) {
        int level = stack.back();
        stack.pop_back();
        visit[level] = true;

        for (int i = 0; i < size; i++) {
            if (adj[level][i] != 0 && !visit[i]) {
                stack.push_back(i);
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (visit[i] == false) {
            return false;
        }
    }
    return true;
}
vector<vector<int>> reduce (vector<vector<int>> adj) {
    vector<vector<int>> visit = adj;
    const int size = adj.size();

    vector<int> dist;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int cell = visit[i][j];
            if (cell != 0) {
                dist.push_back(cell);
                visit[i][j] = 0;
                visit[j][i] = 0;
            }
        }
    }

    sort (dist.begin(), dist.end());
    visit = adj;

    while (!dist.empty()) {

        int weight = dist.back();
        dist.pop_back();
        auto [x,y] = search (visit, weight);

        adj[x][y] = adj[y][x] = 0;
        visit[x][y] = visit[y][x] = 0;

        if (!connected(adj)) {
            adj[x][y] = weight;
            adj[y][x] = weight;
        }
    }

    return adj;
}

int getweight (const vector<vector<int>> &adj) {
    int sum = 0;
    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj.size(); j++) {
            sum += adj[i][j];
        }
    }
    return sum / 2;
}

int main () {

    Timer chrono;

    vector<vector<int>> adj {{0,16,12,21,0,0,0}, {16,0,0,17,20,0,0}, {12,0,0,28,0,31,0}, {21,17,28,0,18,19,23}, {0,20,0,18,0,0,11}, {0,0,31,19,0,0,27}, {0,0,0,23,11,27,0}};

    using vertex = pair<int,int>; // weight, id

    const int size = adj.size();
    vector<vertex> edge;
    vector<vector<int>> graph (size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (adj[i][j] != 0) {
                int id = j, weight = adj[i][j];
                //graph[i].push_back(j);
                edge.push_back({weight,j});
            }
        }
    }
    sort (edge.begin(), edge.end());

    int i = 0;
    auto [weight, id] = edge[i];
    cout << id << " => ";

    int minv = 999, nxv;

    for (int j = 0; j < size; j++) {
        int node = adj[id][j];

        if (node > 0 && node != weight) {
            if (node < minv) {
                minv = node;
                nxv = j;
            }
        }
    }

    graph[id].push_back(nxv);
    cout << nxv;

    /*
    for (int i = 0; i < graph.size(); i++) {
        cout << static_cast<char> (i + 65) << " => ";
        //sort (edge[i].begin(), edge[i].end());
        for (int j = 0; j < graph[i].size(); j++) {
            //cout << edge[i][j].first << " ";
            //cout << static_cast<char> (tree[i][j].second + 65) << " ";
        }
        cout << "\n";
    }
    */
    /*
    auto file = getfile ("files/p107_network.txt");
    // find the maximum saving weight while verifying the graph remains connected
    const int size = file.size();
    vector<vector<int>> adj = txt2int (file);
    vector<vector<int>> visit = adj;
    int total = getweight (adj);

    adj = reduce (adj);

    cout << total - getweight (adj);
    */
    chrono.stop();
    chrono.get_duration();
}
