#include <iostream>

#include <array>
#include <string>
#include <vector>

std::array<int, 10> cockroaches (const std::vector<std::string> &grid) {

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
        auto &[dx,dy] = pos;

        switch (dir) {
            case 'L': while (dx > 1) dx--; dir = 'D'; break;
            case 'U': while (dy > 1) dy--; dir = 'L'; break;
            case 'R': while (dx < width - 2) dx++; dir = 'U'; break;
            case 'D': while (dy < height - 2) dy++; dir = 'R';  break;
        }

        while (search) {
            int nx = dx, ny = dy, lx = dx, ly = dy;

            switch (dir) {
                case 'U': ny = dy - 1, lx = dx + 1; break;
                case 'L': nx = dx - 1, ly = dy - 1; break;
                case 'D': ny = dy + 1, lx = dx - 1; break;
                case 'R': nx = dx + 1, ly = dy + 1; break;
            }

            if (isdigit (grid[ly][lx])) {
                freq[grid[ly][lx] - '0']++;
                break;
            }
            if (isdigit (grid[ny][lx])) {
                freq[grid[ny][lx] - '0']++;
                break;
            }

            if (grid[ny][nx] == '|') {
                if (dir == 'R') dir = 'U';
                if (dir == 'L') dir = 'D';
            } else if (grid[ny][nx] == '-') {
                if (dir == 'U') dir = 'L';
                if (dir == 'D') dir = 'R';
            } else {
                dx = nx, dy = ny;
            }
        }
    }

    return freq;
}

int main () {

    std::vector<std::string> grid ;

    grid = {
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

    auto res = cockroaches(grid);
    std::cout << "\n";

    for (int i = 0; i < 10; i++) {
        if (res[i]) {
            std::cout << i << " :: " << res[i] << "\n";
        }
    }

    std::cout << "\nexit\n";
}

std::array<int, 10> cockroaches1 (const std::vector<std::string> &grid) {

    const int width = grid[0].size(), height = grid.size();
    const std::vector<std::pair<int,int>> corner = {{1,1}, {1,height-2}, {width-2,1}, {width-2,height-2}};
    const std::vector<std::pair<int,int>> corn2 = {{0,0}, {0,height-1}, {width-1,0}, {width-1,height-1}};
    std::array<int, 10> freq {0};
    std::vector<std::pair<char,std::pair<int,int>>> roaches;
    std::vector<std::string> board = grid;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isalpha(grid[y][x])) {
                roaches.push_back({grid[y][x], {x,y}});
                board[y][x] = ' ';
            }
        }
    }

    for (auto [dir,pos] : roaches) {
        bool search = true;
        auto &[dx,dy] = pos;

        switch (dir) {
            case 'L': while (dx > 1) dx--; dir = 'D'; break;
            case 'U': while (dy > 1) dy--; dir = 'L'; break;
            case 'R': while (dx < width - 2) dx++; dir = 'U'; break;
            case 'D': while (dy < height - 2) dy++; dir = 'R';  break;
        }

        board[dy][dx] = dir;

        while (search) {
            int nx = dx, ny = dy, lx = dx, ly = dy;

            switch (dir) {
                case 'U': ny = dy - 1, lx = dx + 1; break;
                case 'L': nx = dx - 1, ly = dy - 1; break;
                case 'D': ny = dy + 1, lx = dx - 1; break;
                case 'R': nx = dx + 1, ly = dy + 1; break;
            }

            if (isdigit (grid[ny][nx])) {
                freq[grid[ny][nx] - '0']++;
                break;
            }

            if (isdigit (grid[ly][lx])) {
                freq[grid[ly][lx] - '0']++;
                break;
            }

            for (int i = 0; i < 4; i++) {
                if (pos == corner[i]) {
                    int corn = grid[corn2[i].second][corn2[i].first];

                    if (isdigit(corn)) {
                        freq[corn - '0']++;
                        search = false;
                    }
                }
            }

            if (grid[ny][nx] == '|') {
                if (dir == 'R') dir = 'U';
                if (dir == 'L') dir = 'D';
            } else if (grid[ny][nx] == '-') {
                if (dir == 'U') dir = 'L';
                if (dir == 'D') dir = 'R';
            } else {
                dx = nx, dy = ny;
            }
        }
    }

    for (int i = 0; i < height; i++) {
        std::cout << board[i] << "\n";
    }

    return freq;
}
