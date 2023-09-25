#include <iostream>
#include <vector>

using namespace std;

int solve (int a, int b) {

    int sum = 0;
    bool *sieve = new bool [b+1]();
		std::vector<int> prime {1,2,3};
		sieve[0] = sieve[1] = true;

    for (int p = 4; p < b; p += 2) {
        sieve[p] = true;
    }

    for (int64_t p = 3; p < b; p += 2) {
		    if (sieve[p] == false) {
						for (int64_t i = p * p; i <= b; i += p) {
								sieve[i] = true;
						}
				}
		}

    for (int64_t i = 5, t = 2; i <= b; i += t, t = 6 - t) {
        if (sieve[i] == false) {
            prime.push_back(i);
        }
    }

    for (int64_t i = 1; i < prime.size(); i++) {
        if ( prime[i] >= a && sieve[i] == false) {
            sum += prime[i];
        }
    }

    delete sieve;
    return sum;
}

int main () {
  int limit = 100;

  cout << 18925907 - solve (15683,60217);

  // cout << solve (1000,100000);

}
