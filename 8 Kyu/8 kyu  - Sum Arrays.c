#include <stdbool.h>

#define VALCELL(x) (board[(x[1] - '1' )][(x[0] - 'A')])

const bool board[8][8] =
         {
         {0, 1, 0, 1, 0, 1, 0, 1},
         {1, 0, 1, 0, 1, 0, 1, 0},
         {0, 1, 0, 1, 0, 1, 0, 1},
         {1, 0, 1, 0, 1, 0, 1, 0},
         {0, 1, 0, 1, 0, 1, 0, 1},
         {1, 0, 1, 0, 1, 0, 1, 0},
         {0, 1, 0, 1, 0, 1, 0, 1},
         {1, 0, 1, 0, 1, 0, 1, 0}
         };

bool chess_board_cell_color(const char* cell1, const char *cell2) {

return (VALCELL(cell1) == VALCELL(cell2)) ? true : false;
}
