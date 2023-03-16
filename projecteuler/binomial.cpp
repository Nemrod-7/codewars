#include "base.hpp"

#include <cmath>
#include <set>

using namespace std;

class check {
  public :
      static void overflow (uint64_t a, double b) {
          uint64_t limit = numeric_limits<uint64_t>::max() / b;
          //if (b > 1) limit /= b;
          if (a > limit) throw overflow_error ("integer overflow\n");

      }
};

uint64_t factorial (int n) {
  uint64_t mult = 1;
  for (int i = 1; i <= n ; ++i)
      mult *= i;

  return mult;
}

uint64_t binom_fact (int n, int k) { // binom_fact (9, 5); -> 10 015 656 in 1s : limit n = ~20, k ~20
    return factorial (n) / (factorial (k) * factorial (n - k));
}
double binom_beta (int n, int k) {     // binom_beta (9, 5); -> 6 042 814 in 1s
    return 1 / ((n + 1) * std::beta (n - k + 1, k + 1));
}
uint64_t binom_recurs (int n, int k) {    // binom_recurs (9, 5); -> 1 208 161 in 1s
  if (k == 0 || k == n) return 1;
  if (k  < 0 || k  > n) return 0;
  return binom_recurs (n - 1, k - 1) + binom_recurs (n - 1, k);
}
uint64_t binomial_mul (int n, int k) {        // binomial_mul (9,5); -> 9 795 281 in 1s
  // => binom (n k) = sum from i = 0 to k ((n + 1 - i) / i)
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;

    long double res = 1, limit;

    k = std::min (k, n - k);
    for (double i = 1; i <= k; i++)
        //check::overflow (res, num);
        res = res * (n + 1 - i) / i;

    return static_cast<uint64_t> (res);
}
uint64_t binomial (int n, int k) {        // binomial (9,5);
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;
    const uint64_t lim = n;
    uint64_t tri[lim+1][lim+1];

    for (int i = 0; i <= lim; ++i) {
        tri[i][0] = tri[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            tri[i][j] = tri[i-1][j-1] + tri[i -1][j];
        }
    }

    return tri[n][k];
}

inline int mod3 (int n) {

    if (n < 0) return ((n %= 3) < 0) ? n + 3 : n;
    if (n * 0xAAAAAAAB <= n) return 0;
    return n % 3;
}
int lucas3 (int n, int k) {

    int prod = 1, np, kp;
    // => lucas (n k) = prod from i = 0 to infinity (binom(n_i k_i) mod p)
    do {
        np = mod3 (n), kp = mod3 (k);
        //cout << '(' << np << ' ' << kp << ") => " << binomial (np, kp) << endl;
        if (np < kp) return 0;
        if (np == 2 && kp == 1) prod *= 2; //prod *= binomial (np, kp);

        n /= 3, k /= 3;

    } while (n || k);

    return mod3 (prod);
}
int lucasth (int n, int k, int p) {

    int prod = 1, np, kp;
    //vector<vector<int>> base = mk_triangle (9);
    // => lucas (n k) = prod from i = 0 to infinity (binom(n_i k_i) mod p)
    do {
        np = n % p, kp = k % p;
        //cout << '(' << np << ' ' << kp << ") => " << binomial (np, kp) << endl;
        if (kp > np) return 0;
        prod *= binomial (np, kp);

        n /= p, k /= p;

    } while (n || k);

    return prod % p;
}

vector<vector<uint64_t>> mk_triangle (int size) {
    vector<vector<uint64_t>> tri;
    uint64_t num;
    for (int n = 0; n <= size; ++n) {
        tri.push_back(vector<uint64_t>());
        for (int k = 0; k <= n; ++k) {
            //num = round (binomial (n, k));
            num = binomial (n, k);

            tri[n].push_back (num);
        }
    }

    return tri;
}
void display_tri (vector<vector<uint64_t>> &tri) {

    for (auto &n : tri) {
        cout << string ((tri.size() - n.size()) * 2, ' ');
        for (auto &k : n) {
            if (k)
                cout << setw(3) << k << ' ';
            else
                cout << "    ";
        }
        cout << endl;
    }
}


int harshad (int num) {

  int ref = num, sum = 0;

  do {
      sum += ref % 10;
  } while (ref /= 10);

  return (sum && num % sum == 0) ? sum : 0;
}
bool strong (int num, const vector<bool> &sieve) {
    if (num == 0) return false;
    int div = harshad (num);
    return (div != 0 && sieve[num / div] == true);
}
bool rightrunc (int num) {

    while (num && harshad (num)) {
        num /= 10;
    }
    return num == 0;
}

void count (int lim) {
  // Find the number of entries which are not divisible by 7 in the first one billion (10^9) rows of Pascal's triangle.

  using u64 = uint64_t;
  vector<int> seq;
  //fstream ofs ("triangle", ios::out);
  vector<vector<u64>> tri (lim+1, vector<u64> (lim + 1));

  int cnt = 0, dig;
  for (int n = 0; n < lim ; n++) {
      tri[n][0] = tri[n][n] = 1;
      //cout << setw(3) << n << " => ";
      cout << string ((lim - n) , ' ');

      for (int k = 1; k <= n; ++k) {

          /*
          dig = lucasth (n, k, 7);

          if (dig != 0) {
              cnt++;
          }
          if (dig != 0) {
            cout << dig << ' ';
          } else {
            cout << "  ";
          }
          */

      }
      /*
      cout << " :: " << cnt;
      */
      cout << '\n';
  }
  cout << cnt;
}

uint64_t cntline7 (int num) {
    uint64_t cnt = 1;

    while (num != 0) {
        int dig = num % 7;
        cnt *= (dig + 1);
        num /= 7;
    }

    return cnt;
}

uint64_t project198_1 (int lim) {
  int dig;
  uint64_t cnt = 0;

  for (int n = 0; n < lim ; n++) {
      //cnt += cntline7 (n);
      for (int k = 0; k <= n; ++k) {
          dig = lucasth (n, k, 7);

          if (dig != 0) {
              cnt++;
          }
          /*
          if (dig != 0) {
            cout << dig << ' ';
          } else {
            cout << "  ";
          }
          */
      }
      /*
      if (n % 49 == 48) {
        cout << "    <--- ";
      }
      cout << '\n';
      */
  }
  return cnt;
}
uint64_t project198_2 (int lim) {

  uint64_t cnt = 0;
  int n = 0, cycle = 1;

  while (n + 49 < lim) {
      cnt += 784 * cycle;
      cycle++;
      n += 49;
  }

  for (; n < lim ; n++) {
      cnt += cntline7 (n);
  }

  return cnt;
}

int main () {

    Timer clock;

    // Find the number of entries which are not divisible by 7 in the first one billion (10^9) rows of Pascal's triangle.

    uint64_t lim = 1e9;
    int cnt1 = 0, cnt2 = 0, dig;

    uint64_t num = lim, rev = 0;
    int base = 7;

    while (num != 0) {
      int dig = num % base;
      //cout << dig;
      rev = rev * 10 + dig;
      num /= base;
    }

    cout << rev << '\n';

    //cout << '\n' << cnt1 <<  " :: " << cnt2;

    clock.stop();
    clock.get_duration();

    return EXIT_SUCCESS;
}
