#include <iostream>

#include <array>
#include <string>
#include <vector>


std::array<int, 10> cockroaches(const std::vector<std::string> &grid) {

    const int width = grid[0].size(), height = grid.size();
    std::array<int, 10> freq {0};
    std::vector<std::pair<char,std::pair<int,int>>> roaches;
    // const std::vector<std::pair<int,int>> direction = {{0,-1},{-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isalpha(grid[y][x])) {
                roaches.push_back({grid[y][x], {x,y}});
            }
        }
    }

    for (auto [dir,p1] : roaches) {
        auto [dx,dy] = p1;
        std::cout << dir << " => ";

        while (true) {
            char nxt = ' ';

            if (dy == height-2) {
                nxt = grid[height-1][dx];
            }
            if (dx == width-2) {
                nxt = grid[dy][width-1];
            }

            if (dy == 1) {
                nxt = grid[0][dx];
            }
            if (dx == 1) {
                nxt = grid[dy][0];
            }

            // std::cout << dx << " " << dy << " " << dir << nxt << "\n";

            if (nxt == '+') break;

            if (isdigit(nxt)) {
                freq[nxt-'0']++;

                std::cout << nxt-'0' << '\n';
                break;
            } else if (nxt == '|') {
                if (dir == 'L') dir = 'D';
                if (dir == 'R') dir = 'U';
            } else if (nxt == '-') {
                if (dir == 'U') dir = 'L';
                if (dir == 'D') dir = 'R';
            }

            switch(dir) {
              case 'U': dy--; break;
              case 'L': dx--; break;
              case 'D': dy++; break;
              case 'R': dx++; break;
            }
        }
    }
    // std::cout << dir << " => "  ;
    return freq;
}
int main () {

    std::vector<std::string> grid = {
        "+----------------0---------------+",
        "|                                |",
        "|                                |",
        "|          U        D            |",
        "|     L                          |",
        "|              R                 |",
        "|           L                    |",
        "|  U                             1",
        "3        U    D                  |",
        "|         L              R       |",
        "|                                |",
        "+----------------2---------------+"
    };
    // , {1,2,2,5,0,0,0,0,0,0});

    auto res = cockroaches(grid);

    std::cout << "\n";
    for (int i = 0; i < 10; i++) {
        if (res[i]) {
            std::cout << i << " :: " << res[i] << "\n";
        }
    }

    std::cout << "\nexit\n";
}

bool is_inside (int x, int y, int width, int height) { return x >= 0 && y >= 0 && x < width && y < height; }

std::array<int, 10> cockroaches_ark(const std::vector<std::string> &grid) {
    //                                                    N      NW       W       SW     S     SE     E      NE
    const std::vector<std::pair<int,int>> direction = {{0,-1},{-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}};

    const int width = grid[0].size(), height = grid.size();
    std::array<int, 10> freq {0};
    std::vector<std::pair<char,std::pair<int,int>>> roaches;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isalpha(grid[y][x])) {
                roaches.push_back({grid[y][x], {x,y}});
            }
        }
    }

    for (auto [dir,pos] : roaches) {
        bool search = true;
        // std::cout << dir << " => ";

        while (search) {
            // std::cout << pos.first << ' ' << pos.second << "\n";
            for (auto &[dx,dy] : direction) {
                int nx = pos.first + dx, ny = pos.second + dy;

                if (isdigit (grid[ny][nx])) {
                    freq[grid[ny][nx] - '0']++;
                    search = false;
                    // std::cout << grid[ny][nx] - '0' << "\n";
                    break;
                } else if (grid[ny][nx] == '|') {
                    if (dir == 'R') {dir = 'U'; break; }
                    if (dir == 'L') {dir = 'D'; break; }
                } else if (grid[ny][nx] == '-') {
                    if (dir == 'U') {dir = 'L'; break; }
                    if (dir == 'D') {dir = 'R'; break; }
                }
            }

            switch (dir) {
                case 'L': pos.first--; break;
                case 'R': pos.first++; break;
                case 'U': pos.second--; break;
                case 'D': pos.second++; break;
            }
        }

    }

    return freq;
}
