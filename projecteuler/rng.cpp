#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <chrono>

using namespace std;

class fibgen {
    private :
      const int64_t mod = 1000000;
      int64_t rn;
      vector<int64_t> seed;

    public :
      fibgen() {
          for (int i = 0; i < 56; i++) {
              int64_t i3 = i * i * i;
              rn = 100003 - 200003 * i + 300007 * i3;
              seed.push_back (rn % mod);
          }
          for (int i = 56; i < 100000; i++) {
              rn = seed[i-24] + seed[i-55];
              seed.push_back (rn % mod);
          }

      }
      int64_t operator [] (int k) {
          int64_t size = seed.size();

          if (k >= size) {
              for (int i = size; i <= k; i++) {
                  rn = seed[i-24] + seed[i-55];
                  seed.push_back (rn % mod);
              }
          }

          return seed[k];
      }
};

int64_t rng (int64_t k) {
    if (k < 56) {
        int64_t i3 = k * k * k;
        return (100003 - 200003 * k + 300007 * i3) % 1000000;
    } else {
        return (rng(k-24) + rng(k-55)) % 1000000;
    }
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    fibgen rn;
    int64_t ref = 524287;
    vector<set<int>> graph;
    //map<int64_t,set<int64_t>> graph;
    //int64_t maxv = 0;
    //
    //for (int i = 0; i < 4592917; i++) {

    //    int64_t b = rn[2*i], a = rn[2*i-1];
    //    maxv = max (maxv, max (a,b)); 
    //    if (a != b) {
    //        graph[a].insert(b);
    //        graph[b].insert(a);
    //        // maxv = max (maxv, max (graph[a].size(), graph[b].size()));
    //    }
    //}

    //auto it = graph.begin();


    //cout << maxv;

    //for (int i = 0; i < graph.size(); i++) {
    //    for (auto &index : graph[i]) {

    //    }
    //}
    ////int cycle = 0;
    //cout << graph[ref].size();
    ////cout << graph[ref].size();  

    //vector<int64_t> s1 {ref};
    //set<int64_t> hist;
  

    //while (!s1.empty()) {

    //    int64_t u = s1.back();
    //    s1.pop_back();
    //    hist.insert(u);

    //    cycle++;

    //    if (cycle == 16) {
    //        break;
    //    }

    //    for (auto index : graph[u]) {
    //        s1.push_back(index);
    //        //cout << index << " ";
    //    }
    //}
    //
    //cout << hist.size();
    
    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
