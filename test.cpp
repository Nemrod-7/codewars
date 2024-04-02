#include <iostream>
#include <vector>

using namespace std;

int main () {
  vector<vector<int>> grid;

  grid =
    {{0, 0, 6, 1, 0, 0, 0, 0, 8},
      {0, 8, 0, 0, 9, 0, 0, 3, 0},
      {2, 0, 0, 0, 0, 5, 4, 0, 0},
      {4, 0, 0, 0, 0, 1, 8, 0, 0},
      {0, 3, 0, 0, 7, 0, 0, 4, 0},
      {0, 0, 7, 9, 0, 0, 0, 0, 3},
      {0, 0, 8, 4, 0, 0, 0, 0, 6},
      {0, 2, 0, 0, 5, 0, 0, 8, 0},
      {1, 0, 0, 0, 0, 2, 5, 0, 0}};


    int cnt = 0;

    vector<vector<int>> row = vector<vector<int>> (10, vector<int> (10));
    vector<vector<int>> col = vector<vector<int>> (10, vector<int> (10));
    vector<vector<int>> sub = vector<vector<int>> (10, vector<int> (10));

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int z = z = y / 3 * 3 + x / 3;

            row[y][grid[y][x]]++;
            col[x][grid[y][x]]++;
            sub[z][grid[y][x]]++;
        }
    }

    // for (int i = 0; i < 9; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         cout << row[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }


  cout << "\nend\n";
}
