#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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

uint64_t pentagonal (int n) { return n * (3 * n - 1) / 2; }

int A (int n, int k) { // count n possibilities out of k ?
    return  n * k * (n + 1) * (k + 1) / 4;
    /*
    int cnt = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            cnt += (n - i + 1) * (k - j + 1);
        }
    }
    return cnt;
    */
}

int main () {

    string str = "0000111";

    do {
      cout << str << "\n";
    } while (next_permutation (str.begin(), str.end()));


    cout << "\nend\n";
}