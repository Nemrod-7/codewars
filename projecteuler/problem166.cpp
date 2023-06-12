#include <iostream>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

void showgrid (const int grid[]) {

  for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
          cout << grid[i * 4 + j];
      }
      cout << "\n";
  }
  cout << "\n";
}
bool is_valid (const int grid[], int ref) {
    const int size = 4;
    int diag = 0, aiag = 0;

    for (int i = 0; i < size; i++) {
        diag += grid[i * size + i];
        aiag += grid[i * size + (size-i-1)];
    }

    if (diag != ref || aiag != ref) return false;

    return true;
}
bool partial  (const int grid[], int y, int sum) {
  int size = 4, diag = 0, col;

  for (int i = 0; i <= y; i++) {
      diag += grid[i * size + i];
      if (diag > sum) return false;
  }
  for (int x = 0; x < size; x++) {
      col = 0;

      for (int i = 0; i <= y; i++) {
          col += grid[i * size + x];
      }
      if (col > sum) return false;
  }

  return true;
}

bool backtrack (int grid[], const vector<int> &base, int y, int sum) {
  const int size = 4;

  if (y == size) {
      if (is_valid (grid,sum)) {
          showgrid (grid);
          return true;
      } else {
          return false;
      }
  }

  for (int num : base) {
      int  x = 0;

      do {
          grid[y * size + x++] = num % 10;
      } while (num /= 10);

      if (partial (grid, y, sum) == true) {
          backtrack (grid, base, y + 1, sum);
      }
  }

  return false;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;
    // Problem 166 Criss Cross

    const int size = 4;
    int lim = 8;
    map<int,vector<int>> base;

    string board (size * size,0);
    int grid[size * size];

    for (int i = 0; i < 10000; i++) {
        int num = i, sum = 0;

        do {
            sum += num % 10;
        } while (num /= 10);

        base[sum].push_back(i);
    }

    for (auto num : base[lim]) {
        cout << num << " ";
    }
    for (auto [num, comb] : base) {
        // cout << num << " => " << comb.size() << "\n";
    }

    // backtrack (grid, base[lim],0, lim);


    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;

}
