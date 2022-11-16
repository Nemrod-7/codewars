#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cmath>

using namespace std;
// NxN nonogram solver
class Nonogram {
    private :
        int width, height;
        vector<vector<bool>> grid;
        vector<vector<int>> cspx, cspy;

        vector<vector<int>> mkcomb (vector<vector<int>> top) {

            int size = top.size(), end = 1;
            vector<vector<int>> csp (size);

            for (int i = 0; i < size; i++)
                end <<= 1;

            for (int num = 0; num < end; num++) {
                int acc = 0;
                vector<int> line;

                for (int i = 0; i < size; i++) {
                    bool bit = static_cast<bool> (num &1 << i);
                    acc += bit;
                    if ((bit == 0 || i == size - 1) && acc != 0) {
                        line.push_back(acc);
                        acc = 0;
                    }
                }

                for (int i = 0; i < size; i++) {
                    if (top[i] == line) csp[i].push_back(num);
                }
            }
            return csp;
        }

        void show (vector<vector<int>> top, vector<vector<int>> left) {
            size_t padx = 0, pady = 0;

            for (auto csp : top) {
                padx = max (csp.size(), padx);
            }
            for (auto csp : left) {
                pady = max (csp.size(), pady);
            }

            while (padx-->0) {
                for (int i = 0; i < pady; i++) {
                    cout << setw(2) << " " << "|";
                }
                for (auto &csp : top) {
                    if (csp.size() > padx) {
                        cout << setw(2) << csp[padx];
                    } else {
                        cout << setw(2) << " ";
                    }
                    cout << setw(2) << "|";
                }
                cout << setw(2) << endl;
            }

            for (int y = 0; y < height; y++) {
                int space = pady - left[y].size();
                while (space-->0) cout << setw(3) << "|";

                for (auto cspy : left[y])
                    cout << setw(2) << cspy << "|";

                for (int x = 0; x < width; x++) {
                    if (grid[y][x]) {
                        cout << setw(2) << "#";
                    } else {
                        cout << setw(2) << " ";
                    }
                    cout << setw(2) << "|";
                }
                cout << setw(2) << endl;
            }
        }
    public :
        Nonogram (vector<vector<int>> top, vector<vector<int>> left) {
            width = top.size(), height = left.size();
            grid.resize(height, vector<bool> (width));

            cspx = mkcomb(top), cspy = mkcomb (left);

            cout << cspy[8].front() << endl;
            /*
            for (auto csp : cspy) {
                cout << csp.size() << endl;
            }
            */
            show (top, left);
        }

        static pair<vector<vector<int>>,vector<vector<int>>> encoder (vector<vector<int>> grid) {

            vector<vector<int>> left, top;

            for (int y = 0; y < grid.size(); y++) {
                int numy = 0, numx = 0;
                vector<int> line, col;

                for (int x = 0; x < grid[0].size(); x++) {
                    numy += grid[y][x], numx += grid[x][y];

                    if ((grid[y][x] == 0 || x == grid[0].size() - 1) && numy != 0) {
                        line.push_back(numy);
                        numy = 0;
                    }

                    if ((grid[x][y] == 0 || x == grid[0].size() - 1) && numx != 0) {
                        col.push_back(numx);
                        numx = 0;
                    }
                }

                top.push_back(col);
                left.push_back(line);
            }

            return {top,left};
        }
};

vector<bool> to_bset (int x) {

    vector<bool> bset;

    for (int i = 0; i < 5; i++)
        bset.push_back(static_cast<bool>(x&1 << i));

    return bset;
}
int to_int (vector<bool> bset) {

    int num = 0;

    for (int i = 0; i < 5; i++)
        num |= bset[i] << i;

    return num;
}

vector<int> mkcomb (int num) {

    vector<int> comb;
    int acc = 0;

    for (int i = 0; i < 5; i++) {
        bool bit = static_cast<bool> (num &1 << i);
        acc += bit;
        if ((bit == 0 || i == 4) && acc != 0) {
            comb.push_back(acc);
            acc = 0;
        }
    }

    return comb;
}

bool backtrack (vector<vector<bool>> &grid, const vector<vector<int>> &cspy, const vector<vector<int>> &cspx, int index) {

    if (index == 5) {

        for (int x = 0; x < 5; x++) {
            int num = 0;

            for (int y = 0; y < 5; y++) {
                num |= grid[y][x] << y;
            }

            if (find (cspx[x].begin(), cspx[x].end(), num) == cspx[x].end()) {
                return false;
            }
        }
        //Display::board(grid);

        return true;
    }

    for (auto &num : cspy[index]) {
        for (int x = 0; x < 5; x++) {
            grid[index][x] = static_cast<bool>(num &1 << x);
        }

        if (backtrack (grid, cspy, cspx, index + 1) == true) {
            return true;
        }
    }

    return false;
}
vector<vector<bool>> solver (vector<vector<int>> top, vector<vector<int>> left) {

    vector<vector<int>> cspy(5), cspx(5);
    vector<vector<bool>> grid (5, vector<bool> (5));

    // making simple bitmasking
    for (int num = 0; num < 32; num++) {
        vector<int> comb = mkcomb (num);
        for (int i = 0; i < 5; i++) {
            if (top[i] == comb) cspx[i].push_back(num);
            if (left[i] == comb) cspy[i].push_back(num);
        }
        //Display::bitnum (i);
    }


    /*
    // reduce bitmasking
    for (int y = 0; y < 5; y++) {
        vector<int> line;
        for (int x = 0; x < 5; x++) {
            int col = cspx[x].front();

            if (cspx[x].size() == 1) {
                for (auto &cmy : cspy[y]) {

                    if (static_cast<bool>(cmy &1 << x) == static_cast<bool>(col&1 << y))
                        line.push_back(cmy);
                }
            }
        }
        cspy[y] = line;
    }
    */
    backtrack (grid, cspy, cspx, 0);

    //Display::nonogram (grid ,top,left);
    return grid;
}

pair<vector<vector<int>>,vector<vector<int>>> encoder (vector<vector<int>> grid) {

    vector<vector<int>> left, top;

    for (int y = 0; y < grid.size(); y++) {
        int numy = 0, numx = 0;
        vector<int> line, col;

        for (int x = 0; x < grid[0].size(); x++) {
            numy += grid[y][x], numx += grid[x][y];

            if ((grid[y][x] == 0 || x == grid[0].size() - 1) && numy != 0) {
                line.push_back(numy);
                numy = 0;
            }

            if ((grid[x][y] == 0 || x == grid[0].size() - 1) && numx != 0) {
                col.push_back(numx);
                numx = 0;
            }
        }

        top.push_back(col);
        left.push_back(line);
    }

    return {top,left};
}

int main () {

    vector<vector<int>> top = {{1,1},{4},{1,1,1},{3},{1}};
    vector<vector<int>> left = {{1},{2},{3}, {2,1},{4}};


    vector<vector<int>> grid = {
    {0, 0, 0, 1, 0, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 0, 1, 0, 0, 0, 1, 1, 0, 0},
    {1, 1, 1, 0, 0, 1, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    auto [top1,left1] = encoder (grid);
    top = {{3}, {4}, {2, 2, 2}, {2, 4, 2}, {6}, {3}};
    left = {{4}, {6}, {2, 2}, {2, 2}, {2}, {2}, {2}, {2}, {}, {2}, {2}};
             /*
    */
    Nonogram graph (top,left);
    //solver (top,left);

    /*
    for (int num = 0; num < 32; num++) {
        bitset<5> mask (num);

        for (int i = 0; i< 5; i++) {
            bool bit = mask[i];
            cout << bit;
        }
        cout << endl;
        //cout << num << " => " << mask.to_string() << "\n";
        //int cell = mask;
    }
    */
}

class Display {
    public :
        static void board (vector<vector<bool>> grdi) {
            cout << endl;
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
        static void nonogram (vector<vector<bool>> grid, vector<vector<int>> top, vector<vector<int>> left) {
            size_t padx = 0, pady = 0;

            for (auto csp : top) {
                padx = max (csp.size(), padx);
            }
            for (auto csp : left) {
                pady = max (csp.size(), pady);
            }
            while (padx-->0) {
                for (int i = 0; i < pady; i++) {
                    cout << " " << "|";
                }
                for (auto &csp : top) {
                    if (csp.size() > padx) {
                        cout << csp[padx];
                    } else {
                        cout << " ";
                    }
                    cout << "|";
                }
                cout << endl;
            }

            for (int y = 0; y < 5; y++) {
                int space = pady - left[y].size();
                while (space-->0) cout << " |";

                for (auto cspy : left[y])
                    cout << cspy << "|";

                for (int x = 0; x < 5; x++) {
                    if (grid[y][x]) {
                        cout << "#";
                    } else {
                        cout << " ";
                    }
                    cout << "|";
                }
                cout << endl;
            }
        }
};
