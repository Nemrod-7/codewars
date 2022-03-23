#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <numeric>
#include <algorithm>

#include <cstring>
using namespace std;
using vertex = pair<int,int>; // id, dist

class BestTravel {
    private :
        static int nearest_neighbour (int t, int k, const std::vector<int> &ls) {

            priority_queue <vertex> q1;
            map<int,bool> visited;

            q1.push ({ls[0], t - ls[0]});
            auto [u, dist] = q1.top();

            visited[u] = true;

            int minv = 9999, nxt = -1;
            for (int i = 0; i < ls.size(); i++) {

                if (visited[ls[i]] == false) {
                    int alt = t - (ls[i] + dist);

                    cout << alt << "\n";
                    if (alt > 0 && alt < minv) {
                      minv = alt;
                      nxt = ls[i];
                    }
                }
            }

            //cout << visited.size();
            return nxt;

        }
        static bool check (const vector<int> &vec, int size) {
          int hist[size] = {0};
          for (int i = 0; i < vec.size(); i++) {
              if (hist[vec[i]]) return false;
              //cout << vec[i] << " ";
              hist[vec[i]]++;
          }
          return true;
        }
    public:
        static int chooseBestSum (int t, int depth, const std::vector<int> &ls) { // simulated annealing ?

            int size = ls.size(), index = 0;
            vector<int> base (depth);

            while (base[depth] == 0) {

                if (check (base, size)) {
                    int sum = 0;
                    for (auto &it : base) sum += ls[it];

                    if (sum <= t)
                        cout << sum << " ";

                }

                base[0]++;

                while (base[index] == size) {
                    base[index++] = 0;
                    base[index]++;
                }
                index = 0;
            }

            return 0;
        }
};

void display (const vector<int> &tablex, const string &dict) {
  for (int i = 0; i < tablex.size(); i++) {
    cout << dict[tablex[i]] << ' ';
  }
  cout << endl;
}

bool check (const vector<int> &vec, int size) {
  int hist[size] = {0};
  for (int i = 0; i < vec.size(); i++) {
      if (hist[vec[i]]) return false;
      //cout << vec[i] << " ";
      hist[vec[i]]++;
  }
  return true;
}

void combinations (int depth, const string &dict) {

    int size = dict.size(), index = 0;
    vector<int> base (depth);

    while (base[depth] == 0) {

      string pass;
      for (auto &it : base) pass += dict[it];
      cout << pass << endl;

      base[0]++;

      while (base[index] == size) {
          base[index++] = 0;
          base[index]++;
      }
      index = 0;
    }
}
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

    //combinations (3, "abccde");

    cout << "end";
    vector<int> ts = {50, 55, 57, 58, 60};
    //cout << BestTravel::chooseBestSum (174, 3, ts);
    // testequal(n, 228);
}
