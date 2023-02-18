#include <iostream>
#include <array>
#include <vector>

using namespace std;

bool validate (const std::array<std::array<int, 9>, 9>& board) {

    for (int i = 0; i < 9; i++) {
        std::array<int, 10> line = {0}, col = {0};
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
              return false;
            }
            line[board[i][j]]++;
            col[board[j][i]]++;
        }
        for (int j = 1; j < 10; j++) {
            if (col[i] != 1 || line[i] != 1) {

              return false;
            }
        }
        cout << endl;
    }

    for (int k = 0; k < 9; k++) {
        std::array<int, 10> block = {0};
        int centx = k / 3 * 3, centy = k % 3 * 3, cnt = 0;

        for (int i = centx; i < centx + 3; i++) {
            for (int j = centy; j < centy + 3; j++) {
                block[board[i][j]]++;
            }
        }
        for (int j = 1; j < 10; j++) {
            if (block[j] != 1) return false;
        }
    }

    return true;
}

int main () {


    const std::array<std::array<int, 9>, 9> board =
      {{{5,3,4,6,7,8,9,1,2},
      {6,7,2,1,9,5,3,4,8},
      {1,9,8,3,4,2,5,6,7},
      {8,5,9,7,6,1,4,2,3},
      {4,2,6,8,5,3,7,9,1},
      {7,1,3,9,2,4,8,5,6},
      {9,6,1,5,3,7,2,8,4},
      {2,8,7,4,1,9,6,3,5},
      {3,4,5,2,8,6,1,7,9}}};

      validate (board);

}
