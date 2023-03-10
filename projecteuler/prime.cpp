#include <iostream>
#include <vector>
#include <cmath>

#include <algorithm>

using namespace std;

bool is_prime (int num) {

    if (num <= 3) return true;
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

int main () {

/*
Problem 51

By replacing the 1st digit of the 2-digit number *3, it turns out that six of the nine possible values: 13, 23, 43, 53, 73, and 83, are all prime.

By replacing the 3rd and 4th digits of 56**3 with the same digit,
this 5-digit number is the first example having seven primes among the ten generated numbers,
yielding the family: 56003, 56113, 56333, 56443, 56663, 56773, and 56993. Consequently 56003,
being the first member of this family, is the smallest prime with this property.

Find the smallest prime which, by replacing part of the number (not necessarily adjacent digits) with the same digit, is part of an eight prime value family.
*/
  int limit = 100000;
  std::vector<bool> prime (limit, true);

  for (int i = 4; i <= limit ; i += 2) {
      prime[i] = false;
  }

  for (int p = 3; p * p <= limit ; p += 2) {
      if (prime[p] == true) {
          for (int i = p * p; i <= limit; i += 2 * p) {
              prime[i] = false;
          }
      }
  }

  for (int i = 56000; i < 56004; i++) {
      string num = to_string(i);

      for (int j = 0; j < 10; j++) {
          char dig = j + '0';
          num[2] = num[3] = dig;

          int dec = stoi(num);
          if (prime[dec] == true) {
            cout << dec << ' ';

          }
      }
      cout << "\n";
  }


    /*
    // problem 74

    int num = 1, cnt = 0;
    double rate = 0.0, limit = 99.0;

    int bn = 0, nb = 0;
    string googol = "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    vector<int> hist;

    for (int i = 1; i < 100000000; i++) {
        if (bouncy (i)) {
            bn++;
        } else {
            nb++;
        }

        if (i % 1000000 == 0) {
          hist.push_back(nb);
          //cout << i << " => " << bn << " " << nb << "\n";
        }
    }

    for (int i = 1; i < hist.size(); i++) {
        cout << hist[i] - hist[i-1] << ' ';
    }


    /*
    while (rate < limit) {

        for (; i <= num; i++) {
            cnt += bouncy (i);
        }
        rate = cnt * 100.0 / num;
        num++;
    }


    cout << rate << " :: " << num - 1;
    */


    std::cout << "\nend\n";
}
