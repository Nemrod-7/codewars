#include <vector>
#include <algorithm>

using namespace std;

int bitpos (int byte) {
    int dig = 0;

    do {
        if (byte &1) return dig;
        dig++;
    } while (byte >>= 1);

    return 0;
}
int bitcnt (int x) { return (x & (x - 1));  }

class Board {
    private :
        int setcell (int N, int clue) {
            int dig = N - clue + 2, cell = 0;
            if (dig == N + 1) return cell |= 1 << N;

            while (dig-->1) cell |= 1 << dig;

            return cell;
        }

    public :
        int N;
        vector<int> clues;
        vector<vector<int>> grid;
        vector<vector<int>> cell;

        Board (const vector<int> src = {}) {
            N = src.size() / 4;
            clues = src;

            grid.resize (N, vector<int>(N));
            cell.resize (N, vector<int>(N));

            int full = 0, i, j;
            for (i = 1; i <= N; i++) {
                full |= 1 << i;
            }

            for (i = 0; i < N; ++i) {   // make cells for vertical borders
                int west = ((N * 4) - 1) - i, east = N + i;

                if (clues[west] == N) {
                    for (j = 0; j < N; ++j) {
                        cell[i][j] |= 1 << (j + 1);
                    }
                } else if (clues[east] == N) {
                    for (j = 0; j < N; ++j) {
                        cell[i][j] |= 1 << (N - j);
                    }
                } else {
                    cell[i][0]     = clues[west] == 0 ? full : setcell (N, clues[west]);
                    cell[i][N - 1] = clues[east] == 0 ? full : setcell (N, clues[east]);
                }
            }

            for (i = 0; i < N; ++i) {   // make cells for horizontal borders
                int south = ((N * 4) - 1) - i - N, north = i;

                if (clues[north] == N) {
                    for (j = 0; j < N; ++j) {
                        cell[j][i] |= 1 << (j + 1);
                    }
                } else if (clues[south] == N) {
                    for (j = 0; j < N; ++j) {
                        cell[j][i] |= 1 << (N - j);
                    }
                } else {
                    int up  = clues[north] == 0 ? full : setcell (N, clues[north]);
                    int dwn = clues[south] == 0 ? full : setcell (N, clues[south]);

                    if (cell[0][i] == 0 || bitcnt (cell[0][i]) > bitcnt (up))
                        cell[0][i] = up;

                    if (cell[N - 1][i] == 0 || bitcnt (cell[N - 1][i]) > bitcnt (dwn))
                        cell[N - 1][i] = dwn;
                }
            }

            for (int y = 1; y < N - 1; y++) { // make inner cells
                for (int x = 1; x < N - 1; x++) {
                    if (cell[y][x] == 0)
                        cell[y][x] = full;
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
                line[y] = &cell[y][x];
            }
            return line;
        }
        vector<int*> getrow (int y) {
            vector<int*> line (N);

            for (int x = 0; x < N; x++) {
                line[x] = &cell[y][x];
            }
            return line;
        }
};

class combinations {
    private :
        vector<int*> line;
        int size;
        void helper (vector<int> data, int i) {
            int *arr = data.data();

            for (int j = 0; j < size; j++) {
                int dig = j + 1;

                if (*line[i] &1 << dig) {

                    if (find (&arr[0], &arr[i], dig) != &arr[i]) continue;
                    arr[i] = dig;

                    if (i == size - 1) {
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
            size = data.size();
            vector<int> arr (data.size());

            helper (arr, 0);
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

void reduce (vector<int*> line) {
    int N = line.size(), i;

    vector<int> hist (N + 1);
    vector<bool> uniq (N + 1);

    for (i = 0; i < N; i++) { // find uniq position of number
        int cell = *line[i], dig = 0;

        if ((cell & (cell - 1)) == 0)
            uniq[bitpos (cell)] = true;

        do {
            if (cell &1) hist[dig]++;
            dig++;
        } while (cell >>= 1);
    }

    for (i = 0; i < N; i++) { // adjust cell when uniq position of a number
        int &cell = *line[i];
        //bitcnt (cell) == 0
        if (bitcnt (cell) > 1) { // bitset<8>(cell).count() > 1
            for (int j = 1; j < N + 1; j++) {
                bool exist = (cell &1 << j);

                if (exist) {
                    if (hist[j] == 1) {
                        cell = 0;
                        cell |= 1 << j;
                    }
                    if (uniq[j]) {
                        cell ^= 1 << j;
                    }
                }
            }
        }
    }
}
void filter (vector<int*> line, const pair<int,int> &clue) {
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
void reduce2 (Board &city) {

    for (int i = 0; i < city.N; i++) {
        auto row = city.getrow (i), col = city.getcol(i);
        reduce (row), reduce (col);
    }
}
void upgrade_grid (Board &city) {
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
            int num = city.cell[y][x];
            if ((num & (num - 1)) == 0)  // if is pow of 2 -> on possible number
                city.grid[y][x] = bitpos (num);
        }
    }
}

bool is_valid (Board &city) {

    const int N = city.N;
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

    for (int y = 0; y < city.N; y++)
        if (city.grid[y][p.first] == num)
            return false;

    return true;
}
bool checkrow (Board &city, pair<int,int> &p, int num) {

    for (int x = 0; x < city.N; x++)
        if (city.grid[p.second][x] == num)
            return false;

    return true;
}
bool backtrack (Board &city, pair<int,int> p) {

    auto &[x, y] = p;

    if (x == city.N) {
        x = 0;
        y++;
    }

    if (y == city.N) {
        return (is_valid(city)) ? true : false;
    }

    if (city.grid[y][x]) return backtrack (city, {x + 1, y});

    for (int dig = 1; dig < city.N + 1; dig++) {

        if ((city.cell[y][x] &1 << dig) && checkrow (city, p, dig) && checkcol (city, p, dig)) {
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

    upgrade_grid(city);
    backtrack(city, {0,0});

    return city.grid;
}

int main () {
  
}
