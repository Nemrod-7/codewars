#include <iostream>
#include <vector>
#include <cmath>

#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

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
vector<int> phi3 (int lim) {
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


void farey (int n) {
    // We know first two terms are 0/1 and 1/n
    double x1 = 0, y1 = 1, x2 = 1, y2 = n;
    printf("%.0f/%.0f %.0f/%.0f", x1, y1, x2, y2);
    double x, y = 0; // For next terms to be evaluated

    while (y != 1.0) {
        // Using recurrence relation to find the next term
        x = floor((y1 + n) / y2) * x2 - x1;
        y = floor((y1 + n) / y2) * y2 - y1;
        // Print next term
        printf(" %.0f/%.0f", x, y);
        // Update x1, y1, x2 and y2 for next iteration
        x1 = x2, x2 = x, y1 = y2, y2 = y;
    }
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    const int lim = 1e3;



    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
