#include <iostream>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

void showgrid (vector<vector<int>> &grid) {

  for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid.size(); j++) {
          cout << grid[i][j];
      }
      cout << "\n";
  }
  cout << "\n";
}
bool is_valid (const vector<vector<int>> &grid, int ref) {
    const int size = grid.size();
    int diag = 0, aiag = 0;

    for (int i = 0; i < size; i++) {
        diag += grid[i][i];
        aiag += grid[i][size-i-1];
    }

    if (diag != ref || aiag != ref) return false;

    for (int i = 0; i < size; i++) {
        int row = 0, col = 0;
        for (int j = 0; j < size; j++) {
            row += grid[i][j], col += grid[j][i];
        }
        if (row != ref || col != ref) return false;
    }

    return true;
}
bool partial2 (vector<vector<int>> &grid, int y, int sum) {

    int diag = 0;
    for (int i = 0; i <= y; i++) {
        diag += grid[i][i];
        if (diag > sum) return false;
    }

    return true;
}
bool backtrack2 (vector<vector<int>> &grid, const vector<int> &base, int y, int sum) {

  if (y == grid.size()) {
      if (is_valid(grid, sum) == true) {
          showgrid(grid);
          return true;
      } else {
          return false;
      }
  }

  for (int comb : base) {
      int num = comb, x = 0;

      do {
         grid[x++][y] = num % 10;
      } while (num /= 10);

      if (partial2 (grid, y, sum) == true)

          backtrack2 (grid, base, y + 1, sum);

  }

  return false;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;
    // Problem 166 Criss Cross

    const int size = 4;
    int ss = 6;
    vector<vector<int>> grid (size, vector<int> (size));
    map<int,vector<int>> base;


    for (int i = 0; i < 10000; i++) {
        int num = i, sum = 0;

        do {
            sum += num % 10;
        } while (num /= 10);

        base[sum].push_back(i);
    }

    for (auto [num, comb] : base) {
        // cout << num << " => " << comb.size() << "\n";
    }

    backtrack2 (grid,base[ss],0, ss);

    /*
    */


    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;

}
