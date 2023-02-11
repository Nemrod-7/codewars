#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <cmath>
#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

vector<uint64_t> sieve (uint64_t num) {

    uint64_t half = (num >> 1) + 1;
    bool *primes = new bool[num + 1];
    vector<uint64_t> sieve {2};

    fill_n (primes, num + 1, true);

    for (uint64_t p = 3; p * p <= num ; p += 2) {
        if (primes[p] == true) {
            for (uint64_t i = p * p; i <= num; i += 2 * p) {
                primes[i] = false;
            }
        }
    }

    for (uint64_t i = 3; i <= num; i += 2) {
        if (primes[i] == true) {
            sieve.push_back(i);
        }
    }

    delete[] primes;
    return sieve;
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
int divsum (int num) {
    int sum = 0;
    for (int i = 1; i < num ; i++) {
        if (num % i == 0) {
            sum += i;
        }
    }
    return sum;
}
int main () {

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t lim = 1e2;

    uint64_t num = lim;
    vector<int> ndiv (lim);

    for (uint64_t p = 2; p <= num ; p++) {
        if (ndiv[p] == 0) {
            for (uint64_t i = p; i <= num; i += p) {
                ndiv[i]++;
            }
        }
    }

    /*

    uint64_t fib[1024] = {1,1};

    for (int i = 2; i < 50; i++) {
        fib[i] = fib[i-1] + fib[i-2];
        cout << i << " => " << fib[i] << "\n";
    }
    */

    int sum = 0;
    for (int i = 220; i < 10000; i++) {
        int rv = divsum (i);

        if (rv != i) {
          int rd = divsum (rv);
          if (rd == i && i != rv) {
            sum += (i + rv);
            //cout << i << " " << rv << endl;
          }

        }

    }
    cout << sum / 2;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
