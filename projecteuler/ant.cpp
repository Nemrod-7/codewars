#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

enum {white, black};
std::vector<std::pair<int,int>> direct {{0,-1},{1,0},{0,1},{-1,0}};

void display (std::vector<std::vector<bool>> &grid, std::pair<int,int> &p) {

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (j == p.first && i == p.second) {
                std::cout << "x";
            } else {
                if (grid[i][j]) {
                    std::cout << "#";
                } else {
                    std::cout << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

int count (const std::vector<std::vector<bool>> &grid) {

    int cnt = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j]) {
                cnt++;
            }
        }
    }
    return cnt;
}

int ant_animation () {
    int N = 500;
    int st = N / 2;

    std::vector<int> seq;
    std::vector<std::vector<bool>> grid (N, std::vector<bool> (N));
    std::pair<int,int> p {st, st};
    auto &[x,y] = p;
    int pos = 0;

    for (int cycle = 0; cycle < 12000; cycle++) {
        auto [dx,dy] = direct[pos];
        p = {x + dx, y + dy};

        switch (grid[y][x]) {
            case white: pos = pos == 3 ? 0 : pos + 1; break;
            case black: pos = pos == 0 ? 3 : pos - 1; break;
        }

        grid[y][x] = grid[y][x] == false ? true : false;

        if (cycle % 104 == 0) {
            int cnt = count (grid);

            if (seq.size()) {
                int df = cnt - seq.back();

                cout << cycle << " " << df << "\n";
            }

            seq.push_back(cnt);
        }



        // cout << "\033c";
        // display (grid, p);
        // std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    return count (grid);
}

int main () {

    enum {white, black};
    const uint64_t limit = 1e18;

    //    cout << limit - 104 * num_period;
    //cout << ans + ant_animation(7) << "\n";
    // 9984

    cout << ant_animation ();




    //cout << (limit % 104) * 12 + 717;
}
