#include <iostream>
#include <vector>

using namespace std;

int main () {

    int64_t lim = 100000000;
    //lim = 10000;
    // problem 357 Prime Generating Integers
    int64_t sum = 1;
    std::vector<bool> sieve (lim + 1, true);
    std::vector<int64_t> prime {2};

    for (int64_t p = 4; p <= lim; p += 2) {
        sieve[p] = false;
    }

    for (int64_t p = 3; p * p <= lim ; p += 2) {
        if (sieve[p] == true) {
            for (int64_t i = p * p; i <= lim; i += 2 * p) {
                sieve[i] = false;
            }
        }
    }


    for (int64_t i = 3; i <= lim; i += 2) {
        if (sieve[i] == true) {
            bool valid = true;
            int num = i - 1;

            for (int64_t j = 2; j * j <= num; j++) {
                if (num % j == 0) {
                    if (sieve[j + num / j] == false) {
                        valid = false;
                        //cout << j << " ";
                        break;
                    }
                }
            }

            if (valid == true) {
                sum += num;
                //cout << num << " ";
            }
        }
    }

    cout << sum;

    cout << "\n";

}
