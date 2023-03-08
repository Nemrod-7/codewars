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
vector<int> factorize (int n) {

    vector<int> factors;

    for (int k = 2; k < n; k++)
        if (n % k == 0)
            factors.push_back(k);

    return factors;
}

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

bool issquare (int num) {
    int sq = sqrt(num);
    return sq * sq == num;
}
bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
vector<int> sieve (int num) {

    int half = (num >> 1) + 1;
    vector<bool> primes (half + 1);
    vector<int> sieve {2};

    for (int p = 3; p * p <= num ; p += 2) {
        if (primes[p/2] == false) {
            for (int i = p * p; i <= num; i += 2 * p) {
                primes[i/2] = true;
            }
        }
    }

    for (int i = 3; i <= num; i += 2) {
        if (primes[i/2] == false) {
            sieve.push_back(i);
        }
    }

    return sieve;
}

int tau (int n) { // count number of divisors
    int total = 1;

    for (; (n & 1) == 0; n >>= 1) // Deal with powers of 2 first
        ++total;

    for (int p = 3; p * p <= n; p += 2) { // Odd prime factors up to the square root
        int count = 1;
        for (; n % p == 0; n /= p)
            ++count;
        total *= count;
    }

    if (n > 1) total *= 2; // If n > 1 then it's prime
    return total;
}
int phi (int num) { // totient funtion

    int res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0)
            num /= 2;

        res -= res / 2;
    }

    for (int pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0)
                num /= pr;

            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}
int phi2 (int num, vector<int> &prime) { // totient funtion

    int res = num;
    int *p = prime.data();

    for (int i = 0; p[i] * p[i] <= num; i++) {
        if (num % p[i] == 0) {
            while (num % p[i] == 0)
                num /= p[i];

            res -= res / p[i];
        }
    }

    return (num > 1) ? res - res / num : res;
}
vector<int> phi3 (int lim) { // sieve of totient
    vector<int> sieve (lim + 1);

    for (int i = 0; i <= lim; i++)
        sieve[i] = i;

    for (int i = 2; i <= lim; i++) {
        if (sieve[i] == i) {
            for (int j = i; j <= lim; j += i)
                sieve[j] -= sieve[j] / i;
        }
    }
    return sieve;
}
int sigma (int num) { // sum of proper divisors

  int n = num, sum = 1;
  int p = 2;

  while (p * p <= n && n > 1) {
    if (n % p == 0) {
      int j = p * p;
      n /= p;

      while (n % p == 0) {
        j *= p;
        n /= p;
      }

      sum *= (j - 1) / (p - 1);
    }
    p = (p == 2) ? 3 : p + 2;
  }

  if (n > 1) sum *= (n + 1);

  return sum - num;
}
uint64_t modpow (uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t x = 1, e = 0;

    while (e < exp) {
        e++;
        x = (base * x) % mod;
    }
    return x;
}

int pythagoreantriplet (int sum) {
    const int s2 = sum / 2;
    const int lim = ceil(sqrt(s2)) - 1;
    int cnt = 0;

    for (int m = 2; m <= lim; m++) {
        if (s2 % m == 0) {
            int sm = s2 / m;
            int k = (m % 2 == 1) ? m + 2 : m + 1;

            while (sm % 2 == 0)
                sm /= 2;

            while ((k < (2 * m)) && (k <= sm)) {
                if (sm % k == 0 && gcd (k,m) == 1) {
                    //cout << k << ' ';
                    int d = s2 / (k * m);
                    int n = k - m;

                    int a = d * (m * m - n * n);
                    int b = 2 * d * m * n;
                    int c = d * (m * m + n * n);

                    if (a+b+c == sum) {
                      //printf ("[%i, %i %i] ", a,b,c);
                        cnt++;
                    }
                }
                k += 2;
            }
        }
    }

    return cnt;
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

bool lychrel (uint64_t num) { // check for lychrel number
    uint64_t iter = 24;

    while (iter--> 0) {
        uint64_t rev = reverse (num);

        if (palindrome (num + rev)) {
            //cout << num << " + " << rev << " = " << num + rev << endl;
            return false;
        } else {
            num += rev;
        }
    }

    return true;
}

bool palindrome (int n1, int n2) {
    int d1[10] = {}, d2[10] = {};

    for (int i = 0; i < 4; i++) {
        d1[n1 % 10]++, d2[n2 % 10]++;
        n1 /= 10, n2 /= 10;
    }

    for (int i = 0; i < 10; i++) {
        if (d1[i] != d2[i]) return false;
    }

    return true;
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
int main () {

    Timer chrono;

    const uint64_t limit = 1e15;
    int a2 = 1004064;

    const char alpha[3] = {'D','U','d'};
    const string ref = "UDDDUdddDDUDDddDdDddDDUDDdUUDd";
    //U D  D  D   U   d
    // vector<int> ve {1,4,13,40,202,202,931,5305};

    /*
    string sub = ref;
    vector<uint64_t> sq1;
    uint64_t step = 1;
    for (uint64_t i = 0; i < 23 ; i++) {
        step *= 3;
    }
    uint64_t start = 24974353390;
    for (uint64_t i = start; i < 1e16 ; i += step){
        uint64_t a1 = i;
        string seq;
        //cout << a1 << " => ";
        while (a1 > 1) {
            int mod = a1 % 3;

            switch (mod) {
              case 0 : a1 = a1 / 3; break;
              case 1 : a1 = (4 * a1 + 2) / 3; break;
              case 2 : a1 = (2 * a1 - 1) / 3; break;
              default: break;
            }
            seq += alpha[mod];
            //cout << alpha[mod];
        }

        if (seq.substr(0, ref.size()) == ref && i > 1e15) {
            //cout << i << " => " << seq << '\n';
            break;
        }
        //cout << endl;
        //cout << i % 3 << ' ';
    }
    /*
    */

    chrono.stop();
    chrono.get_duration();
}
