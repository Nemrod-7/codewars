#include <iostream>

#include <array>
#include <string>
#include <vector>

bool is_inside (int x, int y, int width, int height) { return x >= 0 && y >= 0 && x < width && y < height; }

std::array<int, 10> cockroaches(const std::vector<std::string> &grid) {
    const std::vector<std::pair<int,int>> direction = {{-1,0},{0,-1},{0,1},{1,0}, {-1,-1}, {-1,1}, {1,1},{1,-1}};
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

        while (search) {
            for (auto &[dx,dy] : direction) {
                int nx = pos.first + dx, ny = pos.second + dy;

                if (is_inside (nx,ny,width,height)) {

                    if (isdigit (grid[ny][nx])) {
                        freq[grid[ny][nx] - '0']++;
                        search = false;
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

    // std::vector<std::pair<char,std::pair<int,int>> cand;
    // order : p2.x <= p1.x && p2.y <= p1.y
    // else : p2.y >= p1

    std::cout << dir << " => "  ;

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

    // auto res = cockroaches(room);
    //
    // for (int i = 0; i < 10; i++) {
    //     if (res[i]) {
    //         std::cout << i << " :: " << res[i] << "\n";
    //     }
    // }

}
