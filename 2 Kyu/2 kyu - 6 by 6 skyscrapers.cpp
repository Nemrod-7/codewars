#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include <chrono>

#define N 6

using namespace std;

typedef struct _point {
    int x;
    int y;
} Point;
typedef struct _cluster {
    int head;
    int tail;
    int *sky[N];
    list<vector<int>> root;
} Cluster;
typedef struct _board {
    vector<Cluster> row;
    vector<Cluster> col;
} Board;

//////////////////////////////////func def//////////////////////////////////////
vector<vector<int>> SolvePuzzle (vector<int> clues);

void read_cluster (Cluster input) ;
void read_vector (vector<vector<int>> input);
void display (Board *now);

void Test ();
////////////////////////////////////////////////////////////////////////////////
static vector<vector<int>> clues = {
  { 3, 2, 2, 3, 2, 1,
    1, 2, 3, 3, 2, 2,
    5, 1, 2, 2, 4, 3,
    3, 2, 1, 2, 2, 4 },
  { 0, 0, 0, 2, 2, 0,
    0, 0, 0, 6, 3, 0,
    0, 4, 0, 0, 0, 0,
    4, 4, 0, 3, 0, 0 },
  { 0, 3, 0, 5, 3, 4,
    0, 0, 0, 0, 0, 1,
    0, 3, 0, 3, 2, 3,
    3, 2, 0, 3, 1, 0}
};
static vector<vector<vector<int>>> expected ={
    {{ 2, 1, 4, 3, 5, 6},
     { 1, 6, 3, 2, 4, 5},
     { 4, 3, 6, 5, 1, 2},
     { 6, 5, 2, 1, 3, 4},
     { 5, 4, 1, 6, 2, 3},
     { 3, 2, 5, 4, 6, 1 }},
    {{ 5, 6, 1, 4, 3, 2 },
     { 4, 1, 3, 2, 6, 5 },
     { 2, 3, 6, 1, 5, 4 },
     { 6, 5, 4, 3, 2, 1 },
     { 1, 2, 5, 6, 4, 3 },
     { 3, 4, 2, 5, 1, 6 }},
    {{ 5, 2, 6, 1, 4, 3 },
     { 6, 4, 3, 2, 5, 1 },
     { 3, 1, 5, 4, 6, 2 },
     { 2, 6, 1, 5, 3, 4 },
     { 4, 3, 2, 6, 1, 5 },
     { 1, 5, 4, 3, 2, 6 }}
};
////////////////////////////////////////////////////////////////////////////////

int main () {

    auto start = chrono::high_resolution_clock::now();

    //SolvePuzzle (clues[0]);
    Test();
    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;

    return 0;
}

vector<vector<int>> init_grid() {
    vector<vector<int>> grid;
    for (int i = 0; i < N; ++i)
        grid.push_back(vector<int>(N));

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

    maxval = 0, index = N;
    while (index-->0) {
       if (data[index] > maxval)
          chk.tail++;

       maxval = max (maxval, data[index]);
    }

    return chk;
}
void search_comb (Cluster *now) {

    vector<int> num{1,2,3,4,5,6};

    do {
        Cluster comb = check_num (num);

        if (now->head != 0 && now->tail != 0) {
            if ((comb.head == now->head) && (comb.tail == now->tail))
                  now->root.emplace_back (num);
        }
        else if (now->head != 0 && comb.head == now->head)
                  now->root.emplace_back (num);

        else if (now->tail != 0 && comb.tail == now->tail)
                  now->root.emplace_back (num);

        else if (now->tail == 0 && now->head == 0 )
                  now->root.emplace_back (num);

    } while (next_permutation(num.begin(), num.end()));
}
Board *init (vector<vector<int>> &grid, vector<int> clues) {

    Board *next = new Board;

    for (int i = 0; i < N; i++) {
        Point up = {.x = ((N * 4) - 1) - i , .y = i }, bt = {.x = N + i , .y = up.x - N};

        next->row.push_back ({.head = clues[up.x], .tail = clues[bt.x]});
        next->col.push_back ({.head = clues[up.y], .tail = clues[bt.y]});

        for (int j = 0; j < N; j++) {
            next->row[i].sky[j] = &grid[i][j];
            next->col[i].sky[j] = &grid[j][i];
        }
        search_comb (&next->row[i]);
        search_comb (&next->col[i]);
    }

    return next;
}
void update (Cluster *input, list<vector<int>> *combs, Point p) {

    int i, now[N] = {0}, *entry = *input->sky;
    list<vector<int>>::iterator index = input->root.begin();
    bool flag[N], match;

    fill_n (flag, N, 1);

    while (index != input->root.end()) {

        vector<int>::iterator curr = index->begin();
        match = false;

        for (vector<int> &next : *combs)
            if (curr[p.x] == next[p.y]) {
                match = true;
                break;
            }

        if (match == false)
            index = input->root.erase(index);
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
bool growing (vector<vector<int>> grid) {

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
vector<vector<int>> SolvePuzzle (const vector<int> clues) {

    vector<vector<int>> grid = init_grid();
    Board *actual = init(grid,clues);
    int index = 8;

    while (growing (grid))
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                Point p = {.x = j, .y = i};
                update (&actual->row[i], &actual->col[j].root, p);
                update (&actual->col[i], &actual->row[j].root, p);
            }

    display (actual);

    return grid;
}
/////////////////////////////tools////////////////////////////////////////////////
void read_cluster (Cluster input) {

    cout << "[" << input.head << "]" << " ";
    for (int i = 0; i < N; ++i) {
        //cout << (int)input.sky[i] << " ";
        cout << *input.sky[i] << " ";
    }
    cout << "[" << input.tail << "]" << "\n";
}
void read_vector (list<vector<int>> actual) {

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

int equal (vector<vector<int>> puzzle, vector<vector<int>> expected) {

}
void Test () {
    SolvePuzzle (clues[0]);
    SolvePuzzle (clues[1]);
    SolvePuzzle (clues[2]);
}
