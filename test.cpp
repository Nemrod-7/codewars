#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

/*
 Product Partition           Score(sc)
[708, 2]                    1420  <---- maximum value
[472, 3]                     950
[354, 2, 2]                 1074
[354, 4]                     716
[236, 3, 2]                  723
[236, 6]                     484
[177, 2, 2, 2]               740    # equals to: (177^1 + 2^3) * 4
[177, 4, 2]                  549
[177, 8]                     370
[118, 3, 2, 2]               500
[118, 4, 3]                  375
[118, 6, 2]                  378
[118, 12]                    260
[59, 3, 2, 2, 2]             350    # equals to: (59^1 + 3^1 + 2^3) * 5
[59, 6, 2, 2]                276
[59, 4, 3, 2]                272
[59, 12, 2]                  219
[59, 8, 3]                   210
[59, 6, 4]                   207
[59, 24]                     166  <---- minimum value
 */

vector<int> SieveOfEratosthenes (int num) {
    const int end = sqrt (num);
    bool *primes = new bool[num + 1];
    vector<int> sieve;
    int p, i;

    fill_n (primes, num + 1, 0);

    for (p = 2; p <= end ; p++)
       if (primes[p] == false)
           for (i = p * p; i <= num; i += p)
                primes[i] = true;

    for (i = 2; i <= num; i++)
       if (primes[i] == false)
           sieve.push_back(i);

    return sieve;
}
string p_factors (int num) {

  vector<int> primes = SieveOfEratosthenes (num);

  for (auto &p : primes) {
      int ex = 0;

      while (num % p == 0) {
          num /= p;
          ex++;
      }

      if (ex > 0) {
          cout << p;
          if (ex > 1) cout << "^" << ex;
          if (num > 1) cout << " * ";
      }
  }

  return "";
}
int radical (int maxn, int n) {

    vector<int> primes = SieveOfEratosthenes (maxn);
    vector<pair<int,int>> hist;
    for (int k = 2; k <= maxn; k++) {
        int rad = 1;

        for (auto &p : primes) {
            if (k % p == 0) {
                rad *= p;
            }
            if (p >= k) break;
        }
        hist.push_back({rad, k});
    }
    sort(hist.begin(), hist.end());
    return hist[n - 1].second;
  }

bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
vector<int> factorize (int n) {

    vector<int> factors;

    for (int k = 2; k < n; k++)
        if (n % k == 0)
            factors.push_back(k);

    return factors;
}
int product (const vector<int> &clust) {
    int prod = 1;

    for (auto &num : clust)
        prod *= num;

    return prod;
}

vector<int> path; // it will store all current factors

void recurse(int max, int val) {

    if (val == 1) {
        for (int i = 0; i < path.size(); i++)
            cout << path[i] << " ";

        cout << endl;
        return;
    }

    for (int i = max; i > 1; i--) {
        if (val % i == 0) {
            path.push_back(i);
            recurse(i, val / i);
            path.pop_back();
        }
    }
}
void Output(int value) {
    cout << "Result for " << value << ": " << endl;
    recurse(value, value);
}

int main () {

    //  g(n,[2]);
    int n = 1416;
    vector<int> factors = factorize (n);
    vector<vector<int>> partition;

    //Output (1416);
    vector<int> line (5);

    vector<int> comb = {1,2};

    int total = 0, pad = 5;
    int x = 0;

    for (auto n : comb)
        total += n;

    pad -= total;

    for (auto n : comb) {

        while (n-->0)
            line[x++] = 1;

        x++;
    }

    for (auto cell : line) {
        cout << cell << " ";
    }

    /*
    for (int k = 2; k < 11; k++) {
        cout << "[" << k << "] => ";
        p_factors(k);

        cout << "\n";
    }
    */
}
