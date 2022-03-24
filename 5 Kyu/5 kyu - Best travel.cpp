#include <iostream>
#include <vector>
#include <map>

#include <queue>
#include <numeric>
#include <algorithm>

using namespace std;
using vertex = std::pair<int,int>; // id, dist

class BestTravel {
    private :
        inline static std::vector<int> ts;

        static int nearest_neighbour (int t, int k, const std::vector<int> &ls) {

            priority_queue <vertex> q1;
            map<int,bool> visited;

            q1.push ({ls[0], t - ls[0]});
            auto [u, dist] = q1.top();

            visited[u] = true;

            int minv = 9999, nxt = -1;
            for (int i = 0; i < ls.size(); i++) {
                int alt = t - (ls[i] + dist);

                if (visited[ls[i]] == false && alt > 0) {

                    cout << alt << "\n";
                    if (alt < minv) {
                      minv = alt;
                      nxt = ls[i];
                    }
                }
            }
            //cout << visited.size();
            return nxt;
        }

        static void search (std::vector<int> curr, int index, int lim, int &val) {

            if (val == lim) return;

            const int depth = curr.size();
            int sum = 0;

            for (int i = index; i < ts.size(); i++) {
                if (std::find (&curr[0], &curr[index], ts[i]) != &curr[index]) continue;

                curr[index] = ts[i];

                if (index == depth - 1) {
                    sum = std::accumulate (curr.begin(), curr.end(), 0);

                    if (sum <= lim) {
                        //std::cout << sum << " ";
                        val = std::max (val, sum);
                    }
                } else {
                    search (curr, index + 1, lim, val);
                }
            }
        }
    public:
        static int chooseBestSum (int t, int k, const std::vector<int> &data) { // simulated annealing ?

            if (k > data.size()) return -1;
            int maxv = 0;
            std::vector<int> arr (k);

            nearest_neighbour (t, k, data);
            ts = data;
            //search (arr, 0, t, maxv);

            return maxv;
        }
};

void display (const std::vector<int> &tablex, const std::string &dict) {
  for (int i = 0; i < tablex.size(); i++) {
    std::cout << dict[tablex[i]] << ' ';
  }
  std::cout << std::endl;
}

void combinations (int depth, const string &dict) {

    int size = dict.size(), index = 0;
    vector<int> base (depth);

    while (base[depth] == 0) {

      string pass;
      //for (auto &it : base) pass += dict[it];
      //cout << pass << endl;

      base[0]++;

      while (base[index] == size) {
          base[index++] = 0;
          base[index]++;
      }
      index = 0;
    }
}
/*
void combinations2 (int depth, const string &dict) {
  int digits[24], size = dict.size();
  int index, left;

  memset(digits, 0, sizeof(digits));

  while (true) {

      for (index = depth; index--; ) {
          printf ("%d ", digits[index]);
      }
      printf ("\n");

      for (index = depth; index--; ) {
          left = digits[index]++;

          if (left < size) {
              while (depth > index++) digits[index] = left;
              break;
          }
      }
      if (index < 0) break;
  }
}
*/
void simmulated_annealing () {

    int iteration = -1;

    double temperature = 10000.0;
    double deltaDistance = 0;
    double coolingRate = 0.9999;
    double absoluteTemperature = 0.00001;

    //LoadCities();
    /*
    double distance = GetTotalDistance (currentOrder);

    while (temperature > absoluteTemperature) {
        nextOrder = GetNextArrangement (currentOrder);

        deltaDistance = GetTotalDistance (nextOrder) - distance;

        //if the new order has a smaller distance
        //or if the new order has a larger distance but
        //satisfies Boltzman condition then accept the arrangement
        if ((deltaDistance < 0) || (distance > 0 && Math.Exp (-deltaDistance / temperature) > random.NextDouble())) {
            for (int i = 0; i < nextOrder.Count; i++)
                currentOrder[i] = nextOrder[i];

            distance = deltaDistance + distance;
        }

        //cool down the temperature
        temperature *= coolingRate;

        iteration++;
    }
    shortestDistance = distance;
    */
}


int main () {

        std::vector<int> ts = {202,175,407,246,213,383,258,184,253,69,428,134,355,257,40,38,430,149,78,248,109,161,437,93,257};

        for (int i = 0; i < ts.size(); i++) {
            for (int j = i; j < ts.size(); j++) {

            }
        }
        cout << BestTravel::chooseBestSum (502, 6, ts);
        std::cout << "end";
        /*
        cout << BestTravel::chooseBestSum(230, 4, {100,76,56,44,89,73,68,56,64,123,2333,144,50,132,123,34,89});

        cout << BestTravel::chooseBestSum (1007, 2, {498,288,129,439,237,217,56,273,484,287,240,38,116,456,242,204,107,140,367,128,396,289});
        */
    // testequal(n, 228);

}
