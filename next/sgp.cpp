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
    }
    static void graph (const vector<vector<int>> &hist) {

        for (int i = 0; i < hist.size(); i++) {
            cout << tochar (i) << " : ";
            for (int j = 0; j < hist.size(); j++) {
                if (hist[i][j]) {
                    cout << tochar (j);
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


int main () {

    int n = 20, g = 4;

    vector<int> visit(n);
    vector<vector<int>> graph (n, vector<int> (n));

    // vector<int> key(20);
    // generate(key.begin(), key.end(), [n = 0] () mutable { return n++; });
    //
    // while (cycle-->0) {
    //     vector<pair<int,int>> duo;
    //
    //     for (int i = 0; i < n / 2; i++) {
    //         int a = i * 2, b = n - a - 1;
    //         // printf("%2i %2i|", key[a], key[b]);
    //         duo.push_back({key[a],key[b]});
    //     }
    //     // cout << endl;
    //     rotate(key.begin(), key.begin() + 1, key.end());
    // }

    // sgp(8, 4, 5);

    cout << "\nend\n";
}


struct vertex {
    int index;
    vector<int> visit;
    vector<vector<int>> key;
    vector<vector<int>> hist;
};

void sgp (int num, int grp, int day) {

    vector<vector<int>> start (num / grp, vector<int> (grp));
    vector<vector<int>> graph (num, vector<int> (num));

    for (int i = 0; i < (num / grp); i++) {
        for (int j = 0; j < grp - 1; j++) {
            int a  = i * grp + j;
            for (int k = j + 1; k < grp; k++) {
                int b  = i * grp + k;
                graph[a][b] = graph[b][a] = true;
            }
        }
    }

    int cycle = 0;
    vertex source = {0, vector<int> (num), start, graph};
    vector<vertex> stack {source};

    const vector<string> exit = {"TPLH","SOKG","RNJD","QMFB","IEBA"};

    while (!stack.empty()) {
        auto [index, visit, key, hist] = stack.back();
        int x = index / grp, y = index % grp;
        stack.pop_back();

        cycle++;
        if (cycle == 1000000) break;

        if (index == num) {
            // for (int i = 0; i < (num / grp); i++) {
            //     for (int j = 0; j < grp - 1; j++) {
            //         int a  = i * grp + j;
            //
            //         for (int k = j + 1; k < grp; k++) {
            //             int b  = i * grp + k;
            //             graph[a][b] = graph[b][a] = true;
            //         }
            //     }
            // }

            Show::key(key);
        } else if (index < num) {

            for (int i = 0; i < num; i++) {
                if (!visit[i]) {
                    bool free = true;

                    for (int j = 0; j < y; j++) {
                        if (hist[key[x][j]][i] == true) {
                            free = false;
                            break;
                        }
                    }

                    if (free == true) {
                        vertex next = {index + 1, visit, key, hist};

                        next.visit[i] = true;
                        next.key[x][y] = i;

                        for (int j = 0; j < y; j++) {
                            int a = key[x][j];

                            next.hist[a][i] = true;
                            next.hist[i][a] = true;
                        }

                        stack.push_back (next);
                    }
                }
            }
        }
    }

}
