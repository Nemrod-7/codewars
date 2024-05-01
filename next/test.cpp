#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main () {

    int n = 8, g = 2;

    // ['AB', 'CD', 'EF', 'GH']


    int nblock = n / g;
    vector<int> vec;
    vector<vector<int>> nkey;

    for (int i = 2; i < n; i++) {
        vec.push_back(i);
    }

    vec.push_back(1);

    vec.push_back(0);
    for (int i = 0; i < nblock; i++) {
        vector<int> clust;

        for (int j = 0; j < (g / 2); j++) {
            int a = i * g + j, b = n - a - 1;

            clust.push_back(vec[a]);
            clust.push_back(vec[b]);
        }

        sort(clust.begin(), clust.end());
        nkey.push_back(clust);
    }

    vec.pop_back();


    for (int i = 0; i < nkey.size(); i++) {
        for (int j = 0; j < nkey[i].size(); j++) {
            printf("%c", nkey[i][j] + 'A');
        }
        printf(" ");
    }



}
