#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <queue>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>

#include <chrono>
// NEQ5xBztxeg43aP
using namespace std;

char getsep (string name) {
    int maxv = 0;
    char sep;
    string buff;
    map<char,int> hist;
    istringstream file (name);

    while (getline (file, buff)) {
        for (char ch : buff) {
            if (ispunct (ch)) {
                hist[ch]++;
            }
        }
    }

    for (auto it : hist) {
        if (it.second > maxv) {
            maxv = it.second;
            sep = it.first;
        }
    }

    return sep;
}
vector<int> tokenize (const string &src, char delim) {
    istringstream iss (src);
    string token;
    vector<int> v;

    while (getline (iss, token, delim))
        v.push_back (stoi (token));

    return v;
}
vector<vector<int>> getfile (string name) {
    int num;
    string buff;
    fstream file (name);
    vector<vector<int>> mat;

    while (getline (file, buff)) {
        mat.push_back(tokenize (buff, ' '));
    }

    file.close();
    return mat;
}

vector<uint64_t> sieve (uint64_t num) {

    bool *primes = new bool[num + 1];
    vector<uint64_t> sieve {2};

    fill_n (primes, num + 1, true);

    for (uint64_t p = 3; p * p <= num ; p += 2) {
        if (primes[p] == true) {
            for (uint64_t i = p * p; i <= num; i += 2 * p) {
                primes[i] = false;
            }
        }
    }

    for (uint64_t i = 3; i <= num; i += 2) {
        if (primes[i] == true) {
            sieve.push_back(i);
        }
    }

    delete[] primes;
    return sieve;
}

uint64_t tau (uint64_t n) { // count number of divisors
    uint64_t total = 1;

    for (; (n & 1) == 0; n >>= 1) // Deal with powers of 2 first
        ++total;

    for (uint64_t p = 3; p * p <= n; p += 2) { // Odd prime factors up to the square root
        uint64_t count = 1;
        for (; n % p == 0; n /= p)
            ++count;
        total *= count;
    }

    if (n > 1) total *= 2; // If n > 1 then it's prime

    return total;
}


string p_factors (uint64_t num) {
    ostringstream os;

    for (uint64_t k = 2; k * k <= num; k++) {
        int ex = 0;

        while (num % k == 0) {
            num /= k;
            ex++;
        }

        if (ex > 0) {
            os << k;
            if (ex > 1) os << "^" << ex;
            if (num > 1) os << " * ";
        }
    }

    if (num > 1) os << to_string(num);
    return os.str();
}

bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
vector<uint64_t> factorize (uint64_t n) {

    vector<uint64_t> factors;

    for (uint64_t k = 2; k < n; k++)
        if (n % k == 0)
            factors.push_back(k);

    return factors;
}

uint64_t fibonacci (uint64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci (n - 1) + fibonacci (n - 2);
}
uint64_t sumdig (uint64_t num) {
    uint64_t sum = 0;
    while (num) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

string collatz (uint64_t n) {

    string os;

    while (n != 1) {
        os += to_string (n) + " -> ";
        n = (n % 2 == 0) ? n * 0.5 : 3 * n + 1;
    }

    return os + "1";
}

int pyramidsum (string file) {
    vector<vector<int>> tri = getfile(file);

    for (int y = (tri.size() - 2); y >= 0; y--) {
        for (int x = 0; x <= y; x++) {
            tri[y][x] += max (tri[y + 1][x], tri[y + 1][x + 1]);
        }
    }

    return tri[0][0];
}

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

using vertex = pair<int, pair<int,int>>;

void project82 () {
  vector<int> path = {131,201,96,342,746,422,121,37,331};

  vector<vector<int>> grid = {
    {131,673,234,103,18},
    {201,96,342,965,150},
    {630,803,746,422,111},
    {537,699,497,121,956},
    {805,732,524,37,331}};

  //grid = getfile("p081_matrix.txt");

  int size = grid.size();
  pair<int,int> exit = {size-1, size-1 };
  const vector<pair<int,int>> compass = {{0,1}, {1,0} , {0,-1}};

  priority_queue<vertex, vector<vertex>, greater<vertex>> q1;
  vector<vector<int>> dist (size, vector<int> (size));
  vector<vector<int>> visit (size, vector<int> (size));

  for (int i = 0; i < 0; i++) {
      q1.push({grid[0][i], {i,0}});
  }

  while (!q1.empty()) {
      auto [cost, u] = q1.top();
      q1.pop();

      dist[u.second][u.first] = cost;
      visit[u.second][u.first] = 1;
      if (u == exit) break;

      for (auto dir : compass) {
          int nx = u.first + dir.first;
          int ny = u.second + dir.second;

          if (nx>= 0 && ny >= 0 && nx < size && ny < size) {
              int alt = cost + grid[ny][nx];

              if ( dist[ny][nx] < alt && !visit[ny][nx]) {
                  q1.push({alt, {nx, ny}});
              }
          }
      }
  }

  for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
          cout << setw(5) << dist[i][j];
      }
      cout << endl;
  }

  /*

      for (int i = 1; i < size; i++) {
          grid[0][i] = grid[0][i] + grid[0][i-1];
          grid[i][0] = grid[i][0] + grid[i-1][0];
      }

      for (int i = 1; i < size; i++) {
          for (int j = 1; j < size; j++) {
              grid[i][j] = grid[i][j] + min (grid[i][j-1],grid[i-1][j]);
              //cout << setw(5) << grid[i][j];
          }
      }

      for (int i = 0; i < size; i++) {
          for (int j = 0; j < size; j++) {
              cout << setw(5) << grid[i][j];
          }
          cout << endl;
      }
      */
}

void mult () {
  vector<int> power {2};

  for (int k = 1; k < 1000; k++) {
      //cout << fast_power (2, k) << endl;
      int rem = 0;
      for (int i = 0; i < power.size(); i++) {
          int sq = power[i] * 2;
          power[i] = sq % 10 + rem;
          rem = sq / 10;
      }

      if (rem > 0)
          power.push_back(rem);
  }

  int sum = 0;
  for (int i = power.size() - 1; i >= 0; i--) {
      //cout << power[i];
      sum += power[i];
  }

  cout << sum;

}

bool isPentagonal (uint64_t N) {
    double n = (1 + sqrt(24*N + 1))/6;
    return (n - (uint64_t) n) == 0;
}

int cntdiv (uint64_t num) {
    int np = 0;

    while ((num &1) == 0) {
        num >>= 1;
        np++;
        if (np > 2) return np;
    }

    for (uint64_t p = 3; p * p <= num; p += 2) {
        while (num % p == 0) {
            num /= p;
            np++;
            if (np > 2) return np;
        }
    }
    if (num > 1) np++;
    return np;
}

vector<uint64_t> sieve3 (uint64_t num) {

    uint64_t half = (num >> 1) + 1;
    vector<bool> primes (half + 1);
    vector<uint64_t> sieve {2};

    for (uint64_t p = 3; p * p <= num ; p += 2) {
        if (primes[p/2] == true) {
            for (uint64_t i = p * p; i <= num; i += 2 * p) {
                primes[i/2] = false;
            }
        }
    }

    for (uint64_t i = 3; i <= num; i += 2) {
        if (primes[i/2] == true) {
            sieve.push_back(i);
        }
    }

    return sieve;
}

int phi (int num) { // totient funtion

    int res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0) {
            num /= 2;
        }
        res -= res / 2;
    }

    for (int pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0) {
                num /= pr;
            }
            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}
int phi2 (int num) { // totient funtion

    int res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0) {
            num /= 2;
        }
        res *= (1 - (1 / (double) 2));
    }

    for (int pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0) {
                num /= pr;
            }
            res *= (1 - (1 / (double) pr));
        }
    }

    return (num > 1) ? res - res / num : res;
}
int main () {

    auto start = std::chrono::high_resolution_clock::now();

    const int lim = 1e6;
    int index = 0;
    double maxv = 0;

    vector<int> ndiv (lim);

    vector<int> hist (10);

    int num = 656;
    /*
    do {
        hist[num % 10]++;
    } while (num /= 10);
    */

    for (int k = 2; k < lim; k++) {
        double val = k / static_cast<double> (phi2 (k));

        if (val > maxv) {
            maxv = val;
            index = k;
        }
        /*
        cout << k << " => ";
        cout << ndiv[k] << ' ' << val << endl;
        */
    }
    cout << index;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
