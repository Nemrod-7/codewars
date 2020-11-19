#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct _point {
    int x, y;
} Point;
typedef struct _node {
    int size;
    Point arr[64];
} Array;

const int N = 8;
Point knight_moves[] = {{-2,-1},{-2,+1}, {-1,-2},{-1,+2},
                        {+1,-2},{+1,+2},{+2,-1},{+2,+1}};
////////////////////////////////////func def////////////////////////////////////
int** init_grid();
Point get_point(const char *pt);
short knight(const char *p1, const char *p2) ;
void display(int** board);
void Test();
////////////////////////////////////////////////////////////////////////////////

int main () {


    knight("a1", "b1");
    //get_point ("a1");

    //Test();


}

bool is_inside (Point p) {
    return (p.x >=0 && p.y >=0 && p.x < N && p.y < N);
}
Point get_point(const char *pt) {
    Point actual;
    actual.y = islower(pt[0]) ? pt[0] - 'a': pt[0] - 'A';
    actual.x = pt[1] - '1';
    return actual;
}
bool update (int board[N][N], Array *curr, Point final) {

    int end = curr->size, next = curr->size;

    for (int i = 0; i < end; ++i) {
        for (int j = 0; j < 8; ++j) {
            Point p = {curr->arr[i].x + knight_moves[j].x, curr->arr[i].y + knight_moves[j].y};
            if (is_inside(p) && !board[p.x][p.y]) {
                if (p.x == final.x && p.y == final.y) {
                    return true;
                }
                curr->arr[next].x = p.x, curr->arr[next].y = p.y;

                board[p.x][p.y] = true;
                next++;
            }
        }
    }
    curr->size = next;
    //printf ("%i\n",next);
    return false;
}
short knight(const char *p1, const char *p2) {

    int board[8][8] = {0};
    Point start = get_point(p1), exit = get_point(p2);
    Array actual = {.size = 1, .arr[0] = start};
    short steps = 1;

    while (update (board, &actual, exit) == false) {
        steps++;
          }
    //printf ("steps %i\n", steps);

    return steps;

}

void display(int** board) {
    printf ("----------board---------\n\n");
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            printf ("[%c]",board[x][y] != 0 ? '#' : ' ');
        }
        printf ("\n");
    }
    printf ("\n");
}

void Test() {
    knight("a1", "c1");
    knight("a1", "f1");
    knight("a1", "f3");

    knight("a1", "f4");
    knight("a1", "f7");

    knight("c5","a2");
    knight("h8", "h3");
    knight("h7", "g7");
    knight("b4", "g1");
    knight("a3", "e1");
    knight("a1", "b2");

    knight("e8", "b7");
    knight("f8", "f4");
    knight("b2", "a1");

}
