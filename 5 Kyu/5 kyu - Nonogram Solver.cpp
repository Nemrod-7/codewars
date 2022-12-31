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

        vector<vector<int>> grid;
        vector<vector<int>> cspx, cspy;
				vector<vector<int>> top1, left1;

        vector<vector<int>> mkcomb (vector<vector<int>> top, int nd) {

            int size = top.size(), end = 1UL << nd;
            vector<vector<int>> csp (top.size());
						//cout << end << endl;
            for (int num = 0; num < end; num++) {
                int acc = 0;
                vector<int> clues;
								//cout << num << " => ";
                for (int i = 0; i < nd; i++) {
                    bool bit = (num >> i &1);
										//cout << bit;
                    acc += bit;
                    if ((bit == 0 || i == nd - 1) && acc != 0) {
                        clues.push_back(acc);
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
                    if (top[i] == clues) csp[i].push_back(num);
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
            grid.resize(height, vector<int> (width));

						top1 = top, left1 = left;

            cspx = mkcomb(top, height);
						cspy = mkcomb(left, width);

						for (int y = 0; y < height; y++) {

								if (left[y].size() == 0) continue;

								int maxv = *max_element (left[y].begin(), left[y].end());

								vector<int> line (width);

								for (int i = 0; i < maxv; i++) {
										line[i]++,line[width - i - 1]++;
								}

								for (int x = 0; x < width; x++) { // if overlapping
										if (line[x] == 2) grid[y][x] = 1;
								}
					  }

					  for (int x = 0; x < width; x++) {

								if (top[x].size() == 0) continue;

								int maxv = *max_element (top[x].begin(), top[x].end());

								vector<int> line (height);

								for (int i = 0; i < maxv; i++) {
										line[i]++, line[height - i - 1]++;
								}
								for (int y = 0; y < height; y++) { // if overlapping
										if (line[y] == 2) grid[y][x] = 1;
								}
						}
						/*
						for (int y = 0; y < height; y++) {
								for (int x = 0; x < width; x++) { // if overlapping
										cout << grid[y][x];
								}
								cout << endl;
					  }
						*/

					 //filter ();
            show (top, left);
        }

				bool backtrack (vector<vector<int>> &grid, const vector<vector<int>> &cspy, const vector<vector<int>> &cspx, int index) {

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

				vector<vector<int>> solve () {

						backtrack (grid, cspy, cspx, 0);
						//show (top1,left1);

						for (int y = 0; y < height; y++) {
								for (int x = 0; x < width; x++) {
										cout << (grid[y][x] ? "##" : "  ");
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

    auto [top1,left1] = Nonogram::encoder (grid);

    top = {{3}, {4}, {2, 2, 2}, {2, 4, 2}, {6}, {3}};
    left = {{4}, {6}, {2, 2}, {2, 2}, {2}, {2}, {2}, {2}, {}, {2}, {2}};

    int width = 6, height = 11;

    Nonogram board (top,left);
		//board.solve();
    uint64_t num = 1;

    for (int i = 0; i < 45; i++) {
        num <<= 1;
    }
    for (int i = 0; i < num; i++) {

    }
    cout << num;

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
