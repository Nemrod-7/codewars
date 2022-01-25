#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
///////////////////////////////////////assert///////////////////////////////////
#include <stdarg.h>

union u_tag {
  int ival;
  float fval;
  char *cval;
};

int assert_count = 1;
void Test () ;
////////////////////////////////////////////////////////////////////////////////
#define MAX(x,y) (((x) > (y)) ? (x) : (y))g

#define N 7
#define NUNITS (N * sizeof (int))

typedef struct _point { int x, y; } Point;
typedef struct _node {
    int data[N];
    struct _node *next;
} Node;
typedef struct _cluster {
    int head, tail, *data[N];
    Node *root;
} Cluster;
typedef struct _board {
    Cluster **row2, **col2;
    int *cell, **grid;
} Board;
/////////////////////////////////////tools//////////////////////////////////////
void read_data (int input[N]) {
    for (int i = 0; i < N; ++i)
        printf ("%i", input[i]);
}
void display_node (Cluster *input) {

    Node *curr = input->root;

    while (curr) {
        read_data (curr->data);
        printf ("\n");
        curr = curr->next;
    }
}
void showcomb (Board *curr) {
    int i, j;

    printf (" ");
    for (i = 0; i != N; ++i) {
        printf (" %3i ", curr->col2[i]->head);
    }
    printf ("\n");

    for (i = 0; i != N; ++i) {
        printf ("%i", curr->row2[i]->head);

        for (j = 0; j != N; ++j) {
            if (curr->cell[i * N + j])
                printf ("[%3i]",curr->cell[i * N + j]);
            else
                printf ("[  ]");
        }
        printf ("%i\n", curr->row2[i]->tail);
    }

    printf (" ");
    for (i = 0; i != N; ++i) {
        printf (" %3i ", curr->col2[i]->tail);
    }
    printf ("\n");
}
void display (Board *curr) {
    int i, j;

    printf (" ");
    for (i = 0; i != N; ++i) {
        printf (" %i ", curr->col2[i]->head);
    }
    printf ("\n");

    for (i = 0; i != N; ++i) {
        printf ("%i", curr->row2[i]->head);

        for (j = 0; j != N; ++j) {
            if (curr->grid[i][j])
                printf ("[%i]", curr->grid[i][j]);
            else
                printf ("[ ]");
        }
        printf ("%i\n", curr->row2[i]->tail);
    }

    printf (" ");
    for (i = 0; i != N; ++i) {
        printf (" %i ", curr->col2[i]->tail);
    }
    printf ("\n");
}
////////////////////////////////////////////////////////////////////////////////

int bitpos (int byte) {
    int dig = 0;

    do {
        if (byte &1) return dig;
        dig++;
    } while (byte >>= 1);

    return 0;
}
int bitcnt (int byte) {
    int cnt = 0;

    do {
        cnt += (byte &1);
    } while (byte >>= 1);

    return cnt;
}
int bit2int (int byte) {
    int dig = 0;

    do {
        if (byte &1) return dig;
        dig++;
    } while (byte >>= 1);

    return 0;
}
int setcell (int clue) {
    int dig = N - clue + 2, cell = 0;
    if (dig == N + 1) return cell |= 1 << N;

    while (dig-->1) cell |= 1 << dig;

    return cell;
}

void push (Cluster *stack, int *data) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    memcpy (stack->root->data, data, NUNITS);
    stack->root->next = cell;
  }
void freelist (Node *root) {

    for (Node *curr = root, *next; curr != NULL; curr = next) {
        next = curr->next;
        free (curr);
    }
}
void freeboard (Board *actual) {
    for (int i = 0; i < N; ++i) {
        freelist (actual->row2[i]->root), free (actual->row2[i]);
        freelist (actual->col2[i]->root), free (actual->col2[i]);
    }

    free (actual->row2);
    free (actual->col2);
    free (actual->cell);
    free (actual);
}
int **init_grid (int size) {

    int **grid = malloc (N * NUNITS);
    for (int i = 0; i < N; ++i) {
        grid[i] = calloc (size, sizeof(int));
        //for (int j = 0; j < N ; ++j)
        //    ROW = 0;
    }

    return grid;
  }
Board *mk_board (int *clues) {

    Board *next = malloc (sizeof (Board));
    next->cell = calloc (N * N, sizeof(int));
    next->grid = init_grid (NUNITS);
    next->row2 = (Cluster **) init_grid (sizeof (Cluster));
    next->col2 = (Cluster **) init_grid (sizeof (Cluster));

    int *cell = next->cell, full = 0;

    for (int i = 0; i < N; ++i) {

        Point up = { .x = ((N * 4) - 1) - i , .y = i }, bt = {.x = N + i , .y = up.x - N};

        next->row2[i]->head = clues[up.x], next->row2[i]->tail = clues[bt.x];
        next->col2[i]->head = clues[up.y], next->col2[i]->tail = clues[bt.y];

        for (int j = 0; j < N ; ++j) {
            next->col2[i]->data[j] = &cell[j * N + i];
            next->row2[i]->data[j] = &cell[i * N + j];
        }
    }

    for (int i = 1; i <= N; i++) {
        full |= 1 << i;
    }

    for (int i = 0; i < N; ++i) {   // make cells for vertical borders
        int west = next->row2[i]->head, east = next->row2[i]->tail;

        if (west == N) {
            for (int j = 0; j < N; ++j) {
                cell[i * N + j] |= 1 << (j + 1);
            }
        } else if (east == N) {
            for (int j = 0; j < N; ++j) {
                cell[i * N + j] |= 1 << (N - j);
            }
        } else {
            cell[i * N + 0]     = west == 0 ? full : setcell (west);
            cell[i * N + (N - 1)] = east == 0 ? full : setcell (east);
        }
        /*
        */
    }
    for (int i = 0; i < N; ++i) {   // make cells for horizontal borders

        int south = next->col2[i]->tail, north = next->col2[i]->head;
        if (north == N) {
            for (int j = 0; j < N; ++j) {
                cell[j * N + i] |= 1 << (j + 1);
            }
        } else if (south == N) {
            for (int j = 0; j < N; ++j) {
                cell[j * N + i] |= 1 << (N - j);
            }
        } else {
            int up  = north == 0 ? full : setcell (north);
            int dwn = south == 0 ? full : setcell (south);

            if (cell[0 * N + i] == 0 || bitcnt (cell[0 * N + i]) > bitcnt (up))
                cell[0 * N + i] = up;

            if (cell[(N - 1) * N + i] == 0 || bitcnt (cell[(N - 1) * N + i]) > bitcnt (dwn))
                cell[(N - 1) * N + i] = dwn;
        }

    }

    for (int y = 1; y < N - 1; y++) { // make inner cells
        for (int x = 1; x < N - 1; x++) {
            if (cell[y * N + x] == 0)
                cell[y * N + x] = full;
        }
    }

    return next;
}

bool find (int arr[N], int dig, int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == dig) return true;
    }
    return false;
  }
Cluster check_num (int data[N]) {
    int i, max = 0;
    Cluster chk = {0};

    for (i = 0; i < N; ++i) {
        if ( *(data + i) > max) {
            chk.head++; // *(data + i)
            max = MAX (max, data[i]);
        }
    }

    max = 0;
    while (i-->0) {
       if ( *(data + i) > max){
          chk.tail++;
          max = MAX (max, data[i]);
       }
    }

    return chk;
}
bool equals (const Cluster *a, const Cluster *b) {

    if (b->head == 0 && b->tail == 0) {
        return true;
    } else if (b->head == 0 && a->tail == b->tail) {
        return true;
    } else if (b->tail == 0 && a->head == b->head) {
        return true;
    } else if (a->head == b->head && a->tail == b->tail) {
        return true;
    }

    return false;
}
void combinations (Cluster *curr, int comb[N], int i) {

    for (int j = 0; j < N; j++) {
        int dig = j + 1;

        if (*curr->data[i] &1 << dig) {

            if (find (comb, dig, i)) continue;
            comb[i] = dig;

            if (i == N - 1) {
                Cluster a = check_num (comb);

                if (equals (&a, curr))
                    push (curr, comb);

            } else {
                combinations (curr, comb, i + 1);
            }
        }
    }
}

void reduce (Cluster *line) {     // delete possible unique digits from cells

    int i, hist[N + 1] = {};
    bool uniq[N + 1] = {};

    for (i = 0; i < N; i++) {
        int mask = *line->data[i], dig = 0;
        if ((mask & (mask - 1)) == 0)       // search if a cell contains a unique digit (cannot be present in other cells)
            uniq[bit2int (mask)] = true;

        do {
            if (mask &1) hist[dig]++;       // search if a digit is unique to a cell (not present in other cells)
            dig++;
        } while (mask >>= 1);
    }

    for (i = 0; i < N; i++) {
        int curr = *line->data[i];

        if (bitcnt (curr) > 1) {
            for (int j = 1; j < N + 1; j++) {

                bool exist = (curr &1 << j);
                if (exist) {
                    if (hist[j] == 1) {     // delete other digits if this cell contains a unique digit not present elsewhere
                        curr = 0;
                        curr |= 1 << j;
                    }
                    if (uniq[j]) {          // delete the unique digit found from other cells if present
                        curr ^= 1 << j;
                    }
                }
            }
        }
        *line->data[i] = curr;
    }
}
void reduce2 (Board *curr) {      // reducer multiplier

  for (int i = 0; i < N; i++) {
    reduce (curr->row2[i]), reduce (curr->col2[i]);
  }
}
void filter (Cluster *line) {     // search possible combination and rearrange bitmasking

    if (line->head == 0 && line->tail == 0) return;
    if (line->root != NULL) {
        freelist(line->root);
    }

    line->root = NULL;
    int arr[N] = {};
    combinations (line, arr, 0);        // create possible combinations for this line

    for (int i = 0; i < N; i++) {
        *line->data[i] = 0;
    }

    Node *comb = line->root;
    while (comb) {
        for (int i = 0; i < N; i++) {
            *line->data[i] |= 1 << comb->data[i];   // rearrange bitmasking
        }
        comb = comb->next;
    }
}
void upgrade_grid (Board *curr) { // update grid if a cell contains only one digit
    int loop = 4;

    while (loop-->0) {
        for (int i = 0; i < N; i++) {
            reduce2 (curr);
            filter (curr->row2[i]), filter (curr->col2[i]);
        }
    }

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            int num = curr->cell[y * N + x];

            if ((num & (num - 1)) == 0)  // if is pow of 2 -> on possible number
                curr->grid[y][x] = bitpos (num);
        }
    }
}

bool is_valid (Board *curr) {
    int col[N] = {}, row[N] = {};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            row[j] = curr->grid[i][j];
            col[j] = curr->grid[j][i];
        }

        Cluster *horiz = curr->row2[i], *vert = curr->col2[i];
        Cluster a = check_num (row), b = check_num (col);

        if (!equals (&a, horiz) || !equals (&b, vert)) {
            return false;
        }
    }

    return true;
}
bool checkcol (int **grid, Point p, int num) {

    for (int y = 0; y < N; y++)
        if (grid[y][p.x] == num)
            return false;

    return true;
}
bool checkrow (int **grid, Point p, int num) {

    for (int x = 0; x < N; x++)
        if (grid[p.y][x] == num)
            return false;

    return true;
}
bool backtrack (Board *curr, Point p) {

    if (p.x == N) {
        p.x = 0;
        p.y++;
    }

    if (p.y == N) {
        return (is_valid (curr)) ? true : false;
    }

    if (curr->grid[p.y][p.x]) return backtrack (curr, (Point) {p.x + 1, p.y});

    for (int dig = 1; dig < N + 1; dig++) {

        if ((curr->cell[p.y * N + p.x] &1 << dig) && checkrow (curr->grid, p, dig) && checkcol (curr->grid, p, dig)) {
              curr->grid[p.y][p.x] = dig;

              if (backtrack (curr, (Point) {p.x + 1, p.y}))
                  return true;
        }
    }

    curr->grid[p.y][p.x] = 0;

    return false;
}

int **SolvePuzzle (int *clues) {
    Board *curr = mk_board (clues);
    int **puzzle = curr->grid;
 /**
 * description.
 *
 * @param clues .
 *
 * @return puzzle.
 */

    upgrade_grid(curr);
    backtrack (curr, (Point) {0,0});

    display (curr);
    return puzzle;
}

int main (void) {

    time_t start = clock(), stop;

    SolvePuzzle((int[]){3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});
    Test ();

    stop = clock();
    printf ("Process took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
    return 0;
}

/////////////////////////////////////tests//////////////////////////////////////
#define SIZE 7
static int clues[][SIZE * 4] = {
  { 7, 0, 0, 0, 2, 2, 3,
    0, 0, 3, 0, 0, 0, 0,
    3, 0, 3, 0, 0, 5, 0,
    0, 0, 0, 0, 5, 0, 4 },
  { 0, 2, 3, 0, 2, 0, 0,
    5, 0, 4, 5, 0, 4, 0,
    0, 4, 2, 0, 0, 0, 6,
    5, 2, 2, 2, 2, 4, 1 }
};
static int expected[][SIZE][SIZE] = {
  { { 1, 5, 6, 7, 4, 3, 2 },
    { 2, 7, 4, 5, 3, 1, 6 },
    { 3, 4, 5, 6, 7, 2, 1 },
    { 4, 6, 3, 1, 2, 7, 5 },
    { 5, 3, 1, 2, 6, 4, 7 },
    { 6, 2, 7, 3, 1, 5, 4 },
    { 7, 1, 2, 4, 5, 6, 3 } },
  { { 7, 6, 2, 1, 5, 4, 3 },
    { 1, 3, 5, 4, 2, 7, 6 },
    { 6, 5, 4, 7, 3, 2, 1 },
    { 5, 1, 7, 6, 4, 3, 2 },
    { 4, 2, 1, 3, 7, 6, 5 },
    { 3, 7, 6, 2, 1, 5, 4 },
    { 2, 4, 3, 5, 6, 1, 7 } }
};

void cr_assert(int test,...) {

  va_list list;
  va_start (list,test);
  union u_tag data;

  assert_count++;

  if (test == 0) {
    //data.cval = va_arg(list,char *);
      printf ("\n:::test->%i :::\n",assert_count);
      printf ("%s\n",va_arg(list,char *));
    }

    va_end (list);
}
void cr_assert_eq(int test, int cond,...) {

  #define TOKEN '%'
  #define SPACE ' '
  va_list list;
  va_start (list,cond);
  union u_tag data;
  char *c;

  assert_count++;
  if (test != cond) {
      printf ("\n:::test->%i :::\n",assert_count);
      c = va_arg(list,char *);

      do {

        if (*c == TOKEN) {
            c++;

            switch (*c) {
                case 'i':
                  data.ival = va_arg(list,int);
                  printf ("%i",data.ival);
                  break;
                case 'd':
                  data.ival = va_arg(list,int);
                  printf ("%i",data.ival);
                  break;
                case 'l':
                  data.ival = va_arg(list,int);
                  printf ("%l",data.ival);
                  break;
                case 'f':
                  data.fval = va_arg(list,double);
                  printf ("%f",data.fval);
                  break;
                case 's':
                  data.cval = va_arg(list,char *);
                  printf ("%s",data.cval);
                  break;
                case 'z':
                  data.ival = va_arg(list,int);
                  printf ("%i",data.ival);
                  break;
                default:
                  c++;
            }
            while (*c != SPACE)
              c++;
        }

        putchar (*c);

      } while (*c++);

      printf ("\n");
    }

}

int check (int **solution, int (*expected)[N]) {

    int result = 0;
    if (solution && expected) {
        result = 1;
        for (int i = 0; i < N; i++)
            if (memcmp(solution[i], expected[i], N * sizeof(int))) {
                result = 0;
                break;
            }
      }

    /*
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            printf ("%i ",solution[i][j]);


    printf ("\n");
    }
    printf ("\n");
    */
    return result;
}

void Test () {

    {
      SolvePuzzle((int[]){
        2,3,3,4,4,2,1,
        1,6,4,4,2,2,2,
        2,2,3,3,1,3,3,
        2,5,3,3,2,1,5});
    }

    {
      SolvePuzzle((int[]){
        2,2,0,2,0,2,0,
        0,3,3,0,3,3,0,
        5,4,6,3,0,0,0,
        0,0,4,4,0,0,4
      });
    }

    //cr_assert(check(SolvePuzzle(clues[0]), expected[0]));
    //cr_assert(check(SolvePuzzle(clues[1]), expected[1]));

    {
      SolvePuzzle((int[]){0,0,5,0,0,0,6,4,0,0,2,0,2,0,0,5,2,0,0,0,5,0,3,0,5,0,0,3});
    }
    {
      SolvePuzzle((int[]){0,0,5,3,0,2,0,0,0,0,4,5,0,0,0,0,0,3,2,5,4,2,2,0,0,0,0,5});
    }
    {
      SolvePuzzle((int[]){0,2,3,0,2,0,0,5,0,4,5,0,4,0,0,4,2,0,0,0,6,5,2,2,2,2,4,1});
    }

    {
      SolvePuzzle((int[]){0,2,3,0,2,0,0,5,0,4,5,0,4,0,0,4,2,0,0,0,6,0,0,0,0,0,0,0});
    }

    {
      SolvePuzzle((int[]){7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0,0,0,0,0,5,0,4});
    }
    {
      SolvePuzzle((int[]){0,0,0,0,5,0,4,7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0});
    }

    {
      SolvePuzzle((int[]){0,0,0,0,5,0,4,7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0});
    }

    {
      SolvePuzzle((int[]){3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});
    }

}
