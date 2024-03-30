#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

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
            for (int i = 1; i <= 9; i++)
                if (mask >> i &1) return i;

            return 0;
        }

        static void flip (int &mask, int bit) { mask ^= 1 << bit; }
        static bool exist (int &mask, int bit) { return mask >> bit &1; }
};

class Sudoku {
    private:
        enum level_e {beginner,easy,medium,hard,hell};
        // const std::vector<std::string> difficulty = {"beginner", "easy", "medium", "hard", "hell"};
        static const int N = 9;
        static int level;
        ///////////////////////////solver////////////////////////

        static int nconflict (int col, int row, int sub) {
            int mask = 0;

            for (int j = 1; j <= 9; j++) {
                bool ex = col &1 << j, ey = row &1 << j, bl = sub &1 << j;

                if (!ex && !ey && !bl)
                    mask |= 1 << j;
            }

            return mask;
        }
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

            board = solver (board);
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

        static std::vector<int> solver (std::vector<int> board) {

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

                    if (bit::count (mask) > 1) {
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

            backtrack (board, col, row, sub, hist, hist.size() - 1);

            return board;
        }

};

int main () {




}
