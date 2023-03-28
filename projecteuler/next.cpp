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

int main (int argc, char *argv[]) {


    int x = 6, y = 5, z = 3;
    vector<int> origin = {0,0,0}, exit = {6,5,3};

    cout << "\nend";

}
