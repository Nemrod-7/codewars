#include <numeric>
#include <algorithm>
#include <numbers>
#include <cmath>

#include "base.hpp"
#include "ntheory.hpp"

using namespace std;
// stackoverflow d~a6fK~*>Z$~2Zy
// projecteuler  NEQ5xBztxeg43aP

// -std=c++20 -Wall -Wextra -O2 -pthread -march=native

int main () {

    Timer chrono;
    // constexpr auto π = std::numbers::pi;
    /*
       problem 745 - Sum of Squares II : A008833 Largest square dividing n

       For a positive integer, n, define g(n) to be the maximum perfect square that divides n.
       For example, g(18) = 9, g(19) = 1.

       Also define S(N) = \sum_{n=1}^N g(n)
       For example, S(10) = 24 and S(100) = 767.

       Find S(10^{14}). Give your answer modulo 1000000007

        limit :         1 1
        limit :        10 24
        limit :       100 767
        limit :     1 000 22606
        limit :    10 000 722592
        limit :   100 000 22910120
        limit : 1 000 000 725086120
        limit :10 000 000 22910324448

        Execution time :  6.267060 ms


 1 24 767 22606 722592 22910120 725086120 22910324448
*/
//cout << "limit :" << std::setw(11) << format (limit);
//cout << S(limit);

    int limit = 100;
    i64 sum = 0;
    // auto mu = moebius (limit);
    auto prime = sieve(floor(sqrt(limit) + 1));

    for (i64 i = 1; i <= limit; i++) {
        i64 maxs = 1, sq = 4;
        cout << setw(2) << i << " :: ";

        for (i64 j = 2; sq <= i; j++) {
            if (i % sq == 0) {
                // cout << sq << ' ';
                maxs = sq;
            }
            sq = j * j;
        }

        // cout << i % sq;
        cout << maxs;
        cout << endl;
        sum += maxs % mod;
    }

    cout << " => " << sum;

    chrono.stop();
    chrono.get_duration();
}
