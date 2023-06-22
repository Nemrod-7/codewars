#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <limits>
#include <chrono>

using namespace std;

class fibgen {
    private :
      const int64_t lim = 10000000, mod = 1000000;
      int64_t rn;
      vector<int64_t> seed;

    public :
      fibgen() {
          seed.resize (lim);
          int64_t *rng = seed.data();

          for (int64_t k = 0; k < 56; k++) {
             rn = 100003 - 200003 * k + 300007 * k*k*k;
             rng[k] = rn % mod;
          }

          for (int64_t k = 56; k < lim; k++) {
             rn = rng[k-24] + rng[k-55];
             rng[k] = rn % mod;
          }
      }

      int64_t operator [] (int64_t k) {
          int64_t size = seed.size();

          if (k >= size) {
              for (int64_t i = size; i <= k; i++) {
                  rn = seed[i-24] + seed[i-55];
                  seed.push_back (rn % mod);
              }
          }

          return seed[k];
      }
};

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;


    fibgen rn;
    int64_t ref = 524287;
    vector<set<int>> graph(1000000);
    int64_t maxv = 0;
    int k = 1, cnt = 0;
    //fstream os ("network.txt", ios::out);
    // cout << rn[3681161] << "\n"; // 910544
    /*
    while (graph[524287].size() < 2) {
       int64_t b = rn[2*k], a = rn[2*k-1];

       if (a != b) {
           graph[a].insert(b);
           graph[b].insert(a);
           // maxv = max (maxv, max (graph[a].size(), graph[b].size()));
       }
       k++;
    }
    */


    int i = 1;

    int x0 = rn[6*i-5] % 10000;
    int y0 = rn[6*i-4] % 10000;
    int z0 = rn[6*i-3] % 10000;

    int dx = 1 + rn[6*i-2] % 399;
    int dy = 1 + rn[6*i-1] % 399;
    int dz = 1 + rn[6*i-0] % 399;

    cout << x0 << " " << y0 << " " << z0 << "\n";
    cout << dx << " " << dy << " " << dz << "\n";



    /*
    */
    int cycle = 0;
    //
    // vector<int64_t> s1 {ref};
    // set<int64_t> hist;
    //
    // while (!s1.empty()) {
    //
    //    int64_t u = s1.back();
    //    s1.pop_back();
    //    hist.insert(u);
    //
    //    cycle++;
    //
    //    if (cycle == 150) {
    //        // break;
    //    }
    //
    //    for (auto index : graph[u]) {
    //       if (hist.find(index) == hist.end()) {
    //           s1.push_back(index);
    //       }
    //    }
    // }
    //
    // cout << cycle << " " << hist.size();

    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
