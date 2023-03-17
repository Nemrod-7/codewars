#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include "base.hpp"
// NEQ5xBztxeg43aP
using namespace std;

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

uint64_t digsum (uint64_t num) {
    uint64_t sum = 0;
    while (num) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
  }

uint64_t fibonacci (uint64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci (n - 1) + fibonacci (n - 2);
  }
string collatz (uint64_t n) {

    string os;

    while (n != 1) {
        os += to_string (n) + " -> ";
        n = (n % 2 == 0) ? n * 0.5 : 3 * n + 1;
    }

    return os + "1";
}
void collatz2 (uint64_t a1) {
  const char alpha[3] = {'D','U','d'};
  string seq;
  cout << a1 << " => ";
  while (a1 > 1) {
      int mod = a1 % 3;

      switch (mod) {
        case 0 : a1 = a1 / 3; break;
        case 1 : a1 = (4 * a1 + 2) / 3; break;
        case 2 : a1 = (2 * a1 - 1) / 3; break;
        default: break;
      }
      seq += alpha[mod];
      cout << alpha[mod];
  }
}

bool isPentagonal (uint64_t N) {
    double n = (1 + sqrt(24*N + 1))/6;
    return (n - (uint64_t) n) == 0;
}
bool check_goldbach (int num, const vector<int> &prime) {

    for (int i = 0; i < prime.size() && prime[i] < num; i++) {
        for (int k = 1; k * k < num; k++) {
            if (prime[i] + 2 * (k * k) == num) {
                //cout << prime[i] << " + 2 x " << k << "²";
                return true;
            }
        }
    }

    return false;
}
void farey (int n) {

  typedef struct { int d, n; } frac;
	frac f1 = {0, 1}, f2 = {1, n}, tmp;
	int k;
	printf("%d/%d ", 1, n);
	while (f2.n > 1) {
			k = (n + f1.n) / f2.n;
			tmp = f1;
			f1 = f2;
			f2 = (frac) { f2.d * k - tmp.d, f2.n * k - tmp.n };
			std::cout << f2.d << "/" << f2.n << " ";
	}
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

uint64_t reverse (uint64_t num) {

    uint64_t rev = 0;

    do {
        rev = rev * 10 + num % 10;
    } while (num /= 10);

    return rev;
}
bool palindrome (uint64_t num) {
    if (num % 10 == 0) return false;
    return reverse (num) == num;
}

bool is_prime (int num) {
    if (num < 2) return false;
    if (num < 4) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}

uint64_t harshad (uint64_t num) {
  uint64_t sum = digsum (num);
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

int main () {

    Timer chrono;

    uint64_t limit = 1e15;
    int cnt = 0;

    for (int i = 1; i < 10; i++) {
      for (int j = 1; j < 20; j++) {
          uint64_t nu = pow (i,j);
          uint64_t len = log10 (nu) + 1;

          if (len == j) {
            //cout << nu << ' ';
            cnt++;
          }
      }
      //cout << '\n';
    }

    cout << cnt;

    /*

    int fp[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    problem 387 : harshad numbers

    vector<bool> sieve (limit + 1, true);
    vector<uint64_t> prime {2};

    sieve[0] = sieve[1] = false;

    for (uint64_t i = 4; i <= limit ; i += 2) {
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

    /*
    uint64_t res = 0;

    for (uint64_t p = 23; p * p <= limit ; p += 2) {

        if (sieve[p] == true) {
            int num = p / 10, div = harshad (p / 10);

            if (rightrunc (num) && sieve[num / div] ) {
                cout << num / div << ' ';
                res += p;
            }
        }

    }
    */

    /*

    vector<uint64_t> divs = {3,6,9,11,13,18,19,21};
    vector<uint64_t> nudiv = {2,3,5,7,19,37,47,67,73,89,223,227,229,467,3079,4447,19087,25579,382867,666667,1267579,1541539,2285743,3076939};


    for (int i = 1; i < nudiv.size(); i++) {
        cout << nudiv[i] - nudiv[i-1] << ' ';
    }
    for (auto div : divs) {
      for (auto num : nudiv) {

          cout << div * num << ' ';
    }
  }
    divisors : 3 6 9 11 13 18 19 21
    num / div : 2 3 5 7 19 37 47 67 73 89 223 227 229 467 3079 4447 19087 25579 382867 666667
    */
    // cout << res << " :: " ;


    chrono.stop();
    chrono.get_duration();
}
