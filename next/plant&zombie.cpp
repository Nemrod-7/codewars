#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;
using cell = pair<int,int>;
  enum { grass, plant, multi, zombe };

void display (vector<vector<cell>> grid) {

    const int height = grid.size(), width = grid[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];
            cout << "[";
            cout << setw(2);
            switch (id) {
                case grass: cout << " "; break;
                case plant: cout << pt; break;
                case multi: cout << "s"; break;
                case zombe: cout << pt; break;
                default: break;
            }
            cout << "]";
        }
        cout << endl;
    }
}
void update (vector<vector<cell>> &grid, vector<vector<int>> &zombies, int ng) {

    int height = grid.size(), width = grid[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            if (id == zombe) {
                if (pt > 0) {
                    int nx = x - 1;
                    if (nx >= 0) {
                        grid[y][nx] = grid[y][x];
                        grid[y][x] = {grass, 0};
                    }
                } else {
                    id = grass, pt = 0;
                }
            }
        }
        if (ng < zombies[y].size()) {
            grid[y][width - 1] = { zombe, zombies[y][ng] };
        }
    }
}
bool attack (vector<vector<cell>> &grid) {

    int height = grid.size(), width = grid[0].size();
    bool defense = false;

    for (int y = 0; y < height; y++) {
        int cnt = 0;
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            switch (id) {
                case grass: break;
                case plant:
                  cnt += pt;
                  defense = true;
                  break;
                case multi:
                    defense = true;
                    for (int i = 1; i < width - x ; i++) {
                        int dx = i + x;
                        for (int j = -1; j < 2; j++) {
                            int dy = y + i * j;

                            if (dy >= 0 && dy < height && grid[dy][dx].first == zombe) {
                                grid[dy][dx].second -= 1;
                            }
                        }
                    }
                    //cnt += 1;
                    break;
                case zombe: pt -= cnt; break;
                default: break;
            }
        }
    }

    return defense;
}
int main () {

  vector<string> lawn = { "2       ", "  S     ", "21  S   ", "13      ", "2 3     " };
  vector<vector<int>> zombies = { {0,4,28}, {1,1,6}, {2,0,10}, {2,4,15}, {3,2,16}, {3,3,13} };

  int height = lawn.size(), width = lawn[0].size();
  int ng = 0;
  vector<vector<cell>> grid (height, vector<cell> (width));

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          if (isdigit(lawn[y][x])) grid[y][x] = {plant, lawn[y][x] - '0' };
          if (lawn[y][x] == 'S') grid[y][x] = {multi, 1 };
          grid[y][width - 1] = { zombe, zombies[y][ng] };
      }
  }

  for (int i = 0; i < 8; i++) {
    attack (grid);
    update (grid, zombies, i);
    cout << endl;
    display (grid);

  }

}
