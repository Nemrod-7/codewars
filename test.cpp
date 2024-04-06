#include <iostream>
#include <vector>
#include <random>

using namespace std;

string display (vector<int> board) {

  string os;

  for (int y = 0; y < 81; y++) {
      if (y != 0) {
        if (y % (9 * 3) == 0) os += '\n';
        if (y % 9 == 0) os += '\n';
      }

      os += (board[y] ? to_string (board[y]) : ".") + " ";
      if (y % 3 == 2) os += ' ';
  }
  os += "\n-------------------\n";

  return os;
}
vector<int> convert (vector<vector<int>> input) {
    vector<int> board;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            board.push_back (input[y][x]);
        }
    }
    return board;
}

class Sudoku {
private:
    vector<int> grid;
    vector<vector<int>> row, col, sub;
    vector<pair<vector<int>,int>> tape;

public:

  Sudoku (vector<int> src) {
      row.resize (10, vector<int>(10));
      col.resize (10, vector<int>(10));
      sub.resize (10, vector<int>(10));
      grid = src;

      for (int y = 0; y < 9; y++) {
          for (int x = 0; x < 9; x++) {
              int z = z = y / 3 * 3 + x / 3;

              row[y][src[y * 9 + x]]++;
              col[x][src[y * 9 + x]]++;
              sub[z][src[y * 9 + x]]++;
          }
      }

      for (int y = 0; y < 9; y++) {
          for (int x = 0; x < 9; x++) {
              int z = y / 3 * 3 + x / 3;
              vector<int> cand;

              for (int i = 1; i < 10; i++) {
                  if (!row[y][i] && !col[x][i] && !sub[z][i]) {
                      cand.push_back(i);
                  }
              }
              if (cand.size()) {
                  tape.push_back({cand, y * 9 + x});
              }
          }
      }

  }

  int cost () {
      int cnt = 0;

      for (int i = 0; i < 9; i++) {
          cnt += row[i][0];

          for (int j = 1; j < 10; j++) {
              if (row[i][j] > 1) cnt++;
              if (col[i][j] > 1) cnt++;
              if (sub[i][j] > 1) cnt++;
          }
      }

      return cnt;
  }

  void simulated_annealing() {

      random_device rd;
      mt19937 gen(rd());
      uniform_real_distribution<> rng (0, 1);
      uniform_int_distribution<> pos (0, tape.size() - 1);

      double T = 1.0;
      const double alpha = 1.0 - 1e-4, T_min = 0.1;
      int c0, c1;

      while (T > T_min) {

          auto [cand, index] = tape[pos(gen)];
          int x  = index % 9, y = index / 9;
          int z = y / 3 * 3 + x / 3;
          uniform_int_distribution<> rnw (0, cand.size() - 1);

          int dig = cand[rnw(gen)], bak = grid[index];
          // cout << x << " " << y << "::";

          c0 = cost();

          grid[index] = dig;
          row[y][bak]--, col[x][bak]--, sub[z][bak]--;
          row[y][dig]++, col[x][dig]++, sub[z][dig]++;

          c1 = cost();

          // cout << c0 << " " << c1;
          // cout << '\n';

          if (c1 == 0) {
              break;
          } else if (c1 < c0) {

          } else
           // if (exp((c0 - c1) / T) > rng(gen))
          {

              grid[index] = bak;
              row[y][bak]++, col[x][bak]++, sub[z][bak]++;
              row[y][dig]--, col[x][dig]--, sub[z][dig]--;
          }

          T *= alpha;
      }

  }
};

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

      Sudoku curr (convert(grid));

      curr.simulated_annealing();




  cout << "\nend\n";
}
