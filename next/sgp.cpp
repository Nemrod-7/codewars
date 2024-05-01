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

struct vertex {
    int index;
    int visit;
    vector<vector<int>> key;
};

void sgp (int num, int grp, int day) {

    vector<vector<int>> start (num / grp, vector<int> (grp));
    vector<vector<int>> graph (num, vector<int> (num));

    // for (int i = 0; i < (num / grp); i++) {
    //     for (int j = 0; j < grp - 1; j++) {
    //         int a  = i * grp + j;
    //         for (int k = j + 1; k < grp; k++) {
    //             int b  = i * grp + k;
    //             graph[a][b] = graph[b][a] = true;
    //         }
    //     }
    // }

    int cycle = 0;
    vertex source = {0, 0, start};
    vector<vertex> stack {source};

    const vector<string> exit = {"TPLH","SOKG","RNJD","QMFB","IEBA"};
    int count = 0;

    while (!stack.empty()) {
        auto [index, visit, key] = stack.back();
        int x = index / grp, y = index % grp;
        stack.pop_back();

        cycle++;
        if (cycle == 1000000) break;

        if (index == num) {
            for (int i = 0; i < (num / grp); i++) {
                for (int j = 0; j < grp - 1; j++) {
                    int a  = i * grp + j;

                    for (int k = j + 1; k < grp; k++) {
                        int b  = i * grp + k;
                        graph[a][b] = graph[b][a] = true;
                    }
                }
            }

            count++;
            // Show::key(key);
        } else if (index < num) {

            for (int i = 0; i < num; i++) {
                if ((visit >> i &1) == 0) {
                    bool free = true;

                    for (int j = 0; j < y; j++) {
                        if (graph[key[x][j]][i] == true) {
                            free = false;
                            break;
                        }
                    }

                    if (free == true) {
                        vertex next = {index + 1, visit, key};

                        next.visit |= 1 << i;
                        next.key[x][y] = i;

                        stack.push_back (next);
                    }
                }
            }
        }
    }

    printf("count : %i\n", cycle);
}

int main () {

    int n = 8, g = 4;
    int cycle = n - 1;

    vector<int> index(n - 1);
    vector<int> visit(n);
    vector<vector<int>> graph (n, vector<int> (n));

    generate(index.begin(), index.end(), [n = 1] () mutable { return n++; });

    // while (cycle-->0) {
        vector<vector<int>> tablex, cluster;
        index.push_back(0);

        for (int i = 0; i < n / 2; i++) {
            int a = i * 2, b = n - a - 1;
            printf("%2i %2i|", index[a], index[b]);
            tablex.push_back({index[a],index[b]});
        }


        for (int i = 0; i < tablex.size(); i++) {
            int a = tablex[i][0];
            int b = tablex[i][1];

            if (cluster.size() < (n / g)) {
                visit[a] = true;
                visit[b] = true;

                cluster.push_back(tablex[i]);
            } else {

                for (int j = 0; j < cluster.size(); j++) {
                    int csize = cluster[j].size();

                    if (csize < g) {
                        for (int k = 0; k < cluster[j].size(); k++) {
                            int num = cluster[j][k];

                            if (graph[num][a] == false) {

                            }

                            if (graph[num][b] == false) {

                            }

                        }
                    }

                }
            }


        }



        printf("\n");
        index.pop_back();
        rotate(index.begin(), index.begin() + 1, index.end());
    // }


    //sgp(n, g, 5);

    cout << "\nend\n";
}
