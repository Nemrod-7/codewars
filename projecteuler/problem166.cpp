#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <chrono>

using namespace std;

const int N = 4;
const int pow[N] = {1000,100,10,1};

void showgrid (const int grid[]) {

  for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
          cout << grid[i * N + j] << " ";
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

bool backtrack1 (int grid[], const vector<int> &base, int y, int sum, int &cnt) {

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
      for (int i = 0; i < N; i++) {
        grid[y * N + i] = (num / pow[i]) % 10;
      }

      if (partial (grid, y, sum) == true) {
          backtrack1 (grid, base, y + 1, sum, cnt);
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
    int row = 0, diag = 0;
    for (int i = 0; i < y; i++) {
        diag += grid[i * N + i];
        if (diag > sum) return false;
    }

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
    int col = sum, row = sum;

    for (int i = 0; i < y; i++)
        col -= grid[i * N + x];

    for (int i = 0; i < x; i++)
        row -= grid[y * N + i];

    int nd = min(min(col, row), 9);
    // cout << nd << ' ';
    for (int dig = nd; dig >= 0 ; dig--) {
        grid[y * N + x] = dig;
        backtrack2 (grid, x+1, y, sum, cnt);
    }

    return false;
}
bool backtrack3 (int grid[], int row[], int col[], int x, int y, int sum, int &cnt) {



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

    int maxx = sum - max (row[y],col[x]);
    int lim = min (9, maxx);

        for (int dig = lim; dig >= 0 ; dig--) {
          grid[y * N + x] = dig;
          col[x] += dig;
          row[y] += dig;

          backtrack3 (grid, row, col, x + 1, y, sum, cnt);

          col[x] -= dig;
          row[y] -= dig;
        }
    // cout << max (row[y],col[x]) << ' ';

    return false;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now(), end;
    chrono::duration<double> elapsed;
    // Problem 166 Criss Cross

    int sum = 12, total = 0;
    int grid[N * N] = {0};
    int mask[N * N] = {0};
    int col[N] = {0}, row[N] = {0};
    // vector<int> col(N), row(N);
    // // fill_n(mask, N*N, sum);
    // grid[0] = 6, grid[1] = 3, grid[2] = 3;
    // grid[4] = 5, grid[6] = 4, grid[7] = 3;
    // grid[9] = 7, grid[10] = 1, grid[11] = 4;
    // grid[12]=1, grid[13] = 2, grid[14]=4, grid[15]=5;

    //                 0 1 2   3   4   5    6      7    8    9     10     11     12    13      15     16    17     18      19
    vector<int> seq = {1,8,48,200,675,1904,4736,10608,21925,42328,76976,131320,209127,309968,427440,549184,658457,736744,766736};



    for (int i = 20; i < 21; i++){
      int cnt = 0;
      cout << setw(2) << i << " => " ;
      // backtrack1(grid, base[i], 0, i, cnt);
      //backtrack2 (grid, 0, 0, i, cnt);
      // backtrack3 (grid, row, col, 0, 0, i, cnt);

      total += cnt;
      cout << cnt;
      cout << "\n";
    }


    for (int i = 0; i < seq.size(); i++) {
        total += seq[i] * 2;
        // cout << i << " => " << seq[i] << "\n";
    }
    cout << total;

    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}

void ark() {
  // showgrid(grid);
  /*

  6 3 3 0
  5 0 4 3
  0 7 1 4
  1 2 4 5

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
}
