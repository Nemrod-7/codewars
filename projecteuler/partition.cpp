#include <iostream>
#include <vector>

using namespace std;

vector<int> sieve (int num) {

    int half = (num >> 1) + 1;
    vector<bool> primes (half + 1);
    vector<int> sieve {2};

    for (int p = 3; p * p <= num ; p += 2) {
        if (primes[p/2] == false) {
            for (int i = p * p; i <= num; i += 2 * p) {
                primes[i/2] = true;
            }
        }
    }

    for (int i = 3; i <= num; i += 2) {
        if (primes[i/2] == false) {
            sieve.push_back(i);
        }
    }

    return sieve;
}

int partition (int num) {

    vector<int> cluster (num), prod;
    int index = 0, digit, rem = 0 , stack = 0, next = 0;
    int cnt = 0;
    cluster[index] = num + 1;

    while (cluster[0] > 1) {

        digit = cluster[index] - 1;
        rem = stack, next = index;

        while (rem < num)
            if (num - rem >= digit) {
                if (digit > 1) {
                    index = next;
                    stack = rem;
                }
                rem += cluster[next++] = digit;

            } else {
                digit--;
            }
            /*
            for (int i = 0; i < next; i++) {
                cout << cluster[i] << " ";
            }
            cout << endl;
            */
        if (cluster[index] == 1) {
            index--;
            stack -= cluster[index];
        }
        cnt++;
    }

    return cnt;
}
uint64_t countpart (int n) {

    vector<uint64_t> part(n+1);
    part[0] = 1;

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = i; j <= n; ++j) {
            part[j] += part[j - i];
        }
    }

    return part[n];
}



int coinsum (int n, const vector<int> &coin) {

    vector<int> part(n + 1);
    part[0] = 1;

    for (int i = 0; i < coin.size(); i++) {
        int c = coin[i];

        for (size_t j = c; j <= n; ++j) {
            part[j] += part[j - c];
        }
    }

    return part[n];
}

int main () {

    const int n = 1e6;
    vector<int> prime = sieve (10000);

    for (auto nu : prime) {
        cout << nu << ' ';
    }

    for (int i = 1; i < 1000; i++) {

    }

    cout << "end";
}
