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
bool is_valid (vector<vector<int>> &grid, int ref) {
    int size = grid.size();
    int diag = 0, aiag = 0;

    for (int i = 0; i < size; i++) {
        diag += grid[i][i];
        aiag += grid[size-i-1][size-i-1];
    }
    if (diag != ref || diag != ref) return false;

    for (int i = 0; i < size; i++) {
        int row = 0, col = 0;
        for (int j = 0; j < size; j++) {
            row += grid[i][j], col += grid[j][i];
        }
        if (row != ref || col != ref) return false;
    }

    return true;
}

bool backtrack2 (vector<vector<int>> grid, vector<int> &base, int y) {

  if (y == grid.size()) {
      int sum = 0;
      for (int i = 0; i < grid.size(); i++) {
          sum += grid[i][0];
      }

      if (is_valid(grid, sum) == true) {

          // showgrid(grid);
          return true;
      } else {
          return false;
      }
  }

  for (int num : base) {
      int nn = num, x = 0;

      do {
         grid[x++][y] = nn % 10;
      } while (nn /= 10);

      backtrack2 (grid, base, y + 1);
  }

  return false;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;
    // Problem 166 Criss Cross

    const int size = 3;
    vector<vector<int>> grid (size, vector<int> (size));
    map<int,vector<int>> base;

    for (int i = 0; i < 1000; i++) {
        int num = i, sum = 0;

        do {
            sum += num % 10;
        } while (num /= 10);

        base[sum].push_back(i);
    }

    // backtrack1 (grid, 0, 0, 12);
    backtrack2 (grid,base[12],0);


    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;

}

bool partial (vector<vector<int>> grid, int x, int y, int sum) {

    int row = 0, col = 0;
    for (int i = 0; i <= x; i++) {
        row += grid[i][y];
        if (row > sum) return false;
    }

    for (int i = 0; i <= y; i++) {
        col += grid[x][i];
        if (col > sum) return false;
    }

    return true;
}
bool backtrack1 (vector<vector<int>> grid, int x, int y, int sum) {
    int size =  grid.size();

    if (x == size) {
        int line = 0;
        for (int i = 0; i < size; i++) {
            line += grid[i][y];
        }
        if (line != sum) return false;

        x = 0;
        y++;
    }

    if (y == size) {
        if (is_valid (grid,sum) == true) {
            //cout << x << " " << y << "\n";
            showgrid(grid);
            return true;
        } else {
            return false;
        }
    }

    for (int dig = 0; dig < 10; dig++) {
        grid[x][y] = dig;

        if (partial (grid,x,y,sum) == true) {
            backtrack1 (grid, x + 1 , y, sum);
        }
    }

    return true;
}
