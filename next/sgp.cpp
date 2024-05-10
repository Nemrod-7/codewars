#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

class Show {
    public:
    static char tochar (int nu) { return nu + 'A'; }
    static void vec (const vector<int> &vec) {
        for (auto &ch : vec) {
            cout << ch << ' ';
        }
            cout << endl;
    }
    static void graph (const vector<vector<int>> &graph) {

        for (int i = 0; i < graph.size(); i++) {
            cout << tochar (i) << " : ";
            for (int j = 0; j < graph.size(); j++) {
                if (graph[i][j]) {
                    cout << Show::tochar (j) << ":" << graph[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
    static void key(const vector<vector<int>> &key) {
        for (int i = 0; i < key.size(); i++) {
          for (int j = 0; j < key[i].size(); j++) {
            cout << static_cast<char> (key[i][j] + 'A');
          }
          cout << " ";
        }
        cout << endl;
    }
};

void update(vector<vector<int>> &graph, vector<vector<int>> &key) {

    for (int i = 0; i < key.size(); i++) {
        for (int j = 0; j < key[i].size() - 1; j++) {
            for (int k = j + 1; k < key[i].size(); k++) {
                int a = key[i][j], b = key[i][k];

                graph[a][b]++;
                graph[b][a]++;
            }
        }
    }
}

vector<vector<int>> sgp_test (vector<int> index, vector<vector<int>> &graph, int g) {

    int n = index.size(), n2 = index.size() / 2;
    vector<int> visit(n);
    vector<vector<int>> tablex, cluster;

    for (int i = 0; i < n2; i++) {
        int a = i * 2, b = n - a - 1;
        tablex.push_back({index[a],index[b]});
    }

    while (cluster.size() < (n / g)) {
        vector<int> curr = tablex.back();

        for (auto &a : curr) {
            visit[a] = true;
        }

        tablex.pop_back();
        cluster.push_back(curr);
    }

    for (int i = 0; i < tablex.size(); i++) {
        for (int l = 0; l < 2; l++) {
            int a = tablex[i][l];

            //cout << Show::tochar(a) << ": ";
            for (int j = 0; j < cluster.size(); j++) {

                if (cluster[j].size() < g) {
                    bool free = true;

                    for (int k = 0; k < cluster[j].size(); k++) {
                        int b = cluster[j][k];

                        if (graph[a][b] == true) {
                            free = false;
                        }
                    }

                    if (!visit[a] && free == true) {
                        visit[a] = true;
                        cluster[j].push_back(a);
                    }

                }
            }
        }
    }

    return cluster;
}

int main () {

    const int n = 20, g = 4;
    const int n2 = n / 2;

    vector<int> index(n);
    vector<vector<int>> graph (n, vector<int> (n)), cluster;

    generate(index.begin(), index.end(), [n = 0] () mutable { return n++; });
    int cycle = 2;// n - 0;

    Show::vec(index);

    /*
*/

    //cluster = search_key(index, graph, g);

    //sgp(n, g, 5);
    cout << "\nend\n";
}
