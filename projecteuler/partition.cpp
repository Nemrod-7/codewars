#include <iostream>
#include <vector>

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

int main () {

    int size = 7;

        int tlsz = 3;
        int ntile = size / tlsz;

        for (int i = 3; i < 7; i++) {

        }
        //for (int i = 0; i < size; i++) {

        int nd = size - tlsz;
        for (int j = 0; j < nd; j++) {
          std::vector<int> v(size);

            for (int k = 0; k <= tlsz; k++) {
                v[j+k] = 1;
            }

            for (int k = 0; k < size; k++) {
            //    cout << v[k];
            }
            cout << endl;
        }
    //}

    /*
    1
    1111111
    2
    1111110
    0111111
    3
    1111100
    0111110
    0011111
    4
    1111000
    0111100
    0011110
    0001111
    6
    1110000
    0111000
    0011100
    0001110
    0000111

    1110111

    */

    cout << "\nend\n";
}
