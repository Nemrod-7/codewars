#include "base.hpp"
#include "ntheory.hpp"

#include <thread>
#include <atomic>
// NEQ5xBztxeg43aP
using namespace std;

// -std=c++17 -Wall -Wextra -O0 -pthread

void search (const vector<uint32_t> &prime, int start, int end, int &res) {
    const int limit = prime.back() + 2;
    const int size = prime.size();
    cout << limit << " ";
    for (int i = start; i < end; i++) {
        uint32_t a = prime[i] + 1;

        for (int j = i + 1; j < size; j++) {
            uint32_t b = prime[j] + 1;
            double c = b * b / (double)a;

            if (c >= limit) break;

            for (int k = j + 1; k < size && prime[k] < c ; k++) {
                if (prime[k] == c-1) {
                    res += a + b + prime[k] - 2;
                    // cout << "(" << a -1 << " " << b - 1 << " " << prime[k] << ")" ;
                }
            }
        }
        // cout << "\n";
    }
}


int main () {

    Timer chrono;

    // Problem 123 Prime square remainder
    // problem 120 Square Remainder
    // problem510 (100);

    // problem 518
    int64_t limit = 1e2;
    int nc = 4;
    // (2, 5, 11), (2, 11, 47), (5, 11, 23), (5, 17, 53), (7, 11, 17), (7, 23, 71), (11, 23, 47), (17, 23, 31), (17, 41, 97), (31, 47, 71), (71, 83, 97) => 1035
    const auto prime = sieve (limit);
    const int mem = prime.size() / nc;
    int sum = 0;
    vector<int> res (nc);
    // vector<thread> th (nc);

    for (int i = 0; i < nc; i++) {
        int low = i * mem, high = low + mem ;
        cout << low << " " << high << "\n";
        search (prime, low, high, res[i]);
        sum += res[i];
        cout <<  '\n';
    }

    cout << " => " << sum;
    //showvec(prime);


    chrono.stop();
    chrono.get_duration();
}
