#include <iostream>
#include <vector>
#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;

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
        while (num % 2 == 0) {
            num /= 2;
        }
        res -= res / 2;
    }

    for (int pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0) {
                num /= pr;
            }
            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    const int lim = 1e6;

    int index = 0;
    double maxv = 0.0;
    vector<int> prim = sieve3 (lim);





    cout << index;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
