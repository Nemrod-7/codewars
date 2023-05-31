#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include "base.hpp"
// NEQ5xBztxeg43aP
using namespace std;

void showvec (const vector<int64_t> &v) {
  for (auto it : v) {
      cout << it << " ";
  }
  cout << "\n";
}

class check {
  public :
      static void overflow (int64_t a, int64_t b) {
          int64_t limit = numeric_limits<int64_t>::max() / b;
          if (a > limit) throw overflow_error ("integer overflow\n");


      }
};

string p_factors (int64_t num) {
    ostringstream os;

    for (int64_t k = 2; k * k <= num; k++) {
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
int64_t gcd (int64_t a, int64_t b) { return b == 0 ? a : gcd (b, a % b); }
int64_t lcm (int64_t a, int64_t b) { return a * gcd (a,b) / b; }

bool issquare (int64_t num) {
    int64_t sq = sqrt(num);
    return sq * sq == num;
}
bool is_prime (int64_t num) {

    if (num < 2) return false;
    if (num < 4) return true;
    if (num % 2 == 0 || num % 3 == 0 ) return false;

    for (int64_t i = 5; i * i  <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
vector<int64_t> sieve (int64_t num) {

    int64_t half = (num >> 1) + 1;
    std::vector<bool> primes (half + 1);
    std::vector<int64_t> sieve {2};

    for (int64_t p = 3; p * p <= num ; p += 2) {
        if (primes[p/2] == false) {
            for (int64_t i = p * p; i <= num; i += 2 * p) {
                primes[i/2] = true;
            }
        }
    }

    for (int64_t i = 3; i <= num; i += 2) {
        if (primes[i/2] == false) {
            sieve.push_back(i);
        }
    }

    return sieve;
}

int64_t tau (int64_t n) { // count number of divisors
    int64_t total = 1;

    for (; (n & 1) == 0; n >>= 1) // Deal with powers of 2 first
        ++total;

    for (int64_t p = 3; p * p <= n; p += 2) { // Odd prime factors up to the square root
        int count = 1;
        for (; n % p == 0; n /= p)
            ++count;
        total *= count;
    }

    if (n > 1) total *= 2; // If n > 1 then it's prime
    return total;
}
int64_t sigma (int64_t num) { // sum of proper divisors

  int64_t n = num, sum = 1;
  int64_t p = 2;

  while (p * p <= n && n > 1) {
    if (n % p == 0) {
      int64_t j = p * p;
      n /= p;

      while (n % p == 0) {
        j *= p;
        n /= p;
      }

      sum = sum * (j - 1) / (p - 1);
    }
    p += (p == 2) ? 1 : 2;
  }

  if (n > 1) sum *= (n + 1);

  return sum - num;
}
int64_t phi (int64_t num) { // totient funtion

    int64_t res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0)
            num /= 2;

        res -= res / 2;
    }

    for (int64_t pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0)
                num /= pr;

            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}
int64_t phi2 (int64_t num, vector<int64_t> &prime) { // totient funtion

    int64_t res = num;
    int64_t *p = prime.data();

    for (int64_t i = 0; p[i] * p[i] <= num; i++) {
        if (num % p[i] == 0) {
            while (num % p[i] == 0)
                num /= p[i];

            res -= res / p[i];
        }
    }

    return (num > 1) ? res - res / num : res;
}
vector<int64_t> phi3 (int64_t lim) { // sieve of totient
    vector<int64_t> sieve (lim + 1);

    for (int64_t i = 0; i <= lim; i++)
        sieve[i] = i;

    for (int64_t i = 2; i <= lim; i++) {
        if (sieve[i] == i) {
            for (int64_t j = i; j <= lim; j += i)
                sieve[j] -= sieve[j] / i;
        }
    }
    return sieve;
}

int64_t modpow (int64_t base, int64_t exp, int64_t mod) {
    int64_t res = 1;

    while (exp > 0) {
       if ((exp & 1) > 0) res = (res * base) % mod;
       exp >>= 1;
       if (exp > 0)
         base = (base * base) % mod;
    }

    return res;
  }
int64_t digsum (int64_t num) {
    int64_t sum = 0;
    while (num) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
  }

int64_t fibonacci (int64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci (n - 1) + fibonacci (n - 2);
}
string collatz (int64_t n) {

    string os;

    while (n != 1) {
        os += to_string (n) + " -> ";
        n = (n % 2 == 0) ? n * 0.5 : 3 * n + 1; }
    return os + "1";
}
void collatz2 (int64_t a1) {
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

bool isPentagonal (int64_t N) {
    double n = (1 + sqrt(24*N + 1))/6;
    return (n - (int64_t) n) == 0;
}
bool check_goldbach (int64_t num, const vector<int64_t> &prime) {

    for (int64_t i = 0; i < prime.size() && prime[i] < num; i++) {
        for (int64_t k = 1; k * k < num; k++) {
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
int64_t cntdiv (int64_t num) {
    int np = 0;

    while ((num &1) == 0) {
        num >>= 1;
        np++;
        if (np > 2) return np;
    }

    for (int64_t p = 3; p * p <= num; p += 2) {
        while (num % p == 0) {
            num /= p;
            np++;
            if (np > 2) return np;
        }
    }
    if (num > 1) np++;
    return np;
}

int64_t reverse (int64_t num) {

    int64_t rev = 0;

    do {
        rev = rev * 10 + num % 10;
    } while (num /= 10);

    return rev;
}
bool palindrome (int64_t num) {
    if (num % 10 == 0) return false;
    return reverse (num) == num;
}

int64_t radical (int64_t n) {
  // A007947 -> r(n) -> Π p|n
  if (is_prime (n)) return n;

  int64_t res = (n % 2 == 0) ? 2 : 1;
  while (n % 2 == 0) n /= 2;

  for (int64_t i = 3; i * i <= n; i += 2) {
    if (n % i == 0) {
      res *= i;
      while (n % i == 0) n /= i;
    }
  }

  if (n != 1) res *= n;
  return res;
}
double resilience (int64_t d) {
    return phi (d) / static_cast<double> (d-1);
}

int64_t sumdigsum (int64_t n) {
    int64_t nu[10] = {0,1,2,3,4,5,6,7,8,9};

    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 10; j++) {
            if (digsum(nu[j]) == n) {
                return nu[j];
            }
            nu[j] = nu[j] * 10 + 9;
        }
    }

    return 0;
}

int main () {

    Timer chrono;

    const int64_t mod = 1000000007;
    const int64_t limit = 20;

    vector<vector<int>> vg = {{4,4,1},{9,36,4},{12,12,3},{20,20,5},{28,28,7},{44,44,11},{52,52,13}};
    double a = 9.0, b = 36.0;
    int n = 100; // 3072
    // probelm 206      Concealed Square
    // 1_2_3_4_5_6_7_8_9_0
    string number = "1020304050607080900";

    //cout << number.size();
    // problem510 (100);

    chrono.stop();
    chrono.get_duration();
}
