#include <iostream>
#include <vector>
#include <cmath>

#include <algorithm>

using namespace std;

bool is_prime (int num) {
    if (num < 2) return false;
    if (num < 4) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
int diagonalspiral (int dim) {
    int lim = (dim - 1) / 2, sum = 1;

    for (int i = 1; i <= lim; i++) {
        sum += 16 * i * i + 4 * i + 4;
    }

    return sum;
}
void primespiral () {
  int dim = 3;

  int lim, np = 0, total;
  double rate = 100;
  int i = 1;

  while (rate > 10.0) {
      lim = (dim - 1) / 2, total = lim * 4 + 1;

      for (; i <= lim; i++) {

          int a = 4 * i * i + 1; // up left
          int b = a + 2 * i; // down left
          int c = a - 2 * i; // up right
          int d = a + 4 * i; // down right

          np += is_prime (a) + is_prime (b) + is_prime (c) + is_prime (d);
          rate = np * 100 / (double) total;
      }
      //if (rate < 10) break;
      //cout << rate << " ";
      dim += 2;
  }
}
uint64_t modpow (uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t x = 1, e = 0;

    while (e < exp) {
        e++;
        x = (base * x) % mod;
    }
    return x;
}

bool pandigital1 (int digit[10]) {

    for (int i = 1; i < 10; i++) {
        if (digit[i] != 1)
            return false;
    }
    return true;
}
bool pandigital2 (int num) {
    int digit[10] = {0};

    do { digit[num % 10]++; } while (num /= 10);

    if (digit[0]) return false;

    for (int i = 1; i < 10; i++) {
        if (digit[i] != 1)
            return false;
    }

    return true;
}
bool pandigital4 (int num) {
    int digit[10] = {0};
    int byte = 0;

    do {
        byte |= 1 << (num % 10);
    } while (num /= 10);

    return byte == 45;
  }
bool pandigital3 (const string &num) {
    int digit[10] = {0};

    for (int i = 0; i < num.size(); i++) {
        digit[num[i] - '0']++;
    }
    if (digit[0]) return false;

    for (int i = 1; i < 10; i++) {
        if (digit[i] != 1)
            return false;
    }

    return true;
  }

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }
int lcm (int a, int b) { return a * gcd (a,b) / b; }

bool bouncy (int num) {
    int pr = num % 10;
    bool dec = true, inc = true;

    do {
        int dig = num % 10;
        if (dig < pr) dec = false;
        if (dig > pr) inc = false;
        pr = dig;
    } while (num /= 10);
    //cout << dec << " " << inc;
    return dec == 0 && inc == 0;
}

int replace (string num, const vector<bool> &sieve) {

    int maxv = 0;
    for (int j = 0; j < 10; j++) {
        vector<int> pos;
        int np = 0;

        for (int k = 0; k < num.size(); k++) {
            if (num[k] == j + '0'){
                pos.push_back(k);
            }
        }

        if (pos.size()) {
            string ref = num;

            for (int l = 1; l < 10; l++) {
                for (int k = 0; k < pos.size(); k++) {
                    ref[pos[k]] = l + '0';
                }
                int idn = stoi(ref);
                if (sieve[idn] == true) {
                    np++;
                    cout << ref << " ";
                }
            }
            cout << "\n";
        }
        if (np > maxv) {
            maxv = np;
        }
    }
    return maxv;
}

int main () {

  int limit = 1e7;

  /*
  vector<bool> sieve (limit + 1, true);
  vector<int> prime {2};

  sieve[0] = sieve[1] = false;

  for (int i = 4; i <= limit ; i += 2) {
      sieve[i] = false;
  }

  for (int p = 3; p * p <= limit ; p += 2) {
      if (sieve[p] == true) {
          for (int i = p * p; i <= limit; i += 2 * p) {
              sieve[i] = false;
          }
      }
  }
  */





    std::cout << "\nend\n";
}