#include <iostream>

#include <array>
#include <string>
#include <vector>

std::array<int, 10> cockroaches (const std::vector<std::string> &grid) {

    const int width = grid[0].size(), height = grid.size();
    const std::vector<std::pair<int,int>> corner = {{1,1}, {1,height-2}, {width-2,1}, {width-2,height-2}};
    const std::vector<std::pair<int,int>> corn2 = {{0,0}, {0,height-1}, {width-1,0}, {width-1,height-1}};
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
        // std::cout << dir << "=> ";

        while (search) {
            auto &[dx,dy] = pos;
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
                // std::cout << grid[ly][lx] ;
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
        // std::cout << "\n";
    }

    return freq;
}

int main () {

    std::vector<std::string> grid ;

    grid = {
    "+--------------3--------9--------6",
    "|                         R      |",
    "7                 D    R         |",
    "|                    L           |",
    "|                                |",
    "|                          R     |",
    "4        L                    L  2",
    "|                                |",
    "|     R                          |",
    "|       D                        |",
    "|   L                        R   |",
    "+--8-----------01----------------5"
  };
  // "+--------------3--------9--------6",
  // "|                         6      |",
  // "7                 5    6         |",
  // "|                    4           |",
  // "|                                |",
  // "|                          6     |",
  // "4        4                    4  2",
  // "|                                |",
  // "|     2                          |",
  // "|       0                        |",
  // "|   8                        5   |",
  // "+--8-----------01----------------5"

        //       0  1  2  3  4  5  6  7  8  9
    // Expec : [ 1, 0, 2, 0, 3, 1, 3, 0, 1, 0 ]

    // Actual: [ 1, 0, 1, 0, 3, 2, 3, 0, 1, 0 ]

    auto res = cockroaches(grid);
    std::cout << "\n";

    for (int i = 0; i < 10; i++) {
        if (res[i]) {
            std::cout << i << " :: " << res[i] << "\n";
        }
    }

    std::cout << "\nexit\n";
}

// Expect: [ 0, 1, 2, 1, 1, 2, 3, 2, 1, 1 ]
// Actual: [ 0, 1, 2, 1, 1, 2, 4, 2, 1, 0 ]
