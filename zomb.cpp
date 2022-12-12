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
    cout << endl;
}

vector<vector<cell>> mkgraph (vector<string> lawn, vector<vector<int>> zombies) {

  int height = lawn.size(), width = lawn[0].size();
  vector<vector<cell>> grid (height, vector<cell> (width));

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          if (isdigit(lawn[y][x])) grid[y][x] = {plant, lawn[y][x] - '0' };
          if (lawn[y][x] == 'S') grid[y][x] = {multi, 1 };
      }
  }

  for (auto inv : zombies) {
      int move = inv[0], y = inv[1], hp = inv[2];
      if (move == 0)
          grid[y][width - 1] = { zombe, hp };
  }

  return grid;
}

void disperse (vector<vector<cell>> &grid, int x, int y) {

    int height = grid.size(), width = grid[0].size();
    for (int i = 1; i < width - x ; i++) {
        int dx = i + x;

        for (int j = -1; j < 2; j++) {
            int dy = y + i * j;

            if (dy >= 0 && dy < height && grid[dy][dx].first == zombe) {
                grid[dy][dx].second -= 1;
            }
        }
    }
}
void display2 (vector<vector<pair<char,int>>> &plants, vector<vector<int>> &zombies, int ng) {

    const int height = plants.size(), width = plants[0].size();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            string tup = " ";

            for (auto [id,pos] : plants[y]) {
                if (pos == x)
                    tup = id;
            }

            for (auto &inv : zombies) {
                int move = inv[0], row = inv[1], hp = inv[2];
                int col = (width - 1) - (ng - move);

                if (row == y && col == x)
                    tup = to_string (hp);
            }
            cout << "[" << setw(2) << tup << "]";
        }
        cout << endl;
    }
}
int plantzombie2 (vector<string> lawn, vector<vector<int>> zombies) {

    int ng = 0;
    const int height = lawn.size(), width = lawn[0].size();
    vector<vector<pair<char,int>>> plants (height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isdigit(lawn[y][x]) || lawn[y][x] == 'S') {
                plants[y].push_back ({lawn[y][x], x});
            }
        }
    }

    for (int ng = 0; ng < 2; ng++) {
        vector<int> dfce(height);
        cout << "cycle " << ng << " => ";

        for (int y = 0; y < height; y++) {
            for (auto &[id, pos] : plants[y]) {
                if (id == 'S') {
                    for (int i = 1; i < width - pos ; i++) {
                        int dx = i + pos;

                        for (int j = -1; j < 2; j++) {
                            int dy = y + i * j;
                            if (dy >= 0 && dy < height) {
                            /*
                             && grid[dy][dx].first == zombe) {
                                grid[dy][dx].second -= 1;

                            */
                            }

                        }
                    }
                    dfce[y] += 1;
                } else {
                    dfce[y] += (id - '0');
                }
            }
        }

        for (auto &stat : zombies) { // move - row - hp
            int move = stat[0], row = stat[1], hp = stat[2];
            int col = (width - 1) - (ng - move);

            if (col < width) {
                //cout << "[" << dfce[row]<< "] => " << hp;
                if ((stat[2] - dfce[row]) < 0) {
                    dfce[row] -= hp;
                    stat[2] = 0;
                } else {
                    stat[2] -= dfce[row];
                    dfce[row] = 0;
                }
                cout << "[" << hp << "]\n";
            }
        }
        //cout << "\n";
    }



    return ng;
}
int plantzombie3 (vector<string> lawn, vector<vector<int>> zombies) {

    int ng = 0;
    const int height = lawn.size(), width = lawn[0].size();
    vector<vector<pair<char,int>>> plants (height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isdigit(lawn[y][x]) || lawn[y][x] == 'S') {
                plants[y].push_back ({lawn[y][x], x});
            }
        }
    }

    for (auto &inv : zombies) {
        int move = inv[0], row = inv[1], hp = inv[2];
        int col = (width - 1) - (ng - move);

        if (col < width) {
            int cnt = 0;
            for (int x = 0; x < col; x++) {
                if (isdigit (lawn[row][col])) cnt += lawn[row][col];
                if (lawn[row][col] == 'S') cnt += 1;
            }
            cout << hp << ' ';
        }
    }

    for (int ng = 0; ng < 2; ng++) {

    }



    return ng;
}

int plantzombie1 (vector<string> lawn, vector<vector<int>> zombies) {

  int ng = 0;
  const int height = lawn.size(), width = lawn[0].size();
  vector<vector<cell>> grid (height, vector<cell> (width));

  vector<vector<pair<int,int>>> plants (height);

  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          int index = y * width + x;
          if (isdigit(lawn[y][x])) {
              grid[y][x] = { plant, lawn[y][x] - '0' };
          }
          if (lawn[y][x] == 'S') {
              grid[y][x] = { multi, 1 };
          }
      }
  }

  for (int ng = 0; ng < 4; ng++) {
      cout << "cycle " << ng  << " :: \n";
      for (auto inv : zombies) {
          int move = inv[0], y = inv[1], hp = inv[2];
          if (move == ng)
              grid[y][width - 1] = { zombe, hp };
      }
      display (grid);
      for (int y = 0; y < height; y++) {
          for (int x = 0, cnt = 0; x < width; x++) {
              auto &[id, pt] = grid[y][x];

              switch (id) {
                  case plant: cnt += pt; break;
                  case multi: disperse (grid,x,y); break;
                  case zombe:
                      if ((pt - cnt) < 0) {
                          cnt -= pt;
                          id = grass, pt = 0;
                      } else {
                          pt -= cnt;
                          cnt = 0;
                      }
                      break;
                  default: break;
              }
          }
      }

      for (int y = 0; y < height; y++) {
          for (int x = 0; x < width; x++) {
              auto &[id, pt] = grid[y][x];

              if (id == zombe) {
                  if (x > 0) {
                      grid[y][x-1] = grid[y][x];
                      grid[y][x] = { grass, 0 };
                  }
              }
          }
      }
    }

    return ng;
}

int main () {

  vector<string> lawn = { "2       ", "  S     ", "21  S   ", "13      ", "2 3     " };
  vector<vector<int>> zombies = { {0,4,28}, {1,1,6}, {2,0,10}, {2,4,15}, {3,2,16}, {3,3,13} };

  plantzombie1 (lawn, zombies);

    cout << "end\n";
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
