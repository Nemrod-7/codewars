#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>

#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}

vector<int> sieve3 (int num) {

    int half = (num >> 1) + 1;
    vector<int> prime {2};
    vector<bool> sieve (half);

    for (int p = 3; p * p <= num ; p += 2) {
        if (sieve[p/2] == false) {
            for (int i = p * p; i <= num; i += 2 * p) {
                sieve[i/2] = true;
            }
        }
    }

    for (int i = 3; i <= num; i += 2) {
        if (sieve[i/2] == false) {
            prime.push_back(i);
        }
    }

    return prime;
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
bool check_sum (int num, vector<int> &vec) {

    for (int j = 0; vec[j] < num; j++) {
        int left = num - vec[j];
        //cout << left << " ";
        for (int k = j + 1; vec[k] <= left && left > 0 ; k++) {
            if (left - vec[k] == 0) {
              cout << num << " = " << vec[j] << "+" << vec[k] << endl;
              return true;
            }
        }
    }
    return false;
}

bool ispalindrome (int num) {

    if (num % 10 == 0) return false;
    int ref = num, rev = 0;

    do {
        rev = rev * 10 + (num % 10);
    } while (num /= 10);

    return ref == rev;
}

void test () {
    /*
    Assert::That(values(100), Equals(3));
    Assert::That(values(200),Equals(4));
    Assert::That(values(300),Equals(4));
    Assert::That(values(400),Equals(5));
    Assert::That(values(1000), Equals(11));
    */
}

int sqsum (int lim) {

    int cnt = 0;

    for (int i = 1; i < 500; i++) {
        int sum = i * i;
        //cout << i << " => ";
        for (int j = i + 1; sum < lim; j++) {
            sum += (j * j);

            if (sum < lim && ispalindrome (sum)) {
                cnt++;
                cout << sum << " ";
            }
            //cout << sum << " ";
        }
        //cout << endl;
    }

    cout << " :: "<<  cnt;
    return cnt;
}

int Ackermann(int m, int n) {
    if (n == -1) return n;
    if (m == 0) return n + 1;
    if (n == 0) return Ackermann (m - 1, 1);
    return Ackermann (m - 1, Ackermann (m, n - 1));
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    const int lim = 1e4;
    const int sq = sqrt(lim);
    int cnt = 0;
    set<int> res;

    for (int i = 1; i < lim; i++) {
  			int fn = 2 * (i * i) - 1;

        if (is_prime (fn)) {
            cnt++;
        }
  			//std::cout << fn  <<  " ";
  	}
    cout << " :: "<<  cnt;



    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
