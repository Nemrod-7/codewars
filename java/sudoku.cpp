#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
const int N = 9;
enum {beginner,easy,medium,hard,hell};
////////////////////////////////////////////////////////////////////////////////
class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t cycle;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }
    public :

        Timer() {
            alpha = chrono::steady_clock::now ();
            cycle = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            std::cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << std::endl;
        }
        bool running (double total) {
            update();
            cycle++;
            if (elapsed.count() < total) return true;
            cout << "cycles :: " ;

            string num = to_string (cycle);
            int size = num.size(), index = 0;

            while (size-->0) {
                cout << num[index++];
                if (size % 3 == 0) cout << ' ';
            }
            cout << endl;
            return false;
        }
};
string display (vector<int> board) {

  string os;

  for (int y = 0; y < N * N; y++) {
      if (y != 0) {
        if (y % (N * 3) == 0) os += '\n';
        if (y % N == 0) os += '\n';
      }

      os += (board[y] ? to_string (board[y]) : ".") + " ";
      if (y % 3 == 2) os += ' ';
  }
  os += "\n-------------------\n";

  return os;
}
////////////////////////////////////////////////////////////////////////////////

void flip (int &mask, int bit) { mask ^= 1 << bit; }
bool exist (int &mask, int bit) { return mask >> bit &1; }
int bitcnt (int mask) {
    int cnt = 0;

    do {
        cnt += mask &1;
    } while (mask >>= 1);

    return cnt;
}
int getbit (int mask) {
    for (int i = 1; i <= N; i++)
        if (mask >> i &1) return i;

    return 0;
}

int nconflict (int col, int row, int sub) {
    int mask = 0;

    for (int j = 1; j <= N; j++) {
        bool ex = col &1 << j, ey = row &1 << j, bl = sub &1 << j;

        if (!ex && !ey && !bl)
            mask |= 1 << j;
    }

    return mask;
}

bool backtrack (vector<int> &board, int col[], int row[], int sub[], vector<pair<int,int>> &tape, int end) {

    if (end == -1) return true;
    int x,y,z;

    for (int i = 0; i < end; i++) {
        auto &[mask, pos] = tape[i];
        x = pos % N, y = pos / N, z = y / 3 * 3 + x / 3;
        mask = nconflict (col[x], row[y], sub[z]);
    }

    sort (&tape[0], &tape[end], [](const pair<int,int> &a, const pair<int,int> &b) {
            return bitcnt (a.first) > bitcnt (b.first);
            });

    int index = tape[end].second;
    x = index % N, y = index / N, z = y / 3 * 3 + x / 3;

    for (int dig = 1; dig <= N; dig++) {

        if (!exist (col[x], dig) && !exist (row[y], dig) && !exist (sub[z], dig)) {
            col[x] ^= 1 << dig, row[y] ^= 1 << dig, sub[z] ^= 1 << dig;
            board[index] = dig;

            if (backtrack (board, col, row, sub, tape, end - 1))
                return true;

            board[index] = 0;
            col[x] ^= 1 << dig, row[y] ^= 1 << dig, sub[z] ^= 1 << dig;
        }
    }

    return false;
}
vector<int> solver (vector<int> board) {

    int col[10] = {}, row[10] = {}, sub[10] = {};

    for (int i = 0; i < board.size(); i++) {
        int dig = board[i];
        int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;

        if (dig != 0) {
            col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
        }
    }

    vector<pair<int,int>> hist;

    for (int i = 0; i < board.size(); i++) {
        if (board[i] == 0) {
            int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;
            int mask = nconflict (col[x], row[y], sub[z]);

            if (bitcnt (mask) > 1) {
                hist.push_back({mask, i});
            } else {
                int dig = getbit (mask);

                board[i] = dig;
                col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
            }
        }
    }

    sort (&hist[0], &hist[hist.size()], [](const pair<int,int> &a, const pair<int,int> &b) {
            return bitcnt (a.first) > bitcnt (b.first);
            });

    backtrack (board, col, row, sub, hist, hist.size() - 1);

    return board;
}

vector<int> generate (int lvl) {

    vector<int> board (N * N), pos (N * N);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist (0, board.size() - 1), digits (1, 9);

    int ncell = 10;
    int col[10] = {}, row[10] = {}, sub[10] = {};

    while (ncell-->0) {
        int pos = dist(gen), dig = digits(gen);
        int x = pos % N, y = pos / N, z = y / 3 * 3 + x / 3;

        if (!(col[x] &1 << dig) && !(row[y] &1 << dig) && !(sub[z] &1 << dig)) {
            board[pos] = dig;
            col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
        }
    }

    board = solver (board);
    generate (pos.begin(), pos.end(), [i = 0] () mutable { return i++;});

    shuffle (pos.begin(), pos.end(), rd);

    switch (lvl) {
        case beginner : ncell = 30; break;
        case easy     : ncell = 40; break;
        case medium   : ncell = 50; break;
        case hard     : ncell = 60; break;
        case hell     : ncell = 70; break;
    }

    while (ncell-->0)
        board[pos[ncell]] = 0;

    return board;
}
vector<int> convert (vector<vector<int>> input) {
    vector<int> board;

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            board.push_back (input[y][x]);
        }
    }
    return board;
}

bool is_valid (const vector<int> &board) {

    int col[N] = {}, row[N] = {}, sub[N] = {};

    for (int i = 0; i < board.size(); i++) {
        int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;
        if (board[i] == 0) return false;
        col[x] += board[i], row[y] += board[i], sub[z] += board[i];
    }

    for (int i = 0; i < N; i++) {
        if (col[i] != 45 || row[i] != 45 || sub[i] != 45) return false;
    }

    return true;
}

int main () {

    Timer clock;
    vector<vector<int>> grid;

    grid = {{0, 3, 7, 6, 0, 1, 5, 8, 4},
        {8, 0, 0, 3, 0, 4, 9, 2, 0},
        {6, 0, 9, 2, 5, 0, 3, 7, 1},
        {9, 8, 0, 5, 6, 7, 1, 0, 0},
        {0, 6, 0, 4, 1, 2, 0, 9, 0},
        {0, 0, 1, 8, 3, 9, 0, 6, 5},
        {7, 9, 6, 0, 4, 3, 8, 0, 2},
        {0, 5, 8, 7, 0, 6, 0, 0, 9},
        {1, 2, 4, 9, 0, 5, 6, 3, 0}};


    grid = {{0, 0, 6, 1, 0, 0, 0, 0, 8}, 
        {0, 8, 0, 0, 9, 0, 0, 3, 0}, 
        {2, 0, 0, 0, 0, 5, 4, 0, 0}, 
        {4, 0, 0, 0, 0, 1, 8, 0, 0}, 
        {0, 3, 0, 0, 7, 0, 0, 4, 0}, 
        {0, 0, 7, 9, 0, 0, 0, 0, 3}, 
        {0, 0, 8, 4, 0, 0, 0, 0, 6}, 
        {0, 2, 0, 0, 5, 0, 0, 8, 0}, 
        {1, 0, 0, 0, 0, 2, 5, 0, 0}};

    // grid = // no solution
    // {{0, 0, 0, 0, 0, 5, 0, 8, 0},
    //  {0, 0, 0, 6, 0, 1, 0, 4, 3},
    //  {0, 0, 0, 0, 0, 0, 0, 0, 0},
    //  {0, 1, 0, 5, 0, 0, 0, 0, 0},
    //  {0, 0, 0, 1, 0, 6, 0, 0, 0},
    //  {3, 0, 0, 0, 0, 0, 0, 0, 5},
    //  {5, 3, 0, 0, 0, 0, 0, 6, 1},
    //  {0, 0, 0, 0, 0, 0, 0, 0, 4},
    //  {0, 0, 0, 0, 0, 0, 0, 0, 0}};


    vector<int> board = solver (convert(grid));

    cout << display(board);

    clock.stop();
    clock.get_duration();
}

void test () {
    unsigned int example1[9][9] =
    {{5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}};

    unsigned int example2[9][9] =
    {{5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 0, 3, 4, 8},
        {1, 0, 0, 3, 4, 2, 5, 6, 0},
        {8, 5, 9, 7, 6, 1, 0, 2, 0},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 0, 1, 5, 3, 7, 2, 1, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 0, 0, 4, 8, 1, 1, 7, 9}};

    vector<vector<int>> matrix =
    {{5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}};

    matrix = {
        {6,0,0,0,0,0,0,0,1},
        {0,0,5,6,0,1,2,0,0},
        {0,9,0,0,0,0,0,5,0},
        {0,2,0,0,0,0,0,9,0},
        {3,0,0,0,0,0,0,0,2},
        {0,0,0,2,0,9,0,3,0},
        {0,6,0,0,0,0,0,8,0},
        {0,0,4,8,0,7,6,0,0},
        {8,0,0,0,3,0,0,0,4}};

    matrix = {
        {0,0,0, 0,0,0, 0,0,0},
        {0,0,0, 0,0,3, 0,8,5},
        {0,0,1, 0,2,0, 0,0,0},

        {0,0,0, 5,0,7, 0,0,0},
        {0,0,4, 0,0,0, 1,0,0},
        {0,9,0, 0,0,0, 0,0,0},

        {5,0,0, 0,0,0, 0,7,3},
        {0,0,2, 0,1,0, 0,0,0},
        {0,0,0, 0,4,0, 0,0,9}};

    matrix = {
        {1,5,6, 0,0,0, 0,0,0},
        {0,0,3, 5,0,0, 0,0,0},
        {0,0,0, 0,4,8, 0,7,9},

        {8,7,4, 0,0,0, 0,3,0},
        {2,0,0, 0,0,4, 0,6,0},
        {0,0,0, 0,0,0, 2,0,0},

        {0,0,0, 7,0,0, 5,8,0},
        {0,0,0, 0,3,0, 0,1,0},
        {0,1,0, 2,0,0, 0,0,4}};

    matrix =
    {{8, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 6, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 9, 0, 2, 0, 0},
        {0, 5, 0, 0, 0, 7, 0, 0, 0},
        {0, 0, 0, 0, 4, 5, 7, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 3, 0},
        {0, 0, 1, 0, 0, 0, 0, 6, 8},
        {0, 0, 8, 5, 0, 0, 0, 1, 0},
        {0, 9, 0, 0, 0, 0, 4, 0, 0}};

    matrix = {
        {0, 0, 0, 0, 0, 6, 0, 0, 0},
        {0, 5, 9, 0, 0, 0, 0, 0, 8},
        {2, 0, 0, 0, 0, 8, 0, 0, 0},
        {0, 4, 5, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 0, 6, 0, 0, 3, 0, 5, 4},
        {0, 0, 0, 3, 2, 5, 0, 0, 6},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};
}
