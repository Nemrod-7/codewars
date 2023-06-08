#include <iostream>
#include <vector>
#include <limits>

#include <chrono>

using namespace std;

class fibgen {
    private:
        const int64_t mod = 1000000;
        vector<int64_t> seed;
    public:
        fibgen () {
            seed.resize(4000000);
            int64_t *rng = seed.data();
            //vector<vector<int64_t>> board (2000,vector<int64_t>(2000));

            for (int64_t k = 0; k < 56; k++) {
                rng[k] = (100003 - 200003 * k + 300007 * k*k*k) % mod - 500000;
            }

            for (int64_t k = 56; k < 4000000; k++) {
                rng[k] = (rng[k-24] + rng[k-55] + mod) % mod - 500000;
            }
        }

        int64_t operator [] (int64_t k) {
            return seed[k];
        }
};

int64_t kadane (vector<int64_t> arr) {
    int64_t sum = 0, maxv = numeric_limits<int64_t>::min();

    for (int64_t i = 0; i < arr.size(); i++) {
        sum += arr[i];
        if (maxv < sum)
            maxv = sum;

        if (sum < arr[i])
            sum = arr[i];
    }

    return sum;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    // Problem 149 : Searching for a maximum-sum subsequence
    // Finally, find the greatest sum of (any number of) adjacent entries in any direction (horizontal, vertical, diagonal or anti-diagonal).

    fibgen rng;
    vector<vector<int64_t>> board (2000, vector<int64_t> (2000));

    for (int64_t i = 0; i < 4000000; i++) {
        board[i/2000][i%2000] = rng[i];
    }

    board = {
        {-2, 5, 3, 2},
        { 9,-6, 5, 1},
        { 3, 2, 7, 3},
        {-1, 8,-4, 8}};

    int64_t maxv = 0;

    for (int64_t i = 0; i < board.size(); i++) {
        vector<int64_t> row, col;
        for (int64_t j = 0; j < board.size(); j++) {
            row.push_back(board[i][j]);
            col.push_back(board[j][i]);
        }
        maxv = max (maxv, max (kadane(row), kadane(col)));
        // cout << kadane(row) << " ";
    }

    const int64_t N = board.size();
    const int64_t n2 = N * 2;

    for (int64_t i = 1; i < n2; i++) {

        const int64_t star1 = max ( (int64_t)0, i - N), star2 = max ( (int64_t)0, N - i);

        const int64_t size = min (i, N - star1);
        const int64_t minx = min (N, i) - 1;
        vector<int64_t> diagl (size), diagr (size);

        for (int64_t j = 0; j < size; j++) {
            const int64_t x = star1 + j, lf = star2 + j, rf = minx-j;
            diagl[j] = board[x][rf], diagr[j] = board[x][lf];
            cout  << diagr[j] << " "  ;
        }
        maxv = max (maxv, max (kadane(diagl), kadane(diagr)));
        // cout << "\n";
    }

    cout << maxv;
    // diagonal (board);
    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
