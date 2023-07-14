#include <iostream>
#include <iomanip>
#include <queue>
#include <map>
#include <chrono>

using namespace std;

int cut_log3 (const std::vector<int> &pr, int lim) {

    int maxv = 0;
    std::priority_queue<std::pair<int,int>> q1;
    std::map<std::pair<int,int>,bool> visit;

    q1.push ({0,0});

    while (!q1.empty()) {

        auto u = q1.top();
        auto &[dist, load] = u;
        q1.pop();
        visit[u] = true;

        for (size_t i = 1; i < pr.size(); i++) {
            int alt = dist + pr[i], sum = load + i;
            std::pair<int,int> nxtv = {alt,sum};
            if (sum > lim) break;

            if (alt > dist && !visit[nxtv]) {
                q1.push (nxtv);
                maxv = std::max (maxv, alt);
            }
        }
    }

    return maxv;
}
int cut_log2 (const std::vector<int> &pr, int lim) {

    static int mem[256] = {0};

    if (lim == 0) return 0;
    if (mem[lim] != 0) return mem[lim];
    int val = -1;

    for (int i = 1; i < lim + 1; i++) {
        val = std::max (val, pr[i] + cut_log2 (pr, lim - i));
    }
    mem[lim] = val;
    return val;
}
int cut_log (const std::vector<int> &pr, int lim) {

    vector<int> mem (lim + 1);

    for (int i = 1; i < lim + 1; i++) {
        int maxv = -1;
        for (int j = 1; j < i + 1; j++) {
            maxv = std::max (maxv, pr[j] + mem[i - j]);
        }
        mem[i] = maxv;
    }

    return mem[lim];
}

int main () {

  auto start = chrono::high_resolution_clock::now();

  std::vector<int> pr ({  0,   1,   5,   8,   9,  10,  17,  17,  20,  24, // 0X's
                         30,  32,  35,  39,  43,  43,  45,  49,  50,  54, // 1X's
                         57,  60,  65,  68,  70,  74,  80,  81,  84,  85, // 2X's
                         87,  91,  95,  99, 101, 104, 107, 112, 115, 116, // 3X's
                        119, 121, 125, 129, 131, 134, 135, 140, 143, 145, // 4X's
                        151}); // 50th element
    int n = 50;


    //    all mantle areas (the area of the curved surface)
    //    all top base areas minus the area covered by the cylinders placed above them.


    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << fixed << elapsed.count()  << " ms\n";
}
