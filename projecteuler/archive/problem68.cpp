#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

bool valid (int i, int j, int k) {
  return (i != j && i != k && j != k);
}
void showvec (const vector<int> &ve) {
    cout << "[ ";
    for (auto dig : ve) {
        cout << dig << ' ';
    }
    cout << "]\n";
}
void show (const vector<vector<int>> &a) {
  for (int i = 0; i < a.size(); i++) {
    cout << "[";
    for (int j = 0; j < 3; j++) {
      cout << a[i][j] ;
    }
    cout << "]";
  }
  cout << "\n";
}

std::vector<std::vector<int>> generate (int depth, int size) { // make unique combinations of n choose k

    std::vector<bool> mask (size);
    std::vector<std::vector<int>> combs;

    std::fill (mask.end() - depth, mask.end(), true);

    do {
        std::vector<int> clust;

        for (int i = 0; i < size; ++i) {
            if (mask[i]) {
                clust.push_back(i+1);
            }
        }
        combs.push_back (clust);

    } while (std::next_permutation (mask.begin(), mask.end()));

    return combs;
}

void ring (int total) {
  const int size = 5;
  const int maxx = size * 2, minv = 99;
  vector<vector<int>> comb = generate (size, maxx);
  vector<string> res;

  for (int i = 0; i < comb.size(); i++) {

      do {
        vector<vector<int>> a (size, vector<int> (3));
        int hist[12] = {0};
        bool valid = true;
        int minv = 99;

        for (int k = 0; k < size; k++) {
            const int nx = k == (size-1) ? 0 : k+1;
            a[k][2] = a[nx][1] = comb[i][k];
            hist[comb[i][k]]++;
        }

        for (int k = 0; k < size; k++) {
            const int dig = total - accumulate (a[k].begin(), a[k].end(), 0);

            if (dig <= 0 || dig > maxx || hist[dig]) {
                break;
            }
            hist[dig]++;
            a[k][0] = dig;
        }
        //cout << minv << ' ';
        for (int k = 0; k < size; k++) {
            minv = min (minv, a[k][0]);
        }

        if (a[0][0] && a[0][0] == minv) {
            cout << "[";
            for (int k = 0; k < size; k++) {
                for (int j = 0; j < 3; j++) {
                    cout << a[k][j];
                }
            }
            cout << "]\n";
        }

      } while (next_permutation (comb[i].begin(), comb[i].end()));
  }

}

int main () {

  /*
  // problem 68 5-gon

        4
        \
        3
       | \
      1---2---6
     |
    5

       0,1,2; 0,2,3; 0,3,4; 0,4,5; 0,5,1

                 a[0][1] = 5;
       a[0][2] = a[1][1] = 1;
       a[1][2] = a[2][1] = 2;
       a[2][2] = a[3][1] = 3;
       a[3][2] = a[4][1] = 4;
       a[4][2] = 5;

    */

    int total = 14, size = 5;
    for (int i = 9; i < 19; i++) {
        cout << i << " => \n";
        ring (i);
        //ring (i);
    }

    cout << "\nend\n";

}
