#include <iostream>
#include <vector>
#include <numeric>

using namespace std;
bool valid (int i, int j, int k) {
  return (i != j && i != k && j != k);
}
vector<vector<int>> generate (int size) {
    vector<vector<int>> comb;
    int maxx = size * 2 + 1;
    for (int i = 1; i <= maxx; i++) {
        for (int j = 1; j <= maxx; j++) {
            for (int k = 1; k <= maxx; k++) {
                if (i != j && i != k && j != k) {
                  comb.push_back({i,j,k});
                }
            }
        }
    }

    return comb;
}

void showvec (const vector<int> &ve) {
    cout << "[ ";
    for (auto dig : ve) {
        cout << dig << ' ';
    }
    cout << "]\n";
}
void show (vector<vector<int>> a) {
  for (int i = 0; i < a.size(); i++) {
    cout << "[";
    for (int j = 0; j < 3; j++) {
      cout << a[i][j] ;
    }
    cout << "]";
  }
  cout << "\n";
}
bool outer (vector<vector<int>> a) {

    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < 3; j++) {

        }
    }

    return true;
}
int main () {

  // problem 68 5-gon
  /*
        4
        \
        3
       | \
      1---2---6
     |
    5

    4,3,2; 6,2,1; 5,1,3

    i = 1; j = 2; k = 3;

    inner :
    a[0][1] = k, a[0][2] = j;
    a[1][1] = j, a[1][2] = i;
    a[2][1] = i, a[2][2] = k;
    */

    int ref = 9, size = 3;
    int maxx = size * 2 + 1;
    vector<vector<int>> comb = generate(3);

    vector<vector<int>> a (size, vector<int> (3));

    for (int i = 0; i < comb.size(); i++) {
        int dig[10] = {0};

        a[0][1] = a[2][2] = comb[i][0];
        a[0][2] = a[1][1] = comb[i][1];
        a[1][2] = a[2][1] = comb[i][2];

        for (int k = 0; k < 3; k++) {
            dig[comb[i][k]]++;
        }

        for (int j = 1; j < maxx; j++) {
            if (dig[j]) continue;
          /*
            for (int k = 0; k < 3; k++) {
                if (j + a[k][1] + a[k][2] != ref) continue;
                if (valid (j, a[k][1], a[k][2])) a[k][0] = j;
            }
            */
        }
    }


}

void arkive () {

      int ref = 9, size = 3;
      int maxx = size * 2 + 1;
      string dig;
      for (int i = 1; i <= size * 2; i++) {
           dig += static_cast<char> (i + '0');
      }
      vector<vector<int>> ring (size, vector<int> (3));

      /*
      for (int i = 1; i < maxx; i++) {
          for (int j = 1; j < maxx; j++) {
              for (int k = 1; k < maxx; k++) {
                  int num = i + j + k;
                  if (num == ref) {
                    cout << i << j << k << "\n";
                    //comb.push_back(to_string (i) + to_string (j) + to_string (k));
                  }
              }
          }
      }

      vector<string> comb;
      for (int i = 1; i < maxx; i++) {
          for (int j = i + 1; j < maxx; j++) {
              for (int k = j + 1; k < maxx; k++) {
                  int num = i + j + k;
                  if (num == ref) {
                    //string clust = to_string (i) + to_string (j) + to_string (k);
                    comb.push_back(to_string (i) + to_string (j) + to_string (k));
                  }
              }
          }
      }


      /*
      for (int i = 0; i < comb.size(); i++) {

          do {
              ref = comb[i][0];
              bool found = false;

              for (int j = 0; j < comb.size(); j++) {
                  if (j != i) {
                      if (comb[j].find (ref) != string::npos) {
                          found = true;
                          break;
                      }
                  }
              }
              if (found == false) break;

          } while (next_permutation (comb[i].begin(), comb[i].end()));


          cout << comb[i] << '\n';

      }
      */
}
