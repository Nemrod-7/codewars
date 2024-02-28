#include <iostream>

#include <array>
#include <string>
#include <vector>

bool is_inside (int x, int y, int width, int height) { return x >= 0 && y >= 0 && x < width && y < height; }

std::array<int, 10> cockroaches(const std::vector<std::string> &grid) {
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
        std::cout << dir << " => ";

        while (search) {
            std::cout << pos.first << ' ' << pos.second << "\n";
            for (auto &[dx,dy] : direction) {
                int nx = pos.first + dx, ny = pos.second + dy;
                if (is_inside (nx,ny,width,height)) {

                    if (isdigit (grid[ny][nx])) {
                        freq[grid[ny][nx] - '0']++;
                        search = false;
                        std::cout << grid[ny][nx] - '0' << "\n";
                        break;
                    } else if (grid[ny][nx] == '|') {
                        if (dir == 'R') {dir = 'U'; break; }
                        if (dir == 'L') {dir = 'D'; break; }
                    } else if (grid[ny][nx] == '-') {
                        if (dir == 'U') {dir = 'L'; break; }
                        if (dir == 'D') {dir = 'R'; break; }
                    }
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

    // auto res = cockroaches(grid);

    // std::cout << "\n";
    // for (int i = 0; i < 10; i++) {
    //     if (res[i]) {
    //         std::cout << i << " :: " << res[i] << "\n";
    //     }
    // }
    const std::vector<std::pair<int,int>> direction = {{-1,0},{0,-1},{0,1},{1,0}};
    const int width = grid[0].size(), height = grid.size();
    std::array<int, 10> freq {0};
    std::vector<std::pair<char,std::pair<int,int>>> roaches, hole;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isalpha(grid[y][x])) {
                roaches.push_back({grid[y][x], {x,y}});
            } else if (isdigit(grid[y][x])) {
                hole.push_back({grid[y][x], {x,y}});
            }
        }
    }

    auto [dir,p1] = roaches[0];


    std::cout << dir << " => "  ;
    // std::map<char, std::pair<int,int>> direction {{'U', {0,-1}}, {'L', {-1,0}}, {'D', {0,1}}, {'R', {1,0}}};

    if (dir == 'U') {
        if (is_inside (p1.first,p1.second-1, width, height)) {
            char nxt = grid[p1.second-1][p1.first];

            if (isdigit(nxt)) {
                freq[nxt-'0']++;
            } else if (nxt == '-') {
                dir = 'L';
            } else {
                p1.second--;
            }
        }
    } else if (dir == 'L') {
        if (is_inside (p1.first - 1,p1.second, width, height)) {
          char nxt = grid[p1.second][p1.first-1];

          if (isdigit(nxt)) {
              freq[nxt-'0']++;
          } else if (nxt == '|') {
              dir = 'D';
          } else {
              p1.first--;
          }
}
    } else if (dir == 'D') {
        if (is_inside (p1.first,p1.second + 1, width, height)) {
          char nxt = grid[p1.second+1][p1.first];

          if (isdigit(nxt)) {
              freq[nxt-'0']++;
          } else if (nxt == '-') {
              dir = 'R';
          } else {
              p1.second++;
          }
        }
    } else if (dir == 'R') {
        if (is_inside (p1.first + 1,p1.second, width, height)) {
          char nxt = grid[p1.second][p1.first+1];

          if (isdigit(nxt)) {
              freq[nxt-'0']++;
          } else if (nxt == '-') {
              dir = 'U';
          } else {
              p1.first++;
          }
        }
    }


    // for (auto &[ex, p2] : hole) {
    //       if (dir == 'U') {
    //           if (p2.first <= p1.first && p2.second < p1.second) {
    //               std::cout << "first : " << ex << "\n";
    //               // break;
    //           } else if (p2.first < p1.first && p2.second >= p1.second){
    //               std::cout << "second : " << ex << "\n";
    //             // break;
    //           }
    //           else if (p2.first >= p1.first && p2.second > p1.second) {
    //               std::cout << "third : " << ex << "\n";
    //
    //           } else if (p2.first > p1.first && p2.second < p1.second) {
    //               std::cout << "fourth : " << ex << "\n";
    //           }
    //       }
    // }



}
