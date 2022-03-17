#include <stdio.h>
#include <stdbool.h>

typedef struct _point { int x, y; } point;

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

bool king_is_in_check (const char board[8][8]) {
    point king = search (board, 'K');


    return false;
}

bool diag (char board[8][8], const point p, int dist) {

    const point diag[4] = {{1,1},{-1,-1},{1,-1},{-1,1}};

    for (int i = 1; i <= dist; i++) {


        for (int j = 0; j < 4; j++) {
            point nx = {p.x + diag[j].x * i, p.y + diag[j].y * i};

            if (is_inside (nx)) {
                board[nx.x][nx.y] = '.';
            }
        }
    }
    return false;
}
bool cross (char board[8][8], const point p, int dist) {

    const point cross[4] = {{0,1},{0,-1},{1,0},{-1,0}};

    for (int i = 1; i <= dist; i++) {


        for (int j = 0; j < 4; j++) {
            point nx = {p.x + cross[j].x * i, p.y + cross[j].y * i};

            if (is_inside (nx)) {
                board[nx.x][nx.y] = '.';
            }
        }
    }
    return false;
}
bool knight (char board[8][8], const point p) {
    const point knig[8] = {{1,-2},{-1,-2},{1,2},{-1,2},{2,-1},{-2,-1},{2,1},{-2,1}};

    for (int i = 0; i < 8; i++) {
        point nx = knig[i];
        if (board[p.x + nx.x][p.y + nx.y] == 'N')
            return true;
        board[p.x + nx.x][p.y + nx.y] = '.';
    }

    return false;
}
int main () {
  /*
   king_is_in_check ((const char[8][8]) {
		"   B    ",
		"        ",
		"  K     ",
		"  N     ",
		"    P   ",
		"        ",
		"      Q ",
		"        "
	});
    */

    char board[8][8] = { };
    board[3][3] = 'K';
    point p = {3,3};

    //cross (board, p, 8);
    diag (board, p, 8);
    //knight (board, p);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[j][i] == '\0') printf (" |");
            else printf ("%c|", board[j][i]);

        }
        printf ("\n");
    }
}
