#include <iostream>
#include <vector>

using namespace std;

char tochar (int nu) { return nu + 'A'; }

class Show {
    public:
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
};

int main () {

    int size = 20;
    int grp = 2;
    int s2 = size / grp;
    vector<vector<int>> graph (grp, vector<int> (size / grp));

    for (int i = 0; i < size; i++) {
        graph[i % grp][i / grp] = i;
    }

    for (int i = 0; i < grp; i++) {
        for (int j = 0; j < s2; j++) {
            cout << graph[i][j] << ' ';
        }
        cout << endl;
    }



    cout << "\nend\n";
}


struct vertex {
    int index;
    vector<int> visit;
    vector<vector<int>> key;
    vector<vector<int>> hist;
};

void golfer (int num, int grp, int day) {
    vector<vector<int>> start (num / grp, vector<int> (grp));
    vector<vector<int>> graph (num, vector<int> (num));

    for (int i = 0; i < (num / grp); i++) {
        for (int j = 0; j < grp - 1; j++) {
            int a  = i * 4 + j;
            for (int k = j + 1; k < grp; k++) {
                int b  = i * 4 + k;
                graph[a][b] = graph[b][a] = 1;
            }
        }
    }

    int cycle = 0;
    vertex source = {0, vector<int> (num), start, graph};
    vector<vertex> stack {source};

    while (!stack.empty()) {
        auto [index, visit, key, hist] = stack.back();
        int x = index / grp, y = index % grp;

        stack.pop_back();

        cycle++;
        if (cycle == 2) break;

        if (index == num) {
            for (int i = 0; i < (num / grp); i++) {
                for (int j = 0; j < grp; j++) {
                    cout << static_cast<char> (key[i][j] + 'A');
                }
                cout << " ";
            }
            cout << endl;
        } else {
            for (int i = 0; i < num; i++) {
                if (!visit[num]) {
                    bool valid = true;

                    for (int j = 0; j < y; j++) {
                        if (hist[key[x][j]][i] == true) {
                            valid = false;
                        }
                    }

                    if (valid) {
                        vertex next = {index + 1, visit, key, hist};

                        next.visit[i] = true;
                        next.key[x][y] = i;

                        for (int j = 0; j < y; j++) {
                            next.hist[key[x][j]][i] = true;
                            next.hist[i][key[x][j]] = true;
                        }

                        stack.push_back (next);
                    }
                }
            }
        }
    }

}
void grid()  {
    int size = 4;
    vector<vector<int>> graph (size, vector<int>(size));

    // 0 1 2 3
    // 1 0 3 2
    // 2 3 0 1
    // 3 2 1 0

    for (int i = 0; i < size; i++) {

        // graph[i][0] = i;
        // graph[0][i] = i;
        // graph[(size-1) - i][i] = size - 1;

        for (int j = 0; j < size; j++) {
            cout << (i^j) << ' ';
        }
        cout << '\n';
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // cout << graph[i][j] << ' ';
        }
        // cout << endl;
    }

}

void a_star (int size) {

    const int gsize = 2;
    const vertex source = {0, vector<int> (size), vector<vector<int>>(0), vector<vector<int>> (size, vector<int>(size))};

    const int s2 = size / gsize;
    vector<vertex> stack {source};
    vector<vector<int>> graph (size, vector<int>(size));

    while (!stack.empty()) {

        auto [index, visit, key, hist] = stack.back();
        stack.pop_back();

        bool valid = true;

        for (int i = 0; i < index; i++) {
            for (int j = 0; j < gsize-1; j++) {
                for (int k = j + 1; k < gsize; k++) {
                    int a = key[i][j], b = key[i][k];

                    if (graph[a][b]) {
                        valid = false;
                        break;
                    }
                }
            }
        }

        if (valid == false) continue;


        if (index == s2) {
            for (int i = 0; i < s2; i++) {
                for (int j = 0;j < gsize; j++) {
                    cout << tochar(key[i][j]);
                }
                cout << ' ';
            }
            cout << endl;

            for (int i = 0; i < s2; i++) {
                for (int j = 0; j < gsize-1; j++) {
                    for (int k = j + 1; k < gsize; k++) {
                        int a = key[i][j], b = key[i][k];
                        graph[a][b] = graph[b][a] = true;
                    }
                }
            }

        } else if (index < s2) {

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {

                    if (i != j && !visit[i] && !visit[j] ) {

                        if (!graph[i][j] && !graph[j][i]) {

                            vertex nextv = {index + 1, visit, key, hist};
                            nextv.visit[j] = true;
                            nextv.visit[i] = true;
                            nextv.hist[i][j] = true;
                            nextv.hist[j][i] = true;

                            if (i < j) {
                                nextv.key.push_back({i,j});
                            } else {
                                nextv.key.push_back({j,i});
                            }

                            stack.push_back(nextv);
                        }
                    }
                }
            }
        }
    }


}
