#include <sstream>
#include <vector>

using namespace std;

using cube = vector<vector<vector<char>>>;

const vector<char> players {'O', 'X'};

int check2 (cube &V) {
  const int N = 4;

  for (size_t i = 0; i < players.size(); i++) {
      char player = players[i];

      vector<int> ddiag(N), adiag(N);
      vector<int> row1 (N), row2 (N), row3(N), row4(N);
      vector<int> col1 (N), col2 (N), col3(N), col4(N);
      vector<vector<int>> phase (N, vector<int> (N));

      for (int z = 0; z < N; z++) {
          int z2 = 3 - z;
          // 2D check
          int diagr = 0, diagl = 0;
          for (int y = 0; y < N; ++y) {
              int line = 0, col = 0;
              for (int x = 0; x < N; x++) {

                  if (V[z][x][y] == player) col++;
                  if (V[z][y][x] == player) {
                      line++;
                      phase[y][x]++;
                      if (x == y) diagr++;         // diag from left
                      if (x + y == 3) diagl++;    // diag from right
                  }
                  if (line == 4 || col == 4 || diagr == 4 || diagl == 4) return i;
                  if (phase[y][x] == 4) return i;
              }
          }
          // 3D check diagonals
          //descending
          if (V[z][z][z] == player) ddiag[0]++;
          if (V[z][z][z2] == player) ddiag[1]++;
          if (V[z][z2][z2] == player) ddiag[2]++;
          if (V[z][z2][z] == player) ddiag[3]++;
          //ascending
          if (V[z2][z][z] == player) adiag[0]++;
          if (V[z2][z][z2] == player) adiag[1]++;
          if (V[z2][z2][z2] == player) adiag[2]++;
          if (V[z2][z2][z] == player) adiag[3]++;

          for (int j = 0; j < N; j++)
              if (ddiag[j] == 4 || adiag[j] == 4) return i;

          // 3D check rows
          for (int y = 0; y < N; y++) {
              // descending
              if (V[z][y][z] == player) row1[y]++;
              if (V[z][y][z2] == player) row2[y]++;

              if (V[z][z][y] == player) col1[y]++;
              if (V[z][z2][y] == player) col2[y]++;
              //ascending
              if (V[z2][y][z] == player) row3[y]++;
              if (V[z2][y][z2] == player) row4[y]++;

              if (V[z2][z][y] == player) col3[y]++;
              if (V[z2][z2][y] == player) col4[y]++;

              if (row1[y] == 4 || row2[y] == 4 || row3[y] == 4 || row4[y] == 4) return i;
              if (col1[y] == 4 || col2[y] == 4 || col3[y] == 4 || col4[y] == 4) return i;
          }
      }
  }
  return -1;
}
string playOX3D(const vector<vector<int>> &game) {
    const int N = 4;
    ostringstream oss;
    cube qubiq (N, vector<vector<char>> (N, vector<char> (N)));

    int res;

    for (size_t i = 0; i < game.size(); i++) {

        int x = game[i][0], y = game[i][1], z = game[i][2];
        qubiq[z][y][x] = players[i % 2];

        res = check2 (qubiq);

        if (res != -1) {
            oss << players[res] << " wins after " << i + 1 << " moves";
            return oss.str();
        }
    }

    return "No winner";
}
