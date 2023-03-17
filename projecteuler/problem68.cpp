#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
vector<vector<int>> generate (int ref) {
    vector<vector<int>> ring;

    for (int i = 1; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            for (int k = j + 1; k < 10; k++) {
                int num = i + j + k;
                if (num == ref) {
                  vector<int> clust {i,j,k};
                  ring.push_back(clust);
                  //std::cout << i << " " << j << " " << k << '\n';
                }
            }
        }
    }
    return ring;
}

void showvec (const vector<int> &ve) {
    cout << "[ ";
    for (auto dig : ve) {
        cout << dig << ' ';
    }
    cout << "]\n";
}
int main () {

  // problem 68 5-gon

    int dig[10] = {1,2,3,4,5,6,7,8,9};
    // 4,3,2; 6,2,1; 5,1,3
    int ref = 9;
    auto ring = generate(9);

    for (int i = 1; i < 20; i++) {
      /*
        cout << i << " => ";
        cout << ring.size() << "\n";
        */
    }
    // {{1,2,6},{1,3,5},{2,3,4}};

    /*
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j < 3; j++) {
            int dig = ring[i][j];

            for (int k = 0; k < 3; k++) {
                if (k != i) {
                    if (find(ring[k].begin() , ring[k].end(), dig) == ring[k].end()) {
                        //cout << dig << ' ';
                        swap (ring[i][0], ring[i][j]);
                        //showvec(ring[i]);
                        break;
                    }
                }
            }

        }
    }
    cout << "\n";

    for (int i = 0; i < 3; i++) {
        cout << "[ ";
        for (int j = 0; j < 3; j++) {
          cout << ring[i][j] << ' ';
        }
        cout << "]\n";
    }
    */
    
    /*
    */
}
