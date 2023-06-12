#include <iostream>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

const int N = 4;

void showgrid (const int grid[]) {

  for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
          cout << grid[i * N + j];
      }
      cout << "\n";
  }
  cout << "\n";
}
bool is_valid (const int grid[], int sum) {

    int diag = 0, aiag = 0;
    for (int i = 0; i < N; i++) {
        diag += grid[i * N + i];
        aiag += grid[i * N + (N-i-1)];
    }

    if (diag != sum || aiag != sum) return false;

    for (int y = 0; y < N; y++) {
        int row = 0, col = 0;
        for (int x = 0; x < N; x++) {
            row += grid[y * N + x];
        }
        if (row != sum) return false;
    }


    return true;
}



bool partial  (const int grid[], int y, int sum) {
  int diag = 0, col;

  for (int i = 0; i <= y; i++) {
      diag += grid[i * N + i];
      if (diag > sum) return false;
  }
  for (int x = 0; x < N; x++) {
      col = 0;

      for (int i = 0; i <= y; i++) {
          col += grid[i * N + x];
      }
      if (col > sum) return false;
  }

  return true;
}

bool backtrack (int grid[], const vector<int> &base, int y, int sum, int &cnt) {

  if (y == N) {
      if (grid[0] + grid[5] + grid[10] + grid[15] != sum) return false;
      if (grid[3] + grid[6] + grid[9] + grid[12] != sum) return false;
      // if (is_valid (grid,sum)) {
      // } else { return false; }

      // showgrid (grid);
      cnt++;
      return true;
  }

  for (int num : base) {
      int  x = 0;

      do {
          // int dig = num % 10;
          grid[y * N + x] = num % 10;
          x++;
      } while (num /= 10);

      if (partial (grid, y, sum) == true) {
          backtrack (grid, base, y + 1, sum, cnt);
      }
  }

  return false;
}

int limit (int grid[], int x, int y, int sum) {
    int col = sum, row = sum;

    for (int i = 0; i < y; i++)
        col -= grid[i * N + x];

    for (int i = 0; i < x; i++)
        row -= grid[y * N + i];

    return min(min(col, row), 9);
}

bool checkline (int grid[], int y, int sum) {
    int row = 0;

    for (int x = 0; x < N; x++) {
        row += grid[y * N + x];
    }
    return row == sum;
}
bool backtrack2 (int grid[], int x, int y, int sum, int &cnt) {

    if (x == N) {
        if (checkline (grid,y,sum) == false) {
            return false;
        }
        x = 0;
        y++;
    }

    if (y == N) {
        if (is_valid(grid, sum)) {
            cnt++;
            return true;
        }
        return false;
    }
    int nd = limit (grid,x,y,sum);
    // cout << nd << ' ';
    for (int dig = nd; dig >= 0 ; dig--) {
        grid[y * N + x] = dig;
        backtrack2 (grid, x+1, y, sum, cnt);
    }

    return false;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;
    // Problem 166 Criss Cross

    int grid[N * N] = {0};
    int sum = 6, cnt = 0;


    for (int i = 0; i < 4;i++){
      int cnt = 0;
      cout << i << " => " ;
      backtrack2 (grid,0,0,i,cnt);
      cout << cnt << "\n";
    }
    // cout << cnt;
    // showgrid(grid);
    /*

    6 3 3 0
    5 0 4 3
    0 7 1 4
    1 2 4 5

    0 =>   1 ::    0
    1 =>   4 ::    0
    2 =>  10 ::    0
    3 =>  20 ::    0
    4 =>  35 ::   33
    5 =>  56 ::  228
    6 =>  84 ::  902
    7 => 120 :: 2736
    8 => 165 :: 6955
    9 => 220 :: 15680
    10 => 282 :: 32212
    11 => 348 :: 61367
    12 => 415 :: 108276


    ...................


    27 => 220 :: 42248
    28 => 165 :: 21925
    29 => 120 :: 10608
    30 =>  84 :: 4736
    31 =>  56 :: 1904
    32 =>  35 :: 675
    33 =>  20 :: 200
    34 =>  10 :: 48
    35 =>   4 :: 8
    36 =>   1 :: 1


    vector<int> seq {0,0,0,0,33,228,902,2736,6955,15680,32212,61367,108276};

    */
    /*
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


    for (int num : base[1]) {
      cout << num << " ";
    }
    */

    // auto comb = base[i];
    // cout << comb.size() << " :: ";
    // backtrack (grid, base[i], 0, i, cnt);

    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
