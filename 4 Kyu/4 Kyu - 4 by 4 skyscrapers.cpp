#include <iostream>
#include <vector>
#include <algorithm>

#include <chrono>
#include <cstring>

#define N 4

using namespace std;

typedef struct _point {
    int x;
    int y;
} Point;
typedef struct _cluster {
    int head;
    int tail;
    int *sky[N];
    vector<vector<int>> root;
} Cluster;
typedef struct _board {
    vector<Cluster> row;
    vector<Cluster> col;
} Board;

//////////////////////////////////func def//////////////////////////////////////
int** SolvePuzzle (int *clues);

void read_cluster (Cluster input) ;
void read_vector (vector<vector<int>> input);
void display (Board *now);

void Test ();
////////////////////////////////////////////////////////////////////////////////
static int clues[][16] = {
{ 2, 2, 1, 3,
  2, 2, 3, 1,
  1, 2, 2, 3,
  3, 2, 1, 3 },
{ 0, 0, 1, 2,
  0, 2, 0, 0,
  0, 3, 0, 0,
  0, 1, 0, 0 },
};

int outcomes[][4][4] = {
{ { 1, 3, 4, 2 },
  { 4, 2, 1, 3 },
  { 3, 4, 2, 1 },
  { 2, 1, 3, 4 } },
{ { 2, 1, 4, 3 },
  { 3, 4, 1, 2 },
  { 4, 2, 3, 1 },
  { 1, 3, 2, 4 } },
};

////////////////////////////////////////////////////////////////////////////////

int main () {

    auto start = chrono::high_resolution_clock::now();
    int clues[] = {0,0,1,2, 0,2,0,0, 0,3,0,0, 0,1,0,0};

    //SolvePuzzle (clues);
    Test();

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;

    return 0;
}

int **init_matrix () {

    int **grid = new int*[N];
    for (int i = 0; i < N; ++i) {
        grid[i] = new int[N];
        fill_n (grid[i],N,0);
    }
    return grid;
}
Cluster check_num (vector<int> data) {

    int maxval = 0, index;
    Cluster chk = {0};

    for (int now : data) {
        if (now > maxval)
            chk.head++;

        maxval = max (maxval, now);
    }

    maxval = 0, index = 4;
    while (index-->0) {
       if (data[index] > maxval)
          chk.tail++;

       maxval = max (maxval, data[index]);
    }

    return chk;
}
vector<vector<int>> permutation () {

    vector<vector<int>> actual;
    vector<int> num{1,2,3,4};

    do { actual.push_back(num);
    } while (next_permutation(num.begin(), num.end()));

    return actual;
}
void make_combs (vector<vector<int>> combinations, Cluster *now) {

    for (vector<int> data : combinations) {
        Cluster comb = check_num (data);

        if (now->head != 0 && now->tail != 0) {
            if ((comb.head == now->head) && (comb.tail == now->tail))
                  now->root.push_back (data);
        }
        else if (now->head != 0 && comb.head == now->head)
                  now->root.push_back (data);

        else if (now->tail != 0 && comb.tail == now->tail)
                  now->root.push_back (data);

        else if (now->tail == 0 && now->head == 0 )
                  now->root.push_back (data);
    }
}
Board *init (int **grid, int *clues) {

    Board *next = new Board;
    vector<vector<int>> combs = permutation();

    for (int i = 0; i < N; i++) {
        Point up = {.x = ((N * 4) - 1) - i , .y = i }, bt = {.x = N + i , .y = up.x - N};

        next->row.push_back ({.head = clues[up.x], .tail = clues[bt.x]});
        next->col.push_back ({.head = clues[up.y], .tail = clues[bt.y]});

        for (int j = 0; j < N; j++) {
            next->row[i].sky[j] = &grid[i][j];
            next->col[i].sky[j] = &grid[j][i];
        }
        make_combs (combs, &next->row[i]);
        make_combs (combs, &next->col[i]);
    }

    return next;
}
void upgrade (Cluster *input, vector<vector<int>> *combs, Point p) {

    int i, now[N] = {0}, *entry = *input->sky;
    vector<vector<int>>::iterator index = input->root.begin();
    bool flag[N], match;

    fill_n (flag, N, 1);

    while (index != input->root.end()) {

        vector<int>::iterator curr = index->begin();
        match = false;

        for (vector<int> next : *combs)
            if (curr[p.x] == next[p.y]) {
                match = true;
                break;
            }

        if (match == false)
            input->root.erase(index);
        else {
            for (i = 0; i < N; ++i) {
                if (now[i] > 0 && now[i] != curr[i])
                    flag[i] = false;

                now[i] = curr[i];
            }
            index++;
        }
    }

    for (i = 0; i < N; ++i)
        if (flag[i] == true)
            *input->sky[i] = now[i];
}
bool growing (int **grid) {

    static int total = N * N, expension = 0;
    int count = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] != 0)
                count++;

    if (count != 0 && count < expension || count == total)
        return false;

    expension = count;

    return true;
}
int** SolvePuzzle (int *clues) {

    int **grid = init_matrix(), index = 3;
    Board *actual = init(grid,clues);

    while (growing (grid))
    //while (index-->0)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                Point p = {.x = j, .y = i};
                upgrade (&actual->row[i], &actual->col[j].root, p);
                upgrade (&actual->col[i], &actual->row[j].root, p);
            }

    display (actual);

    return grid;
}

void read_cluster (Cluster input) {

    cout << "[" << input.head << "]" << " ";
    for (int i = 0; i < N; ++i) {
        //cout << (int)input.sky[i] << " ";
        cout << *input.sky[i] << " ";
    }
    cout << "[" << input.tail << "]" << "\n";
}
void read_vector (vector<vector<int>> actual) {

    for (vector<int> index : actual) {
        for (int val : index)
            cout << val;

        cout << endl;
    }
}

void display (Board *now) {

  cout << " ";
  for (int x = 0; x < N; ++x)
      cout << " " << (int)now->col[x].head <<" ";

  cout << "\n";

  for (int x = 0; x < N; ++x) {
      cout << (int)now->row[x].head;
      for (int y = 0; y < N; ++y) {
          if (*now->row[x].sky[y])
              cout <<  "[" << *now->row[x].sky[y] << "]";
          else
              cout << ("[ ]");
      }
      cout << (int)now->row[x].tail;
      cout << ("\n");
  }
  cout << " ";

  for (int x = 0; x < N; ++x)
      cout << " " <<  (int)now->col[x].tail << " ";
  cout << ("\n");
}

int equal (int **puzzle, int expected[4][4]) {
    if (!puzzle || !expected) return 0;
    for (int i = 0; i < 4; ++i)
        if (memcmp (puzzle[i], expected[i], 4*sizeof (int))) return 0;
    return 1;
}
void Test () {
    equal (SolvePuzzle (clues[0]), outcomes[0]);
    equal (SolvePuzzle (clues[1]), outcomes[1]);
}
