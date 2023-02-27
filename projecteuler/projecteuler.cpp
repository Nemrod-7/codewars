#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <set>
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

bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
vector<uint64_t> sieve (uint64_t num) {

    uint64_t half = (num >> 1) + 1;
    vector<bool> primes (half + 1);
    vector<uint64_t> sieve {2};

    for (uint64_t p = 3; p * p <= num ; p += 2) {
        if (primes[p/2] == false) {
            for (uint64_t i = p * p; i <= num; i += 2 * p) {
                primes[i/2] = true;
            }
        }
    }

    for (uint64_t i = 3; i <= num; i += 2) {
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

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t lim ;
    uint64_t sum = 1, maxv = 0;
    set<uint64_t> binom;
    uint64_t tri[60][60] = {{1},{1,1}};
    vector<uint64_t> prime;

    for (int n = 0; n < 51; n++) {
        tri[n][0] = tri[n][n] = 1;
        for (int k = 1; k < n; k++) {
            tri[n][k] = tri[n-1][k] + tri[n-1][k-1];
            binom.insert(tri[n][k]);
            //cout << tri[n][k] << " ";
        }
        //cout << endl;
    }

    maxv = *max_element (binom.begin(), binom.end());
    lim = static_cast<uint64_t> (sqrt(maxv));
    prime = sieve(lim);

    for (auto num : binom) {
        bool sqfree = true;

        for (int i = 0; i < prime.size() && prime[i] * prime[i] <= num; i++) {

            if (num % (prime[i] * prime[i]) == 0) {
                sqfree = false;
            }

        }
        if (sqfree == true) {
            sum += num;
        }

    }
    // 34 029 210 557 338
    // 34029210557338
    cout << sum << " ";
    //cout << "\ncount : " << cnt;


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
