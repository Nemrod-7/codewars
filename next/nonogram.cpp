#include <iostream>
#include <vector>

using namespace std;

// 5x5 nonogram solver
class Display {
    public :
        static void board (vector<vector<int>> grdi) {

            for (auto line : grdi) {
                for (auto cell : line) {
                    cout << cell << ' ';
                }
                cout << endl;
            }
        }
        static void readcomb (int num) {
            for (int i = 0; i < 5; i++) {
                bool bit = num&1 << i;
                cout << bit;
            }
            cout << endl;
        }
};


vector<int> mkcomb (int num) {

    vector<int> comb;
    int acc = 0;

    for (int i = 0; i < 5; i++) {
        bool bit = num &1 << i;
        acc += bit;
        if ((bit == 0 || i == 4) && acc != 0) {
            comb.push_back(acc);
            acc = 0;
        }
    }

    return comb;
}

void solver (vector<vector<int>> top, vector<vector<int>> left) {

    vector<vector<int>> lateral(5), vertical(5);
    vector<vector<int>> grid (5, vector<int> (5));

    for (int num = 0; num < 32; num++) {
        vector<int> comb = mkcomb (num);

        for (int i = 0; i < 5; i++) {
            if (top[i] == comb) vertical[i].push_back(num);
            if (left[i] == comb) lateral[i].push_back(num);
        }
        //Display::bitnum (i);
    }


    int y = 0, x = 0;
    vector<int> line (5);
    auto comb = lateral[y].front();

    for (int x = 0; x < 5; x++) {
        bool bit = bool (comb&1 << x);

    }


    Display::board(grid);

}

int main () {
    vector<vector<int>> top = {{1,1},{4},{1,1,1},{3},{1}};
    vector<vector<int>> left = {{1},{2},{2,1},{4}};

    solver (top,left);
//([&[1, 1], &[4], &[1, 1, 1], &[3], &[1]], [&[1], &[2], &[3], &[2, 1], &[4]])

}
