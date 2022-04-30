#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>
#include <algorithm>

#include <chrono>

using namespace std;
//////////////////////////////////func def//////////////////////////////////////
void Test () ;

////////////////////////////////////////////////////////////////////////////////
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

class Skyscraper {
    private :
        vector<int> fill_c (int N, int clue) {
            int num = N - clue + 2;
            if (num == N + 1) return { N };

            vector<int> dig;

            while (num-->1)
                dig.push_back (num);

            return dig;
        }
        int setcell (int N, int clue) {
            int dig = N - clue + 2, cell = 0;
            if (dig == N + 1) return cell |= 1 << N;

            while (dig-->1) cell |= 1 << dig;

            return cell;
        }

    public :
        int N;
        vector<int> clues, grid, cell;

        Skyscraper (const vector<int> src = {}) {
            clues = src;
            N = src.size() / 4;

            cell.resize (N * N);
            grid.resize (N * N);

            int full = 0;
            for (int i = 1; i <= N; i++) {
                full |= 1 << i;
            }

            for (int i = 0; i < N; ++i) {   // make cells for vertical borders
                int west = ((N * 4) - 1) - i, east = N + i;

                if (clues[west] == N) {
                    for (int j = 0; j < N; ++j) {
                        cell[i * N + j] |= 1 << (j + 1);
                    }
                } else if (clues[east] == N) {
                    for (int j = 0; j < N; ++j) {
                        cell[i * N + j] |= 1 << (N - j);
                    }
                } else {
                    cell[i * N + 0]     = clues[west] == 0 ? full : setcell (N, clues[west]);
                    cell[i * N + (N - 1)] = clues[east] == 0 ? full : setcell (N, clues[east]);
                }
            }

            for (int i = 0; i < N; ++i) {   // make cells for horizontal borders
                int south = ((N * 4) - 1) - i - N, north = i;

                if (clues[north] == N) {
                    for (int j = 0; j < N; ++j) {
                        cell[j * N + i] |= 1 << (j + 1);
                    }
                } else if (clues[south] == N) {
                    for (int j = 0; j < N; ++j) {
                        cell[j * N + i] |= 1 << (N - j);
                    }
                } else {
                    int up  = clues[north] == 0 ? full : setcell (N, clues[north]);
                    int dwn = clues[south] == 0 ? full : setcell (N, clues[south]);

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
                line[y] = &cell[y * N + x];
            }
            return line;
        }
        vector<int*> getrow (int y) {
            vector<int*> line (N);

            for (int x = 0; x < N; x++) {
                line[x] = &cell[y * N + x];
            }
            return line;
        }
        int index (pair<int,int> p) {
            return p.second * N + p.first;
        }
};
class combinations {
    private :
        vector<int*> line;

        void helper (vector<int> data, int i) {
            int *arr = data.data();

            for (int j = 0; j < line.size(); j++) {
                int dig = j + 1;
                if (*line[i] &1 << dig) {

                    if (find (&arr[0], &arr[i], dig) != &arr[i]) continue;
                    arr[i] = dig;

                    if (i == line.size() - 1) {
                        combs.push_back (data);
                    } else {
                        helper (data, i + 1);
                    }
                }
            }
        }
    public :
        vector<vector<int>> combs;
        combinations (const vector<int*> &data) {
            line = data;
            vector<int> arr (data.size());

            helper (arr, 0);
        }
};

class Display {
    public :
        static void board (Skyscraper &curr) {
            int N = curr.N, i, j, up;

            cout << " ";
            for (i = 0; i != N; ++i)
                cout << " " << curr.clues[i] << " ";
            cout << endl;

            for (i = 0; i != N; ++i) {
                up = ((N * 4) - 1) - i;

                cout << curr.clues[up];
                for (j = 0; j != N; ++j) {
                    if (curr.grid[i * N + j])
                        cout <<  "[" << curr.grid[i * N + j] << "]";
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

        static void read_clust (const vector<int> &input) {
            for (int i = 0; i < input.size(); i++)
                cout << input[i];
            cout << endl;

        }
};

pair<int,int> check_num (vector<int> &now) {
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
    //Display::point (a);
    return false;
}
void reduce (vector<int*> line) {
    int N = line.size(), i;

    int hist[N + 1] = {};
    bool uniq[N + 1] = {};

    for (i = 0; i < N; i++) {
        int mask = *line[i], dig = 0;
        if ((mask & (mask - 1)) == 0)
            uniq[bit2int (mask)] = true;

        do {
            if (mask &1) hist[dig]++;
            dig++;
        } while (mask >>= 1);
    }

    for (i = 0; i < N; i++) {
        int &curr = *line[i];

        if (bitcnt (curr) > 1) {
            for (int j = 1; j < N + 1; j++) {
                bool exist = (curr &1 << j);

                if (exist) {
                    if (hist[j] == 1) {
                        curr = 0;
                        curr |= 1 << j;
                    }
                    if (uniq[j]) {
                        curr ^= 1 << j;
                    }
                }
            }
        }
    }
}
void filter (vector<int*> input, const pair<int,int> &clue) {

    if (clue == pair {0,0}) return;
    combinations line (input);

    for (int i = 0; i < input.size(); i++) {
        *input[i] = 0;
    }

    for (auto &comb : line.combs) {
        pair<int,int> curr = check_num (comb);

        if (equals (curr, clue)) {
            for (int i = 0; i < input.size(); i++) {
                *input[i] |= 1 << comb[i];
            }
        }
    }
}
void reduce2 (Skyscraper &city) {
    const int N = city.N;

    for (int i = 0; i < N; i++) {
        auto row = city.getrow (i), col = city.getcol(i);
        reduce (row), reduce (col);
    }
}

void upgrade_grid (Skyscraper &city) {
    const int N = city.N;
    int loop = 4;

    while (loop-->0) {
        for (int i = 0; i < N; i++) {
            auto row = city.getrow (i), col = city.getcol(i);
            pair vertical = city.getcluec(i), lateral = city.getcluer(i);

            reduce2 (city);
            filter (row, lateral), filter (col, vertical);
        }
    }

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            int num = city.cell[y * N + x];

            if ((num & (num - 1)) == 0) {      // if is pow of 2 => one solution for this cell
                city.grid[y * N + x] = bit2int (num);
            }
        }
    }
}

bool is_valid (Skyscraper &city) {

    const int N = city.N;
    vector<int> col (N), row (N);

    for (int i = 0; i < N; i++) {
        int west = ((N * 4) - 1) - i, east = N + i, south = west - N, north = i;
        pair<int,int> horiz {city.clues[west], city.clues[east]}, vert {city.clues[north], city.clues[south]};

        for (int j = 0; j < N; j++) {
            row[j] = city.grid[i * N + j];
            col[j] = city.grid[j * N + i];
        }

        if (!equals (check_num (row), horiz) || !equals (check_num (col), vert))
            return false;
    }

    return true;
}

bool checkcol (Skyscraper &city, pair<int,int> &p, int num) {

    for (int y = 0; y < city.N; y++) {
        int index = y * city.N + p.first;
        if (y != p.second && city.grid[index] == num)
            return false;
    }

    return true;
}
bool checkrow (Skyscraper &city, pair<int,int> &p, int num) {

    for (int x = 0; x < city.N; x++) {
        int index = p.second * city.N + x;
        if (x != p.first && city.grid[index] == num)
            return false;
    }

    return true;
}
bool backtrack (Skyscraper &city, pair<int,int> p) {

    auto &[x, y] = p;
    int index = y * city.N + x;
    if (x == city.N) {
        x = 0;
        y++;
    }

    if (y == city.N) {
        return (is_valid (city)) ? true : false;
    }

    if (city.grid[index]) return backtrack (city, {x + 1, y});

    for (int dig = 1; dig < city.N + 1; dig++) {

        if (city.cell[index] &1 << dig) {
            city.grid[index] = dig;

            if (checkrow (city, p, dig) && checkcol (city, p, dig)) {

                  if (backtrack (city, {x + 1, y}))
                      return true;
            }
        }
    }

    city.grid[index] = 0;

    return false;
}

vector<vector<int>> SolvePuzzle (const vector<int> &clues) {

    Skyscraper city (clues);

    //upgrade_grid(city);
    auto row = city.getrow (0);
    pair lateral = city.getcluer(0);
    reduce2 (city);

    combinations line (row);

    //filter (row, lateral);
    //backtrack(city, {0,0});
    /*
    for (int y = 0; y != city.N; ++y) {
        for (int x = 0; x != city.N ; ++x) {
            cout << bitcnt(city.cell[y][x]) << " ";
        }
        cout << endl;
    }
    */
    //Display::board (city);


    return {};
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    SolvePuzzle ({2,2,1,3,2,2,3,1,1,2,2,3,3,2,1,3});

    //SolvePuzzle({0,0,5,0,0,0,6,4,0,0,2,0,2,0,0,5,2,0,0,0,5,0,3,0,5,0,0,3});
    //Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
    return 0;
}

void Test() {

  /*
  */

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
      // no solutions

      SolvePuzzle ({2,3,3,3,2,1, 1,4,2,2,3,5, 4,3,2,2,1,5, 2,3,2,3,1,2});
      SolvePuzzle ({2,3,3,1,2,6, 3,4,2,2,2,1, 1,3,5,2,2,3, 4,2,2,3,1,2});
      SolvePuzzle({3,3,2,1,2,2,3,4,3,2,4,1,4,2,2,4,1,4,5,3,2,3,1,4,2,5,2,3});

}
///////////////////////////////////////////////////////////////////////////////
