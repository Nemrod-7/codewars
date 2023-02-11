#include "base.hpp"

#include <cmath>

using namespace std;

class check {
  public :
      static void overflow (uint64_t a, double b) {
          uint64_t limit = numeric_limits<uint64_t>::max() / b;
          //if (b > 1) limit /= b;
          if (a > limit) throw overflow_error ("integer overflow\n");

      }
};

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

char triangle (const std::string &row) { // insane colored triangle
    const std::vector<char> colors = {'B','R','G'};
    int N = row.size() - 1, sum = 0, k = row.size();

   while (k-->0)
        sum += lucas3 (N, k) * row[k] % 3;

    return colors[mod3 (pow (-1, N) * sum)];
}

inline uint64_t factorial (int n) {
  uint64_t mult = 1;
  for (int i = 1; i <= n ; ++i)
      mult *= i;

  return mult;
}

inline uint64_t binom_fact (int n, int k) { // binom_fact (9, 5); -> 10 015 656 in 1s
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
uint64_t binomial (int n, int k) {        // binomial (9,5); -> 9 795 281 in 1s
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

vector<vector<int>> mk_triangle (int size) {
    vector<vector<int>> tri;
    int num;
    for (int n = 0; n <= size; ++n) {
        tri.push_back(vector<int>());
        for (int k = 0; k <= n; ++k) {
            //num = round (binomial (n, k));
            num = lucas3 (n, k);

            tri[n].push_back (num);
        }
    }

    return tri;
}
void display_tri (vector<vector<int>> &tria) {

    for (auto &n : tria) {
        cout << string ((tria.size() - n.size()) * 2, ' ');
        for (auto &k : n) {
            if (k)
                cout << setw(3) << k << ' ';
            else
                cout << "    ";
        }
        cout << endl;
    }
}

int main () {

  Timer clock;



  clock.stop();
  clock.get_duration();

  return EXIT_SUCCESS;
}

/////////////////////////////////Arkive////////////////////////////////////////
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
