#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
///////////////////////////////////////assert///////////////////////////////////
#include <stdarg.h>

union u_tag{
  int ival;
  float fval;
  char *cval;
};

int assert_count = 1;
////////////////////////////////////////////////////////////////////////////////

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define ROW (*(*(grid + i) + j))
#define COL (*(*(grid + j) + i))
#define N 7
#define NUNITS (N * sizeof (int))

typedef struct _point {
    int x;
    int y;
} Point;
typedef struct _node {
    int data[N];
    struct _node *next;
} Node;
typedef struct _cluster {
    int head;
    int tail;
    int *sky[N];
    Node *root;
} Cluster;
typedef struct _board {
    Cluster **row;
    Cluster **col;
} Board;
typedef int** Matrix;
//////////////////////////////////func def//////////////////////////////////////

void read_data (int *input);
void display_node (Cluster *input);
void display_clust (Cluster *input);
void display (Board *now) ;
void Test () ;

void push(Cluster *stack, int *data) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    memcpy (stack->root->data, data, NUNITS);
    stack->root->next = cell;
}
int swap (int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

Cluster check_num (int data[N]) {
    int i, max = 0;
    Cluster chk = {0};

    for (i = 0; i < N; ++i) {
        if ( *(data + i) > max)
            chk.head++; // *(data + i)
            max = MAX (max, data[i]);
    }

    max = 0;
    while (i-->0) {
       if ( *(data + i) > max)
          chk.tail++;
          max = MAX (max, data[i]);
    }

    return chk;
}
void make_combs (Node *root, Cluster *actual) {

    while (root) {
        Cluster comb = check_num (root->data);

        if (actual->head != 0 && actual->tail != 0) {
            if ((comb.head == actual->head) && (comb.tail == actual->tail))
                  push (actual, root->data);
        }

        else if (actual->head != 0 && comb.head == actual->head)
                  push (actual, root->data);

        else if (actual->tail != 0 && comb.tail == actual->tail)
                  push (actual, root->data);

        else if (actual->tail == 0 && actual->head == 0 )
                  push (actual, root->data);

        root = root->next;
    }
}
void permutation (int a[], int size, Cluster *now) {

    if (size == 1) {
        push (now, a);
        return;
    }

    for (int i = 0; i < size; i++) {
        permutation(a, size - 1, now);
        (size % 2 == 1) ? swap (&a[i], &a[size-1]) : swap (&a[0], &a[size-1]);
    }
}
Matrix init_matrix (int size) {

    Matrix grid = malloc (N * NUNITS);
    for (int i = 0; i < N; ++i) {
        grid[i] = calloc (size, sizeof(int));
        //for (int j = 0; j < N ; ++j)
        //    ROW = 0;
    }

    return grid;
}
Board *init_board (int *clues, Matrix grid) {

    Board *next = malloc (sizeof (Board));
    unsigned int comb[7] = {1,2,3,4,5,6,7};

    next->row = init_matrix (sizeof (Cluster));
    next->col = init_matrix (sizeof (Cluster));
    Cluster combinations = {0};
        permutation(comb, N, &combinations);

    for (int i = 0; i < N; ++i) {

        Point up = {.x = ((N * 4) - 1) - i , .y = i }, bt = {.x = N + i , .y = up.x - N};

        next->row[i]->head = clues[up.x], next->row[i]->tail = clues[bt.x];
        next->col[i]->head = clues[up.y], next->col[i]->tail = clues[bt.y];

        next->row[i]->root = NULL;
        next->col[i]->root = NULL;

        for (int j = 0; j < N ; ++j) {
            next->col[i]->sky[j] = &COL;
            next->row[i]->sky[j] = &ROW;
        }

        make_combs (combinations.root, next->row[i]);
        make_combs (combinations.root, next->col[i]);
    }

    return next;
}
void freelist (Node *root) {

  for (Node *curr = root, *next; curr != NULL; curr = next) {
      next = curr->next;
      free (curr);
  }
}
void freeboard (Board *actual) {

    for (int i = 0; i < N; ++i) {
        freelist (actual->row[i]->root), free (actual->row[i]);
        freelist (actual->col[i]->root), free (actual->col[i]);
    }

    free (actual->row);
    free (actual->col);
}

void update (Cluster *input, Cluster *check, Point coord) {

    unsigned int now[N] = {0}, *entry = *input->sky;
    bool flag[N];
    Node *curr = input->root, *trash ;

    memset (flag,1, N * sizeof(bool));
    input->root = NULL;

    while (curr) {
        bool match = false;

        for (Node *temp = check->root; temp; temp = temp->next)
            if(curr->data[coord.x] == temp->data[coord.y]) {

                for (int i = 0; i < N; ++i) {
                    if (now[i] > 0 && now[i] != curr->data[i])
                          flag[i] = false;

                    now[i] = curr->data[i];
                }

                push (input, curr->data);
                break;
            }

        //trash = curr;
        curr = curr->next;

        //free (trash);
    }

    for (int i = 0; i < N; ++i)
        if (flag[i] == true)
            *input->sky[i] = now[i];

}
bool growing (Matrix grid) {

    static int total = N * N, expension = 0;
    int count = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (ROW != 0)
                count++;

    if (count != 0 && count < expension || count == total)
        return false;

    expension = count;

    return true;
}
int** SolvePuzzle (int *clues) {

    Matrix puzzle = init_matrix (NUNITS);
    Board *actual = init_board (clues, puzzle);
    unsigned index = 5;

    //while (index--> 0) {
    while (growing (puzzle)) {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N ; ++j) {
                 Point p = {.x = j, .y = i};
                 update (actual->row[i], actual->col[j], p);
                 update (actual->col[i], actual->row[j], p);
            }
        //    display (actual);
    }



    display (actual);
    //display_node (actual->row[1]);
    //freeboard (actual);
    //free (actual);

    return puzzle;
}

int main(void) {

    time_t start = clock(), stop;

    unsigned int comb[10] = {1,2,3,4,5,6,7,8,9,10};

    //printf ("%i\n", 5039 * N * 2);
    //  SolvePuzzle((int[]){3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});
    Test ();

    stop = clock();
    printf ("Process took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
    return 0;
  }
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
void display_clust (Cluster *input) {

    if (input) {
        printf ("[%i][",input->head);
        for (int i = 0; i < N; ++i)
            printf ("%i",*input->sky[i]);
        printf ("][%i]\n",input->tail);
    }

}
void display (Board *now) {

  printf ("   ");
  for (int x = 0; x < N; ++x)
      printf (" %i ",now->col[x]->head);

  printf ("\n");
  for (int x = 0; x < N; ++x) {
      printf (" %i ",now->row[x]->head);
      for (int y = 0; y < N; ++y) {
          if (*now->row[x]->sky[y])
              printf ("[%i]",*now->row[x]->sky[y]);
          else
              printf ("[ ]");
      }
      printf (" %i \n",now->row[x]->tail);
  }
  printf ("   ");
  for (int x = 0; x < N; ++x)
      printf (" %i ",now->col[x]->tail);
  printf ("\n");

}
/////////////////////////////////////tests//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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

    cr_assert(check(SolvePuzzle(clues[0]), expected[0]));
    cr_assert(check(SolvePuzzle(clues[1]), expected[1]));

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
    //  SolvePuzzle((int[]){3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});
    }

}
