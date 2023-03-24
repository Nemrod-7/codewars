#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int A (int n, int k) {
    return  n * k * (n + 1) * (k + 1) / 4;

    int cnt = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            cnt += (n - i + 1) * (k - j + 1);
        }
    }
    return cnt;
}

int main () {

    const int limit = 2000000;
    const int limx = 20, limy = 20;

    const double num = 2;
    const double sq = sqrt (num);

    int ip = floor(sq);

    cout << sq;

    cout << "\nend";

}
