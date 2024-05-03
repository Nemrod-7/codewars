
#include <iostream>
#include <vector>
#include <set>

using namespace std;

void array(int n) {
    int it = 2;
    vector<int> arr(n);

    arr[0] = it, arr[n-1] = n-it-1;

    for (int i = 0; i < n; i++) {
        if (arr[i] == 0 && i != it) {
            cout << 'x' << ' ';
        } else {
            cout << arr[i] << ' ';
        }
    }
}
void latinsquare(int n) {

      for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
              int cell = i^j;
              printf("%2i ", cell);
          }
          printf("\n");
      }
  printf("\n");
}


int main () {

    int n = 4, fs = 0;
    vector<pair<int,int>> pairs;
    vector<vector<int>> cube (n, vector<int>(n));
    vector<vector<int>> graph (n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        cube[i][0] = i, cube[0][i] = i;
        cube[i][n-1] = n - i - 1, cube[n-1][i] = n - i - 1;
        cube[i][n-i-1] = n - 1;
    }

    for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                pairs.push_back({i,j});
        }
    }

    // latinsquare (4);

    for (int k = 1; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int cell = cube[i][j];
                int ak = (k * i + j) % n;
                printf("%2i ", ak);
                // if (cell == 0 && i != j) {
                //     printf (" x ");
                // } else {
                //     printf("%2i ", cube[i][j]);
                // }
            }
            printf("\n");
        }

        printf("\n");
    }
}
