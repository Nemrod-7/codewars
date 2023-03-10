#include <iostream>
#include <cmath>
#include <algorithm>
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

uint64_t sumdig (uint64_t num) {
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

int main () {

    Timer chrono;




  /*
    for (int i = 2; i <= 50000000; i++) {
        uint64_t pn = (2 * (i * i) - 1);

        //cout << setw(3) << i << " => " << setw(4) << pn ;
        //cout << " :: " << 2 * i * i;
        if (is_prime(pn) == true) {
            cnt++;
            //cout <<  " prime " ;
        }

        //cout << "\n";

    }
    */

    chrono.stop();
    chrono.get_duration();
}
