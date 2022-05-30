#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <random>
#include <chrono>
/*------------------------------------------------------------------------------
 7 x 7 skyscraper solution - use of bitmask - adaptive combinations of sets
 use of board ==> vector<vector<int>>
------------------------------------------------------------------------------*/
using namespace std;

random_device rd;
mt19937 gen(rd());
//////////////////////////////////func def//////////////////////////////////////
void Test () ;
////////////////////////////////////////////////////////////////////////////////

int bitpos (int byte) {
    int dig = 0;

    do {
        if (byte &1) return dig;
        dig++;
    } while (byte >>= 1);

    return 0;
}
int bitcnt (int x) { return (x &(x - 1));  }
int bitcnt2 (int byte) {
    int cnt = 0;

    do {
        cnt += (byte &1);
    } while (byte >>= 1);

    return cnt;
}

class Board {
    private :
        int N;
        int setmask (int N, int clue) {
            int dig = N - clue + 2, mask = 0;
            if (dig == N + 1) return mask |= 1 << N;

            while (dig-->1) mask |= 1 << dig;

            return mask;
        }

    public :
        vector<int> clues;
        vector<vector<int>> grid, mask;

        Board (const vector<int> src = {}) {
            N = src.size() / 4;
            clues = src;

            grid.resize (N, vector<int>(N));
            mask.resize (N, vector<int>(N));

            int full = 0, i, j;
            for (i = 1; i <= N; i++) {
                full |= 1 << i;
            }

            for (i = 0; i < N; ++i) {   // make masks for vertical borders
                int west = ((N * 4) - 1) - i, east = N + i;

                if (clues[west] == N) {
                    for (j = 0; j < N; ++j) {
                        mask[i][j] |= 1 << (j + 1);
                    }
                } else if (clues[east] == N) {
                    for (j = 0; j < N; ++j) {
                        mask[i][j] |= 1 << (N - j);
                    }
                } else {
                    mask[i][0]     = clues[west] == 0 ? full : setmask (N, clues[west]);
                    mask[i][N - 1] = clues[east] == 0 ? full : setmask (N, clues[east]);
                }
            }

            for (i = 0; i < N; ++i) {   // make masks for horizontal borders
                int south = ((N * 4) - 1) - i - N, north = i;

                if (clues[north] == N) {
                    for (j = 0; j < N; ++j) {
                        mask[j][i] |= 1 << (j + 1);
                    }
                } else if (clues[south] == N) {
                    for (j = 0; j < N; ++j) {
                        mask[j][i] |= 1 << (N - j);
                    }
                } else {
                    int up  = clues[north] == 0 ? full : setmask (N, clues[north]);
                    int dwn = clues[south] == 0 ? full : setmask (N, clues[south]);

                    if (mask[0][i] == 0 || bitcnt (mask[0][i]) > bitcnt (up))
                        mask[0][i] = up;

                    if (mask[N - 1][i] == 0 || bitcnt (mask[N - 1][i]) > bitcnt (dwn))
                        mask[N - 1][i] = dwn;
                }
            }

            for (int y = 1; y < N - 1; y++) { // make inner masks
                for (int x = 1; x < N - 1; x++) {
                    if (mask[y][x] == 0)
                        mask[y][x] = full;
                }
            }
        }

        pair<int,int> getcluec (int i) {
            int south = ((N * 4) - 1) - i - N, north = i;
            return {clues[north], clues[south]};
        }
        pair<int,int> getcluer (int i) {
            int west = ((N * 4) - 1) - i, east = N + i;
            return {clues[west], clues[east]};
        }

        vector<int*> getcol (int x) {
            vector<int*> line (N);

            for (int y = 0; y < N; y++) {
                line[y] = &mask[y][x];
            }
            return line;
        }
        vector<int*> getrow (int y) {
            vector<int*> line (N);

            for (int x = 0; x < N; x++) {
                line[x] = &mask[y][x];
            }
            return line;
        }
        int size() { return N; }
};

class combinations {
    private :
        vector<int*> line;
        int size;

        void helper (vector<int> curr, int i) {
            int *arr = curr.data();

            for (int j = 0; j < size; j++) {
                int dig = j + 1;

                if (*line[i] &1 << dig) {

                    if (find (&arr[0], &arr[i], dig) != &arr[i]) continue;
                    arr[i] = dig;

                    if (i == size - 1) {
                        combs.push_back (curr);
                    } else {
                        helper (curr, i + 1);
                    }
                }
            }
        }
    public :
        vector<vector<int>> combs;

        combinations (const vector<int*> &data) {
            line = data;
            size = data.size();
            vector<int> arr (data.size());

            helper (arr, 0);
        }
};

class Display {
    public :
        static void board (Board &curr) {
            int i, j, up, N = curr.size();

            cout << " ";
            for (i = 0; i != N; ++i)
                cout << " " << curr.clues[i] << " ";
            cout << endl;

            for (i = 0; i != N; ++i) {
                up = ((N * 4) - 1) - i;

                cout << curr.clues[up];
                for (j = 0; j != N; ++j) {
                    if (curr.grid[i][j])
                        cout <<  "[" << curr.grid[i][j] << "]";
                    else
                        cout << ("[ ]");
                }
                cout << curr.clues[N + i];
                cout << endl;
            }
            cout << " ";

            for (i = 0; i != N; ++i)
                cout << " " << curr.clues[((N * 4) - 1) - i - N] << " ";
            cout << endl;

        }
        static void point (pair<int,int> p) {
            cout << "["<< p.first << "," << p.second << "]\n";
        }
        static void vect (const vector<int> &input) {
            for (int i = 0; i < input.size(); i++)
                cout << input[i] << ',';
            cout << endl;

        }
};

bool equals (const pair<int,int> &a, const pair<int,int> &b) {

    if (b.first == 0 && b.second == 0) {
        return true;
    } else if (b.first == 0 && a.second == b.second) {
        return true;
    } else if (b.second == 0 && a.first == b.first) {
        return true;
    } else if (a == b) {
        return true;
    }

    return false;
}
pair<int,int> check_num (const vector<int> &now) {
    const int N = now.size();
    int end = N - 1, first = 0, sec = 0, index = N;
    pair comb = {0,0};

    while (index-->0) {
        if (now[index] > sec) {
            sec = now[index];
            comb.second++;
        }
        if (now[N - 1 - index] > first) {
            first = now[end - index];
            comb.first++;
        }
    }

    return comb;
}
void searchcomb (vector<int*> line, const pair<int,int> &clue) {
    const int N = line.size();
    if (clue == pair {0,0}) return;
    combinations cmb (line);

    for (int i = 0; i < N; i++)
        *line[i] = 0;

    for (auto &comb : cmb.combs) {
        pair<int,int> curr = check_num (comb);

        if (equals (curr, clue))
            for (int i = 0; i < N; i++) {
                *line[i] |= 1 << comb[i];
            }
    }

}
void reduce (vector<int*> line) {
    int N = line.size(), i;

    vector<int> hist (N + 1);
    vector<bool> uniq (N + 1);

    for (i = 0; i < N; i++) { // find uniq position of number
        int mask = *line[i], dig = 0;

        if ((mask & (mask - 1)) == 0)
            uniq[bitpos (mask)] = true;

        do {
            if (mask &1) hist[dig]++;
            dig++;
        } while (mask >>= 1);
    }

    for (i = 0; i < N; i++) { // adjust mask when uniq position of a number
        int &mask = *line[i];

        if (bitcnt (mask) > 1) { // bitset<8>(mask).count() > 1
            for (int j = 1; j < N + 1; j++) {
                bool exist = (mask &1 << j);

                if (exist) {
                    if (hist[j] == 1) {
                        mask = 0;
                        mask |= 1 << j;
                    }
                    if (uniq[j]) {
                        mask ^= 1 << j;
                    }
                }
            }
        }
    }
}

void reduce2 (Board &city) {

    for (int i = 0; i < city.size(); i++) {
        auto row = city.getrow (i), col = city.getcol(i);
        reduce (row), reduce (col);
    }
}
void upgrade_grid (Board &city) {

    const int N = city.size();
    int loop = 1; // loop = 4

    while (loop-->0) {
        for (int i = 0; i < N; i++) {
            auto row = city.getrow (i), col = city.getcol(i);
            pair vertical = city.getcluec(i), lateral = city.getcluer(i);

            reduce2 (city);
            //searchcomb (row, lateral), searchcomb (col, vertical);
        }
    }

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            int num = city.mask[y][x];
            //cout << bitcnt2 (num) << ' ';
            if ((num &(num - 1)) == 0)  // if is pow of 2 -> on possible number
                city.grid[y][x] = bitpos (num);
        }
        //cout << endl;
    }
}

bool is_valid (Board &city) {

    const int N = city.size();
    vector<int> col (N), row (N);

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {
            row[j] = city.grid[i][j];
            col[j] = city.grid[j][i];
        }

        pair<int,int> horiz = city.getcluer (i), vert = city.getcluec(i);

        if (!equals (check_num (row), horiz) || !equals (check_num (col), vert))
            return false;
    }

    return true;
}
bool checkcol (Board &city, pair<int,int> &p, int num) {

    for (int y = 0; y < city.size(); y++)
        if (city.grid[y][p.first] == num)
            return false;

    return true;
}
bool checkrow (Board &city, pair<int,int> &p, int num) {

    for (int x = 0; x < city.size(); x++)
        if (city.grid[p.second][x] == num)
            return false;

    return true;
}
bool backtrack (Board &city, pair<int,int> p) {

    auto &[x, y] = p;


    if (x == city.size()) {
        x = 0;
        y++;
    }

    if (y == city.size()) {
        return (is_valid(city)) ? true : false;
    }
    if (city.grid[y][x]) return backtrack (city, {x + 1, y});

    for (int dig = 1; dig < city.size() + 1; dig++) {
        if ((city.mask[y][x] &1 << dig) && checkrow (city, p, dig) && checkcol (city, p, dig)) {

            city.grid[y][x] = dig;

            if (backtrack (city, {x + 1, y}))
                return true;
        }
    }

    city.grid[y][x] = 0;

    return false;
}

vector<vector<int>> SolvePuzzle (const vector<int> &clues) {

    Board city (clues);
    upgrade_grid (city);
    //backtrack (city, {0,0});

    Display::board (city);
    return city.grid;
}

int getnum (vector<vector<int>> &grid, int limx, int limy) { // find rnd unused number

    vector<int> hist (grid.size() + 1), rnd;

    for (int i = 0; i < limy; i++) {
        hist[grid[i][limx]] = true;
    }
    for (int i = 0; i < limx; i++) {
        hist[grid[limy][i]] = true;
    }

    for (int i = 1; i < grid.size() + 1; i++) {
        if (!hist[i]) rnd.push_back (i);
    }
    if (rnd.size() == 0) return 0;

    uniform_int_distribution<> dist (0, rnd.size() - 1);
    return rnd[dist(gen)];
}
vector<vector<int>> gen_latin_sq (int size) { // generate a random latin square

    vector<vector<int>> grid (size, vector<int>(size));

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int dig = getnum (grid, x, y);

            if (dig != 0) {
                grid[y][x] = dig;
            } else {
                x = -1;
            }
        }
    }

    return grid;
}
vector<int> gen_puzzle (int size) { // generate random skyskraper clues

    vector<vector<int>> board = gen_latin_sq (size);
    vector<int> clues (size * 4);

    for (int i = 0; i < size; ++i) {
        int west = ((size * 4) - 1) - i, east = size + i;
        int south = west - size, north = i;

        vector<int> line (size), col (size);

        for (int j = 0; j  < size; j++) {
            line[j] = board[i][j], col[j] = board[j][i];
        }
        pair<int,int> horiz = check_num (line);
        pair<int,int> verti = check_num (col);
        clues[west] = horiz.first, clues[east] = horiz.second;
        clues[north] = verti.first, clues[south] = verti.second;
    }

    for (auto &it : clues) {
    //    cout << it << " ";
    }

    return clues;
}

class combinations2 {
    private :
        vector<int*> line;
        vector<vector<int>> subs;
        int size;

        void helper (vector<int> curr, int i) {
            int *arr = curr.data();

            for (int j = 0; j < size; j++) {
                int dig = j + 1;

                if (*line[i] &1 << dig) {

                    if (find (&arr[0], &arr[i], dig) != &arr[i]) continue;
                    arr[i] = dig;

                    if (i == size - 1) {
                        combs.push_back (curr);
                    } else {
                        helper (curr, i + 1);
                    }
                }
            }
        }

        void helper2 (vector<int> curr, int i) {
            int *arr = curr.data();

            for (int j = 0; j < subs[i].size(); j++) {
                int dig = subs[i][j];

                if (find (&arr[0], &arr[i], dig) != &arr[i]) continue;
                arr[i] = dig;

                if (i == size - 1) {
                    combs.push_back (curr);
                } else {
                    helper2 (curr, i + 1);
                }
            }
        }
    public :
        vector<vector<int>> combs;

        combinations2 (const vector<int*> &data) {

            line = data;
            size = data.size();
            vector<int> arr (data.size());

            subs.resize (size);

            for (int i = 0; i < size; i++) {
                for (int j = 1; j < 10; j++) {
                    if (*line[i] &1 << j)
                        subs[i].push_back (j);
                }
            }

            helper2 (arr, 0);
        }
};

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    int size = 9;
    auto vs = gen_puzzle (size);
    vs = {2,2,5,2,2,1,3,4,3,4,2,3,2,2,3,5,4,1,1,4,2,3,3,3,2,4,3,6,3,1,3,3,3,3,2,2};

    Board city (vs);

    upgrade_grid (city);

    auto row = city.getrow (3);

    combinations2 combs (row);
    //searchcomb (row, lateral);

    //SolvePuzzle (vs);
    //SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});

    //SolvePuzzle({0,0,5,0,0,0,6,4,0,0,2,0,2,0,0,5,2,0,0,0,5,0,3,0,5,0,0,3});
    //Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
    return 0;
}

void Test() {

    //4 x 4 skycrapers
    SolvePuzzle({ 2, 2, 1, 3,  2, 2, 3, 1,  1, 2, 2, 3,  3, 2, 1, 3 });
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
    SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});
    SolvePuzzle ({0,0,1,2,0,2,0,0,0,3,0,0,0,1,0,0});
    SolvePuzzle ({1,2,4,2,2,1,3,2,3,1,2,3,3,2,2,1});
    SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});
    SolvePuzzle ({0,2,0,0,0,3,0,0,0,1,0,0,0,0,1,2});
    SolvePuzzle ({2,1,3,2,3,1,2,3,3,2,2,1,1,2,4,2});
    SolvePuzzle ({2,2,3,1,1,2,2,3,3,2,1,3,2,2,1,3});
    SolvePuzzle ({0,2,0,0,0,3,0,0,0,1,0,0,0,0,1,2});
    SolvePuzzle ({3,1,2,3,3,2,2,1,1,2,4,2,2,1,3,2});
    SolvePuzzle ({1,2,2,3,3,2,1,3,2,2,1,3,2,2,3,1});
    SolvePuzzle ({0,3,0,0,0,1,0,0,0,0,1,2,0,2,0,0});
    SolvePuzzle ({3,1,2,3,3,2,2,1,1,2,4,2,2,1,3,2});
    SolvePuzzle ({3,2,1,3,2,2,1,3,2,2,3,1,1,2,2,3});

    // 6 x 6 skycrapers
    SolvePuzzle (clues[0]);
    SolvePuzzle (clues[1]);
    SolvePuzzle (clues[2]);
    SolvePuzzle({0,3,0,5,3,4,0,0,0,0,0,1,0,3,0,3,2,3,3,2,0,3,1,0});
    SolvePuzzle ({0,0,3,0,3,3, 0,4,4,4,0,0, 0,0,5,0,5,0, 0,2,2,0,2,0});
    SolvePuzzle ({0,4,2,0,3,3, 0,0,0,0,3,2, 4,0,0,3,0,5, 4,0,0,3,0,2});
    SolvePuzzle ({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6});

    SolvePuzzle ({3,1,2,3,2,5, 3,2,3,3,2,1, 1,3,3,2,5,3, 3,2,1,3,2,2});

     clues = {
      { 7, 0, 0, 0, 2, 2, 3,
        0, 0, 3, 0, 0, 0, 0,
        3, 0, 3, 0, 0, 5, 0,
        0, 0, 0, 0, 5, 0, 4 },
      { 0, 2, 3, 0, 2, 0, 0,
        5, 0, 4, 5, 0, 4, 0,
        0, 4, 2, 0, 0, 0, 6,
        5, 2, 2, 2, 2, 4, 1 }
    };

     expected = {
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

    for (auto now : clues) SolvePuzzle (now);

      SolvePuzzle({
        2,3,3,4,4,2,1,
        1,6,4,4,2,2,2,
        2,2,3,3,1,3,3,
        2,5,3,3,2,1,5});

      SolvePuzzle({
        2,2,0,2,0,2,0,
        0,3,3,0,3,3,0,
        5,4,6,3,0,0,0,
        0,0,4,4,0,0,4
      });

      SolvePuzzle({0,0,5,0,0,0,6,4,0,0,2,0,2,0,0,5,2,0,0,0,5,0,3,0,5,0,0,3});
      SolvePuzzle({0,0,5,3,0,2,0,0,0,0,4,5,0,0,0,0,0,3,2,5,4,2,2,0,0,0,0,5});
      SolvePuzzle({0,2,3,0,2,0,0,5,0,4,5,0,4,0,0,4,2,0,0,0,6,5,2,2,2,2,4,1});
      SolvePuzzle({0,2,3,0,2,0,0,5,0,4,5,0,4,0,0,4,2,0,0,0,6,0,0,0,0,0,0,0});
      SolvePuzzle({7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0,0,0,0,0,5,0,4});
      SolvePuzzle({0,0,0,0,5,0,4,7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0});
      SolvePuzzle({0,0,0,0,5,0,4,7,0,0,0,2,2,3,0,0,3,0,0,0,0,3,0,3,0,0,5,0});

      SolvePuzzle ({2,3,3,3,2,1, 1,4,2,2,3,5, 4,3,2,2,1,5, 2,3,2,3,1,2});
      SolvePuzzle ({2,3,3,1,2,6, 3,4,2,2,2,1, 1,3,5,2,2,3, 4,2,2,3,1,2});
      SolvePuzzle({3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});

      // 8 x 8
      SolvePuzzle({2,1,2,4,5,3,6,4, 3,6,3,2,2,1,4,2, 2,1,2,4,2,2,3,3,4,2,5,4,4,2,1,2});
}
///////////////////////////////////////////////////////////////////////////////
/*

*/
