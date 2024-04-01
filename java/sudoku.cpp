#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;
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

  for (int y = 0; y < 81; y++) {
      if (y != 0) {
        if (y % (9 * 3) == 0) os += '\n';
        if (y % 9 == 0) os += '\n';
      }

      os += (board[y] ? to_string (board[y]) : ".") + " ";
      if (y % 3 == 2) os += ' ';
  }
  os += "\n-------------------\n";

  return os;
}
////////////////////////////////////////////////////////////////////////////////

class bit {
    public :
        static int count (int mask) {
            int cnt = 0;

            do {
                cnt += mask &1;
            } while (mask >>= 1);

            return cnt;
        }
        static int get (int mask) {
            switch (mask) {
              case 2 : return 1; break;
              case 4 : return 2; break;
              case 8 : return 3; break;
              case 16 : return 4; break;
              case 32 : return 5; break;
              case 64 : return 6; break;
              case 128 : return 7; break;
              case 256 : return 8; break;
              case 512 : return 9; break;
            }
            return 0;
        }

        static void flip (int &mask, int bit) { mask ^= 1 << bit; }
        static bool exist (int &mask, int bit) { return mask >> bit &1; }
        static std::string show (int mask) {
            std::string os;
            for (int i = 1; i < 10; i++) {
                os += to_string(mask >> i&1 );
            }
            return os;
        }
        static bool is_pow_of_2 (int mask) {
            return (mask & -mask) == mask;
        }
};

class Sudoku {
    private:
        enum level_e {beginner,easy,medium,hard,hell};
        // const std::vector<std::string> difficulty = {"beginner", "easy", "medium", "hard", "hell"};
        static const int N = 9;
        static int level;
        ///////////////////////////solver////////////////////////
        bool is_valid (const std::vector<int> &board) {

            int col[10] = {}, row[10] = {}, sub[10] = {};

            for (size_t i = 0; i < board.size(); i++) {
                int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;
                if (board[i] == 0) return false;
                col[x] += board[i], row[y] += board[i], sub[z] += board[i];
            }

            for (int i = 0; i < N; i++) {
                if (col[i] != 45 || row[i] != 45 || sub[i] != 45) return false;
            }

            return true;
        }

        static int nconflict (int col, int row, int sub) {
            int mask = 0;

            for (int j = 1; j <= 9; j++) {
                bool ex = col &1 << j, ey = row &1 << j, bl = sub &1 << j;

                if (!ex && !ey && !bl)
                    mask |= 1 << j;
            }

            return mask;
        }

        static bool backtrack (std::vector<int> &board, int col[], int row[], int sub[], std::vector<std::pair<int,int>> &tape, int it) {

            if (it == -1) return true;
            int x,y,z;

            for (auto &[mask, pos] : tape) {
                x = pos % N, y = pos / N, z = y / 3 * 3 + x / 3;
                mask = nconflict (col[x], row[y], sub[z]);
            }

            std::sort (&tape[0], &tape[it], [](const std::pair<int,int> &a, const std::pair<int,int> &b) {
                    return bit::count (a.first) > bit::count (b.first);
                    });

            int index = tape[it].second;
            x = index % N, y = index / N, z = y / 3 * 3 + x / 3;

            for (int dig = 1; dig <= N; dig++) {

                if (!bit::exist (col[x], dig) && !bit::exist (row[y], dig) && !bit::exist (sub[z], dig)) {
                    col[x] ^= 1 << dig, row[y] ^= 1 << dig, sub[z] ^= 1 << dig;
                    board[index] = dig;

                    if (backtrack (board, col, row, sub, tape, it - 1))
                        return true;

                    board[index] = 0;
                    col[x] ^= 1 << dig, row[y] ^= 1 << dig, sub[z] ^= 1 << dig;
                }
            }

            return false;
        }
        /////////////////////////////////////////////////////////
        static void simulated_annealing (std::vector<int> &board, int col[], int row[], int sub[], std::vector<std::pair<int,int>> &tape) {
            random_device rd;
            mt19937 gen(rd());

            for (auto &[mask, pos] : tape) {
                int x = pos % N, y = pos / N, z = y / 3 * 3 + x / 3;
                mask = nconflict (col[x], row[y], sub[z]);

                if (bit::is_pow_of_2(mask)) {
                    board[pos] = bit::get(mask);
                } else {
                    std::vector<int> candidate;

                    for (int dig = 1; dig <= N; dig++) {
                        if (bit::exist (mask, dig)) {
                            candidate.push_back(dig);
                        }
                    }

                    uniform_int_distribution<> rng (0, candidate.size() - 1);
                    int dig = rng(gen);
                    
                    board[pos] = candidate[dig];
                    col[x] ^= 1 << dig, row[y] ^= 1 << dig, sub[z] ^= 1 << dig;

                }
            }

        }
        static std::vector<int> create () {
            std::vector<int> board (9 * 9), pos (9 * 9);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist (0, board.size() - 1), digits (1, 9);

            int ncell = 10;
            int col[10] = {}, row[10] = {}, sub[10] = {};

            while (ncell-->0) {
                int pos = dist(gen), dig = digits(gen);
                int x = pos % 9, y = pos / 9, z = y / 3 * 3 + x / 3;

                if (!(col[x] &1 << dig) && !(row[y] &1 << dig) && !(sub[z] &1 << dig)) {
                    board[pos] = dig;
                    col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
                }
            }

            board = solver2 (board);
            std::generate (pos.begin(), pos.end(), [i = 0] () mutable { return i++;});
            std::shuffle (pos.begin(), pos.end(), rd);

            switch (level) {
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
    public:

        static std::vector<int> solver1 (std::vector<int> board) {

            int col[10] = {}, row[10] = {}, sub[10] = {};

            for (size_t i = 0; i < board.size(); i++) {
                int dig = board[i];
                int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;

                if (dig != 0) {
                    col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
                }
            }

            std::vector<std::pair<int,int>> hist;

            for (size_t i = 0; i < board.size(); i++) {
                if (board[i] == 0) {
                    int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;
                    int mask = nconflict (col[x], row[y], sub[z]);

                    if (!bit::is_pow_of_2(mask)) {
                    // if (bit::count (mask) > 1) {
                        hist.push_back({mask, i});
                    } else {
                        int dig = bit::get (mask);
                        board[i] = dig;
                        col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
                    }
                }
            }

            std::sort (&hist[0], &hist[hist.size()], [](const std::pair<int,int> &a, const std::pair<int,int> &b) {
                return bit::count (a.first) > bit::count (b.first);
            });

            // backtrack (board, col, row, sub, hist, hist.size() - 1);
            simulated_annealing (board, col, row, sub, hist);

            return board;
        }
        static std::vector<int> solver2 (std::vector<int> board) {
            bool flag = true;
            int col[10] = {}, row[10] = {}, sub[10] = {};
            std::vector<std::pair<int,int>> hist;

            for (size_t i = 0; i < board.size(); i++) {
                int dig = board[i];
                int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;

                if (dig != 0) {
                    col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
                }
            }

            while (flag) {
                flag = false;

                for (size_t i = 0; i < board.size(); i++) {
                    if (board[i] == 0) {
                        int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;
                        int mask = nconflict (col[x], row[y], sub[z]);
                        int dig = bit::get (mask);

                        if (dig != 0) {
                            board[i] = dig;
                            col[x] |= 1 << dig, row[y] |= 1 << dig, sub[z] |= 1 << dig;
                            flag = true;
                        }
                    }
                }
            }

            for (size_t i = 0; i < board.size(); i++) {
                if (board[i] == 0) {
                    int x = i % N, y = i / N, z = y / 3 * 3 + x / 3;
                    int mask = nconflict (col[x], row[y], sub[z]);
                    hist.push_back({mask,i});
                }
            }

            std::sort (&hist[0], &hist[hist.size()], [](const std::pair<int,int> &a, const std::pair<int,int> &b) {
                return bit::count (a.first) > bit::count (b.first);
            });

            // backtrack (board, col, row, sub, hist, hist.size() - 1);
            simulated_annealing (board, col, row, sub, hist);
            return board;
        }
};

vector<int> convert (vector<vector<int>> input) {
    vector<int> board;

    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            board.push_back (input[y][x]);
        }
    }
    return board;
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

        grid = // multiple solutions ?
        {  {4, 0, 5, 0, 1, 0, 7, 0, 8},
            {0, 0, 7, 0, 0, 5, 0, 0, 0},
            {0, 3, 0, 7, 0, 0, 0, 5, 0},
            {0, 0, 3, 0, 0, 0, 0, 0, 5},
            {0, 4, 0, 2, 0, 8, 0, 6, 0},
            {5, 0, 0, 0, 0, 0, 1, 0, 0},
            {0, 7, 0, 0, 2, 3, 0, 1, 0},
            {0, 0, 0, 4, 0, 0, 2, 0, 0},
            {9, 0, 6, 0, 7, 0, 4, 0, 3}
        };

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

    vector<int> board = Sudoku::solver1 (convert(grid));
    // vector<int> board = solver (convert(grid));

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
