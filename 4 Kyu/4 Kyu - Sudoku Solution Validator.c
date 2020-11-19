#include<stdbool.h>

#define IS_CENTER(x) (((x) % 3) == true)

bool check_array (int array[10]) {
  bool flag = true;
  for (int i = 1; i < 10; ++i) {
      if (array[i] > 1)
          flag = false;
      array[i] = 0;
  }
  return flag;
}
bool check_block (unsigned int board[9][9],int x, int y) {

    int neigh[10] = {0};

    for (int i = (x - 1); i <= (x + 1); i++)
        for (int j = (y - 1); j <= (y + 1); j++) {
            neigh[board[i][j]]++;
        }

    return check_array(neigh);
}
bool validSolution (unsigned int board[9][9]) {

    bool is_valid = true;
    int row[10] = {0}, col[10] = {0};

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (IS_CENTER(i) && IS_CENTER(j)) {
                if (check_block (board,i,j) == false)
                    is_valid = false;
            }
            row [board[i][j]]++;
            col [board[j][i]]++;
        }
        if (check_array (row) == false || check_array (col) == false)
            is_valid = false;
    }

    return is_valid;
}
