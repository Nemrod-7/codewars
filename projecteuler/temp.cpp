#include <iostream>
#include <vector>
#include <deque>

#include <chrono>

using namespace std;

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    // Problem 149 : Searching for a maximum-sum subsequence
    // Finally, find the greatest sum of (any number of) adjacent entries in any direction (horizontal, vertical, diagonal or anti-diagonal).
    
    const int64_t mod = 1000000;

    vector<int64_t> seed(4000000);
    int64_t *rng = seed.data();
    vector<vector<int64_t>> board (2000,vector<int64_t>(2000));

    for (int64_t k = 0; k < 56; k++) {
        rng[k] = (100003 - 200003 * k + 300007 * k*k*k) % mod - 500000;
    }

    for (int64_t k = 56; k < 4000000; k++) {
        rng[k] = (rng[k-24] + rng[k-55] + mod) % mod - 500000;
    }

    for (int64_t i = 0; i < 4000000; i++) {
        board[i/2000][i%2000] = rng[i];
    }

    // cout << 10 << " => " << rng[10] << "\n";
    // cout << 100 << " => " << rng[100] << "\n";

      // rn = seed[(k-24) % 56] + seed[(k-55) % 56];
      // cout << rn << " ";

    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
