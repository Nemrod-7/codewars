#include <iostream>
#include <vector>

using namespace std;

int main() {

    vector<vector<int>> switches = {{0,1,2},{1,2},{1,2,3,4},{1,4}};

    int N = 5;
    int nsw = switches.size();
    vector<int> light (N);
    vector<int> hist (nsw);

    for (int i = 0; i < nsw; i++) {
        vector<int> next = light;

        for (auto cell : switches[i]) {
            next[cell] ^= 1;
        }

        for (int j = 0; j < N; j++) {
            hist[i] += next[j];
            cout << next[j] << " ";
        }

        cout << " :: " << hist[i] << "\n";
    }

}
