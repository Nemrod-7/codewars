#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <chrono>

using namespace std;

// NxN nonogram solver
class Nonogram {
    private :
        size_t width, height;

        vector<vector<bool>> grid;
        vector<vector<int>> cspx, cspy;
				vector<vector<int>> top1, left1;

        vector<vector<int>> mkcomb (vector<vector<int>> top, int nd) {

            int size = top.size(), end = 1;
            vector<vector<int>> csp (top.size());

            for (int i = 0; i < nd; i++)
                end <<= 1;
						//cout << end << endl;
            for (int num = 0; num < end; num++) {
                int acc = 0;
                vector<int> line;
								//cout << num << " => ";
                for (int i = 0; i < nd; i++) {
                    bool bit = static_cast<bool> (num >> i &1);
										//cout << bit;
                    acc += bit;
                    if ((bit == 0 || i == nd - 1) && acc != 0) {
                        line.push_back(acc);
                        acc = 0;
                    }
                }
								/*
								cout << " :: ";
								for (auto it : line) {
										cout << it << ' ';
								}
								cout << endl;
								*/

                for (int i = 0; i < size; i++) {
                    if (top[i] == line) csp[i].push_back(num);
                }
            }

            return csp;
        }
				void filter () { // reduce bitmasking
												 //
						for (size_t y = 0; y < height; y++) {

								for (size_t x = 0; x < width; x++) {

										if (cspy[y].size() == 1) {
												int row = cspy[y].front();
												int bit = row >> x &1;
												//grid[y][x] = bit; 

												/*
												for (auto it = cspx[x].begin(); it != cspx[x].end();) {
														int num = *it;

														if (bity != (num >> y &1)) {
																cspx[x].erase(it);
														} else {
																it++;
														}
												}
												*/
										}

										if (cspx[x].size() == 1) {
												int col = cspx[x].front();

												/*
												for (auto it = cspy[y].begin(); it != cspy[y].end();) {
														int num = *it;

														if (col != (num >> y &1)) {
																cspy[y].erase(it);
														} else {
																it++;
														}
												}
												*/
										}
								}
								//cout << endl;
								//cout << cspy[y].size() << "\n";
						}

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
                for (size_t i = 0; i < pady; i++) {
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

            for (size_t y = 0; y < height; y++) {
                size_t space = pady - left[y].size();
                while (space-->0) cout << setw(3) << "|";

                for (auto cspy : left[y])
                    cout << setw(2) << cspy << "|";

                for (size_t x = 0; x < width; x++) {
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

						top1 = top, left1 = left;

            cspx = mkcomb(top, height);
						cspy = mkcomb(left, width);

						filter ();
            //show (top, left);
        }

				bool backtrack (vector<vector<bool>> &grid, const vector<vector<int>> &cspy, const vector<vector<int>> &cspx, int index) {

						if (index == height) {

								for (int x = 0; x < width; x++) {
										int num = 0;

										for (int y = 0; y < height; y++) {
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
								for (int x = 0; x < width; x++) {
										grid[index][x] = num >> x &1;
								}

								if (backtrack (grid, cspy, cspx, index + 1) == true) {
										return true;
								}
						}

						return false;
				}

				vector<vector<bool>> solve () {
						
						backtrack (grid, cspy, cspx, 0);
						//show (top1,left1);

						for (int y = 0; y < height; y++) {
								for (int x = 0; x < width; x++) {
										cout << (grid[y][x] ? "#" : " ");
								}
								cout << endl;
						}
						return grid;
				}

        static pair<vector<vector<int>>,vector<vector<int>>> encoder (vector<vector<int>> grid) {

            vector<vector<int>> left, top;

            for (size_t y = 0; y < grid.size(); y++) {
                int numy = 0, numx = 0;
                vector<int> line, col;

                for (size_t x = 0; x < grid[0].size(); x++) {
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
        bset.push_back(x >> i &1);

    return bset;
}
int to_int (vector<bool> bset) {

    int num = 0;

    for (size_t i = 0; i < bset.size(); i++)
        num |= bset[i] << i;

    return num;
}

int main () {

		auto start = chrono::high_resolution_clock::now();

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

		/*
		*/
    //auto [top1,left1] = encoder (grid);
    top = {{3}, {4}, {2, 2, 2}, {2, 4, 2}, {6}, {3}};
    left = {{4}, {6}, {2, 2}, {2, 2}, {2}, {2}, {2}, {2}, {}, {2}, {2}};

    Nonogram board (top,left);
		//board.solve();


		auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "\nProcess took " << elapsed.count()  << " ms" << endl;

}

pair<vector<vector<int>>,vector<vector<int>>> encoder (vector<vector<int>> grid) {

    vector<vector<int>> left, top;

    for (size_t y = 0; y < grid.size(); y++) {
        int numy = 0, numx = 0;
        vector<int> line, col;

        for (size_t x = 0; x < grid[0].size(); x++) {
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

