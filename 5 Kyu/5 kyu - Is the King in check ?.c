#include <stdio.h>
#include <stdbool.h>

typedef struct _point { int x, y; } point;

void display (const char board[8][8]) {

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char tile = board[j][i];

            switch (tile) {
                case 'K' : printf ("\u265A"); break;
                case 'Q' : printf ("\u2655"); break;
                case 'R' : printf ("\u2656"); break;
                case 'B' : printf ("\u2657"); break;
                case 'N' : printf ("\u2658"); break;
                case 'P' : printf ("\u2659"); break;
                case ' ' : printf (" "); break;
            }
            printf ("|");
        }
        printf ("\n");
    }
}

const point knig[8] = {{1,-2},{-1,-2},{1,2},{-1,2},{2,-1},{-2,-1},{2,1},{-2,1}};
const point cross[4] = {{0,1},{0,-1},{1,0},{-1,0}};
const point diag[4] = {{-1,-1},{-1,1},{1,1},{1,-1}};

bool is_inside (point p) {
  return p.x >= 0 && p.y >=0 && p.x < 8 && p.y < 8;
}
point search (const char board[8][8], const char id) {
  point king;
  for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
          if (board[i][j] == id) king.x = i, king.y = j;
      }
  }
  return king;
}
char chkline (const char board[8][8], point p, point dir, int dist) {
    for (int i = 1; i < dist; i++) {
        point nx = {p.x + dir.x * i, p.y + dir.y * i};
        char tile = board[nx.x][nx.y];

        if (is_inside (nx) && tile != ' ') {
            return tile;
        }
    }
    return ' ';
}
bool knight (const char board[8][8], const point p) {

    for (int i = 0; i < 8; i++) {
        point nx = {p.x + knig[i].x, p.y + knig[i].y};

        if (is_inside (nx)) {
            if (board[nx.x][nx.y] == 'N') {
                  return true;
            }
        }
        //board[p.x + nx.x][p.y + nx.y] = '.';
    }

    return false;
}

bool king_is_in_check (const char board[8][8]) {

    point king = search (board, 'K');

    for (int i = 0; i < 4; i++) {
        char corn = chkline (board, king, diag[i], 8);
        char strai = chkline (board, king, cross[i], 8);
        char near = chkline (board, king, diag[i], 2);

        if (i < 2 && near == 'P') return true;
        if (corn == 'Q' || corn == 'B') return true;
        if (strai == 'Q' || strai == 'R') return true;
    }

    if (knight (board, king) == true) return true;

    return false;
}

int main () {

  bool chkdiag (const char board[8][8], const point p, int dist) {


    for (int i = 1; i <= dist; i++) {

      for (int j = 0; j < 4; j++) {
        point nx = {p.x + diag[j].x * i, p.y + diag[j].y * i};
        char tile = board[nx.x][nx.y];
        if (is_inside (nx) && tile != ' ') {
          printf ("%c", tile);
          if (tile == 'Q' || tile == 'B') {
            return true;
          } else {

          }
        }
      }
    }
    return false;
  }
  bool chkcross (const char board[8][8], const point p, int dist) {

    for (int i = 1; i <= dist; i++) {

      for (int j = 0; j < 4; j++) {
        point nx = {p.x + cross[j].x * i, p.y + cross[j].y * i};
        char tile = board[nx.x][nx.y];

        if (is_inside (nx) && tile != ' ') {
          //printf ("%c ", tile);
          if (tile == 'R' || tile == 'Q') {
            //    return true;
          }
        }
      }
    }
    return false;
  }
   char board[8][8] = {
     "        ",
   		"        ",
   		"  A B   ",
   		"   K    ",
   		"  C D   ",
   		"        ",
   		"        ",
   		"        "
};
   bool check = king_is_in_check (board);

   printf ("%i", check);

}
