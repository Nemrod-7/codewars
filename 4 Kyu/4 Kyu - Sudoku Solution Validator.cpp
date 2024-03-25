#include <iostream>
#include <vector>

using namespace std;

bool validSolution (unsigned int board[9][9]) {

    for (int i = 0; i < 9; i++) {
        int line = 0, col = 0;
        for (int j = 0; j < 9; j++) {
            line += board[i][j];
            col += board[j][i];
        }
        if (line != 45 || col != 45) return false;
    }

    for (int k = 0; k < 9; k++) {
        int centx = k / 3 * 3, centy = k % 3 * 3, cnt = 0;

        for (int i = centx; i < centx + 3; i++) {
            for (int j = centy; j < centy + 3; j++) {
                cnt += board[i][j];
            }
        }
        if (cnt != 45) return false;
    }

    return true;
}

int main () {

    unsigned int board[9][9];
    int row[10] = {};

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            board[i][j] = i * 9 + j;
        }
    }

    int cnt = 0;
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int zx = y / 3 * 3 + x / 3, zy = y % 3 * 3 + x % 3;
            
            if (board[y][x] < 0 || board[y][x] > 9) {
                return false;
            }
            if (board[y][x]) cnt++;

            // row[y] |= 1 << board[y][x];
            // col[x] |= 1 << board[x][y];
            // col[zx] |= 1 << board[zx][zy];

            // std::cout << board[zx][zy] << ' ';
        }
        // std::cout << '\n';
    }

}
