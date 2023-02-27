#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>

#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

int pythagoreantriplet (int sum) {
    const int s2 = sum / 2;
    const int lim = ceil(sqrt(s2)) - 1;
    int cnt = 0;

    for (int m = 2; m <= lim; m++) {
        if (s2 % m == 0) {
            int sm = s2 / m;
            int k = (m % 2 == 1) ? m + 2 : m + 1;

            while (sm % 2 == 0)
                sm /= 2;

            while ((k < (2 * m)) && (k <= sm)) {
                if (sm % k == 0 && gcd (k,m) == 1) {
                    int d = s2 / (k * m);
                    int n = k - m;

                    int a = d * (m * m - n * n);
                    int b = 2 * d * m * n;
                    int c = d * (m * m + n * n);
                    cnt++;
                    //printf ("[%i, %i %i] ", a,b,c);
                }
                k += 2;
            }
        }
    }

    if (cnt != 0) {
        cout << s2 << " " << cnt << endl;
    }

    return cnt;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<int>> adj {{0,16,12,21,0,0,0}, {16,0,0,17,20,0,0}, {12,0,0,28,0,31,0}, {21,17,28,0,18,19,23}, {0,20,0,18,0,0,11}, {0,0,31,19,0,0,27}, {0,0,0,23,11,27,0}};
    vector<vector<int>> grp (adj.size());

    set<int> weight;

    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj[i].size(); j++) {
              //cout << setw(2) << adj[i][j] << " ";
              weight.insert (adj[i][j]);
              if (adj[i][j] != 0) {
                grp[i].push_back(j);
                grp[j].push_back(i);
              }
        }
        //cout << endl;
    }

    int lim = 15e5;

    map<int,int> hist;


    for (int i = 2; i < 50; i++) {
        int cnt = pythagoreantriplet (i);

        if (cnt > 0) {
          //    cout << "[" << i << "]\n";

            hist[i] = cnt;
        }
        /*
        for (int j = i + 1; j < 10000; j++) {
            int c = i * i + j * j;
            int sq = sqrt (c);

            if (sq * sq == c) {
                int per = i + j + sq;
                //cout << i << ' ' << j << endl;
                //cout << per << " ";
                hist[per]++;
            }
        }
        */
    }

    for (auto [per, freq] : hist) {
        if (freq > 0) {

        //    cout << "[" << per << "]" << freq << endl;
        }
    }
    /*
    */


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
