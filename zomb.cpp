#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
// samsic fY65C2X3RQGkQ2b
using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
        template<class T> static void That (const vector<T> &a, const vector<T> &b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
        template<class T> static void equals_eq (const T &res, const T &exp, std::string str) {

            static int cycle;
            cycle++;
            if (res != exp) {
                std::cout << "test " << cycle << " : " << str << " => ";
                std::cout << "got : " << res << " ";
                std::cout << "exp : " << exp << "\n";
            }
        }
};
template<class T> T Equals (const T& entry) { return entry;}
template<class T> T EqualsContainer (const T& entry) { return entry;}
void Test ();
/////////////////////////////////Timer/////////////////////////////////////////
using cell = pair<int,int>;
enum { grass, plant, multi, zombe };

string display (vector<vector<cell>> grid) {

    const int height = grid.size(), width = grid[0].size();
    stringstream iss;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            iss << "[";
            switch (id) {
                case grass: iss << setw(2) << " "; break;
                case multi: iss << setw(2) <<"s"; break;
                case zombe: iss << setw(2) << pt; break;
                case plant: iss << "p" << pt; break;
                default: break;
            }
            iss << "]";
        }
        iss << endl;
    }
    iss << endl;

    return iss.str();
}
string showforces (vector<vector<cell>> &grid, vector<vector<int>> zmb) {

    const int height = grid.size(), width = grid[0].size();
    ostringstream os;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            switch (id) {
                case grass: os << ";"; break;
                case plant: os << pt << ";"; break;
                case multi: os << "s" << ";"; break;
                default: os << " " << ";"; break;
            }
        }
        os << "  ::  ;";
        for (auto vec : zmb) {
            int mov = vec[0], row = vec[1], hp = vec[2];
            if (row == y) os << hp << ';';
        }
        os << endl;
    }
    os << endl;

    return os.str();
}

vector<vector<cell>> mkgraph (vector<string> lawn) {

    int height = lawn.size(), width = lawn[0].size();
    vector<vector<cell>> grid (height, vector<cell> (width));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (isdigit(lawn[y][x])) grid[y][x] = {plant, lawn[y][x] - '0' };
            if (lawn[y][x] == 'S') grid[y][x] = {multi, 1 };
        }
    }

    return grid;
}
void disperse (vector<vector<cell>> &grid, int x, int y) {

    const int height = grid.size(), width = grid[0].size();
    bool hit[3] = {0};

    for (int i = 1; i < width - x ; i++) {
        int dx = i + x;

        for (int j = 0; j < 3; j++) {
            int dy = y + i * (j - 1);

            if (dy >= 0 && dy < height && hit[j] == false && grid[dy][dx].first == zombe) {
                if (grid[dy][dx].second > 1)
                    grid[dy][dx].second -= 1;
                else
                    grid[dy][dx] = {grass, 0};

                hit[j] = true;
            }
        }
    }
}

string mkcsv (vector<vector<cell>> grid) {

    const int height = grid.size(), width = grid[0].size();
    stringstream iss;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            switch (id) {
                case grass: iss << setw(2) << " "; break;
                case multi: iss << setw(2) <<"s"; break;
                case zombe: iss << setw(2) << pt; break;
                case plant: iss << "p" << pt; break;
                default: break;
            }
            iss << ";";
        }
        iss << endl;
    }
    iss << endl;

    return iss.str();
}
string displayfile (vector<vector<cell>> grid) {

    const int height = grid.size(), width = grid[0].size();
    stringstream iss;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];
            iss << setw(2);
            switch (id) {
                case grass: iss << " "; break;
                case plant: iss << "p" ; break;
                case multi: iss << "s"; break;
                case zombe: iss << pt; break;
                default: break;
            }
            iss << ";";
        }
        iss << endl;
    }
    iss << endl;

    return iss.str();
}
int plantzombie1 (vector<string> lawn, vector<vector<int>> zombies) {

    int cycles = zombies.front()[0], minc = zombies.back()[0];
    const int height = lawn.size(), width = lawn[0].size();
    vector<vector<cell>> grid = mkgraph(lawn);
    stringstream iss;
    //cout << showforces (grid, zombies);
    //ofstream ofs ("plant.csv");
    /*
    for (auto line : lawn) {
        cout << line << endl;
    }
    */
    for (auto inv : zombies) {
        int move = inv[0], row = inv[1], hp = inv[2];
    //    cout << move << " " << row << " " << hp << endl;
    }
    while (true) {
        int nzomb = 0;

        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width; x++) {
                auto &[id, pt] = grid[y][x];

                if (id == zombe) {
                    int nid = grid[y][x-1].first;
                    grid[y][x-1] = { grass, 0 };
                    swap (grid[y][x-1], grid[y][x]);
                }
            }
        }

        for (auto inv : zombies) {
            int move = inv[0], y = inv[1], hp = inv[2];
            if (move == cycles)
                grid[y][width - 1] = { zombe, hp };
        }
        //cout << "cycle " << cycles  << "\n";
        cout << display (grid);
        //ofs << mkcsv (grid);
        //ofs << tofile(grid);
        for (int y = 0; y < height; y++) {
            for (int x = 0, cnt = 0; x < width; x++) {
                auto &[id, pt] = grid[y][x];

                switch (id) {
                    case plant: cnt += pt; break;
                    case multi: disperse (grid,x,y); break;
                    case zombe:
                        if (x == 0) {
                            return cycles + 1;
                        }

                        if (cnt >= pt) {
                            cnt -= pt;
                            grid[y][x] = {grass, 0};
                        } else {
                            pt -= cnt;
                            cnt = 0;
                            nzomb++;
                        }

                        break;
                    default: break;
                }
            }
        }

        if (cycles > minc && nzomb == 0) break;

        cycles++;
    }

    //ofs.close();
    return 0;
}

int main () {

    vector<string> lawn;
    vector<vector<int>> zombies;

    lawn = {"3  S1     "," S S3     "," 32S1     ","6         ","S42       ","S1 15     ","  4       "};
    zombies = {{0,0,22},{0,2,31},{0,3,27},{0,5,36},{0,6,18},{1,1,24},{1,3,17},{1,5,23},{1,6,11},{4,4,37},{7,2,27},{7,3,15},{7,5,20},{8,1,20},{8,2,18},{8,4,27},{8,6,12},{11,3,13},{11,5,18},{15,0,26},{15,1,16},{15,4,21},{15,6,10}};

    lawn = {"43S1     "," 412     ","  11     ","41 2     ","61       ","S22      "};
    zombies = {{0,0,36},{0,1,28},{0,2,8},{0,4,28},{0,5,20},{1,0,24},{1,2,5},{1,3,31},{1,5,13},{2,3,20},{2,4,22},{3,1,25},{3,2,4},{3,5,12},{4,3,18},{4,4,18},{5,0,29},{5,1,20},{5,2,4},{6,0,20},{6,1,14},{6,3,16},{6,4,16},{6,5,13},{9,0,19},{9,2,5},{9,5,12},{10,1,17},{10,2,4},{10,3,18},{10,4,18},{13,0,19},{13,5,11}};
    Assert::That (plantzombie1 (lawn, zombies), 15);

    //Test();
    cout << "\nend\n";
}

void Test() {

    vector<string> lawn = lawn = { "2       ", "  S     ", "21  S   ", "13      ", "2 3     "};
    vector<vector<int>> zombies =  { {0,4,28}, {1,1,6}, {2,0,10}, {2,4,15}, {3,2,16}, {3,3,13}};
    int res =  10;

    lawn = { "11      ", " 2S     ", "11S     ", "3       ", "13      "};
    zombies = { {0,3,16}, {2,2,15}, {2,1,16}, {4,4,30}, {4,2,12}, {5,0,14}, {7,3,16}, {7,0,13}};
    res = 12;
    Assert::That (plantzombie1 (lawn, zombies), res);

    lawn = { "12        ", "3S        ", "2S        ", "1S        ", "2         ", "3         "};
    zombies = { {0,0,18}, {2,3,12}, {2,5,25}, {4,2,21}, {6,1,35}, {6,4,9}, {8,0,22}, {8,1,8}, {8,2,17}, {10,3,18}, {11,0,15}, {12,4,21}};
    res = 20;
    Assert::That (plantzombie1 (lawn, zombies), res);

    lawn = {"12      ", "2S      ", "1S      ", "2S      ", "3       "};
    zombies ={ {0,0,15}, {1,1,18}, {2,2,14}, {3,3,15}, {4,4,13}, {5,0,12}, {6,1,19}, {7,2,11}, {8,3,17}, {9,4,18}, {10,0,15}, {11,4,14}};
    res = 19;
    Assert::That (plantzombie1 (lawn, zombies), res);

    lawn = { "1         ", "SS        ", "SSS       ", "SSS       ", "SS        ", "1         "};
    zombies = { {0,2,16}, {1,3,19}, {2,0,18}, {4,2,21}, {6,3,20}, {7,5,17}, {8,1,21}, {8,2,11}, {9,0,10}, {11,4,23}, {12,1,15}, {13,3,22}};
    res = 0;
    Assert::That (plantzombie1 (lawn, zombies), res);

    lawn = {"31S 1      ","3  1       ","21 21      "," 21 1      ","41  1      "," 131       "};
    zombies = {{0,0,29},{0,2,29},{1,2,19},{1,3,21},{2,0,22},{2,1,23},{2,3,13},{2,5,29},{3,1,14},{3,4,37},{3,5,18},{8,0,17},{8,1,10},{8,2,18},{8,3,11},{8,4,23},{8,5,12},{11,0,15},{11,2,16},{11,3,10},{11,4,18},{14,1,13},{14,4,16},{14,5,16},{15,0,16},{15,2,17},{15,3,11},{16,1,10},{16,5,13}};
    res = 19;
    Assert::That (plantzombie1 (lawn, zombies), res);
        /*
        */
    //example_tests.into_iter{}.for_each{|{grid,zqueue,sol}| assert_eq!{pnz::plants_and_zombies{&grid,&zqueue},sol}};
}

int plantzombie2 (vector<string> lawn, vector<vector<int>> zombies) {

    int cycles = zombies.front()[0], minc = zombies.back()[0];
    const int height = lawn.size(), width = lawn[0].size();
    vector<vector<cell>> grid = mkgraph(lawn);
    stringstream iss;
    //ofstream ofs ("plant.csv");
    //cout << showforces (grid, zombies);

    while (true) {
        int nzomb = 0;
        for (auto inv : zombies) {
            int move = inv[0], y = inv[1], hp = inv[2];
            if (move == cycles)
                grid[y][width - 1] = { zombe, hp };
        }
        cout << "cycle " << cycles  << " :: \n";
        //ofs << tofile(grid);
        cout << display (grid);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto [id, pt] = grid[y][x];
                switch (id) {
                    case plant:

                        for (int i = 0; i < pt; i++) {
                            for (int j = 1, hit = false; j < width - x; j++) {
                                int dx = j + x;

                                if (grid[y][dx].first == zombe && hit == false) {
                                    hit = true;
                                    grid[y][dx].second -= 1;
                                    if (grid[y][dx].second > 1) {
                                        grid[y][dx].second -= 1;
                                    } else {
                                        grid[y][dx] = {grass, 0};
                                    }
                                }
                            }
                        }

                    ; break;

                    case multi: disperse (grid,x,y); break;
                    case zombe:
                        if (x == 0) return cycles + 1;
                        nzomb++; break;
                    default: break;
                }
            }
        }

        if (cycles >= minc && nzomb == 0) break;

        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width; x++) {
                auto &[id, pt] = grid[y][x];

                if (id == zombe) {
                    int nid = grid[y][x-1].first;

                    if (nid == plant || nid == multi) {
                        grid[y][x].second = 1;
                    }

                    grid[y][x-1] = { grass, 0 };
                    swap (grid[y][x-1], grid[y][x]);
                }
            }
        }

        cycles++;
    }

    //ofs.close();
    return 0;
}

/*

// something
function plantsAndZombies(lawn,zombies){
    const zombiesRows = [];
    for (let i = 0; i < lawn.length; i++) {
        zombiesRows.push([]);
    }
    for (let i = 0; i < lawn.length; i++) {
        zombiesRows[i] = zombiesRows[i].sort((a, b) => b[0] - a[0]);
    }
    lawn = lawn.map(row => row.split(''));
    let moves = 0;
    let noZombies;
    while (true) {

// add zombies
 zombies.forEach((zombie, k) => {
  if (zombie[0] === 0) {
    zombiesRows[zombie[1]].push([zombie[0],zombie[2]])
  }
})
zombies = zombies.filter(zombie => zombie[0] > 0);

// first only numbered plants
lawn.forEach((row, i) => {
  row.forEach((spot, j) => {

    // switch fire numbered,, later make for S from right and top
    if (spot !== ' ' && spot !== 'S') {
      let damage = Number(spot);
      zombiesRows[i].some((zombie, k) => {

        // first zombie we can hit
        const plantSpot = row.length - j-1;;
        if (zombie[0] < plantSpot) {
          zombie[1] -= damage;
          damage = 0

          // check if this killed him and remove him if so
          if (zombie[1] <= 0) {
            // save damage for next one
            damage += zombie[1] * -1;
          }
          if (damage == 0) return true
        }
        return false
      })
      zombiesRows[i] = zombiesRows[i].filter(zombie => zombie[1] > 0);
    }
  })
})
let i = 0;
let j = lawn[0].length - 1;
while (j !== -1) {
  const spot = lawn[i][j];
  if (spot === 'S') {

    // get zombies from all 3 lanes
    // horizontal lane
      zombiesRows[i].some((zombie, k) => {
        // first zombie we can hit
        const plantSpot = lawn[0].length-1-j;
        const damage = 1;
        if (zombie[0] < plantSpot) {
          zombie[1] -= damage;
          if (zombie[1] <= 0) {
            zombiesRows[i].splice(k, 1);
          }
          return true;
        }
        return false;
      })

      // diagonal lanes
      // diagonal up
      let iDia = i
      let jDia = j
      while (true) {
        iDia--;
        jDia++;
        if (iDia < 0 || jDia > lawn[0].length-1) break
        // check if any zombies on this place
        let zombieShot = false;
        zombiesRows[iDia].some((zombie, k) => {
          if (zombie[0] == lawn[0].length-1 - jDia) {
            zombie[1] -= 1;
            // check if this killed him and remove him if so
            if (zombie[1] <= 0) {
              zombiesRows[iDia].splice(k, 1);
            }
            zombieShot = true;
            return true;
          }
          return false;
        })
        if (zombieShot) break;
      }

      // diagonal down
      iDia = i
      jDia = j
      while (true) {
        iDia++;
        jDia++;
        if (iDia > lawn.length-1 || jDia > lawn[0].length-1) break

        // check if any zombies on this place
        let zombieShot = false;
        zombiesRows[iDia].some((zombie, k) => {
          if (zombie[0] == lawn[0].length-1 - jDia) {
            zombie[1] -= 1;
            // check if this killed him and remove him if so
            if (zombie[1] <= 0) {
              zombiesRows[iDia].splice(k, 1);
            }
            zombieShot = true;
            return true;
          }
          return false;
        });
        if (zombieShot) break
      }
    }
  if (i === lawn.length-1) {
    i = -1;
    j--;
  }
  i++;
}

// update all zombie moves by one
zombiesRows.forEach((zRow, i) => zRow.forEach(zombie => {
  zombie[0]++;
  let j = lawn[0].length-1-zombie[0];
  // check if zombie got onto some plant
  if (lawn[i][j] !== ' ') lawn[i][j] = ' ';
}))
// update all zombies that are waiting to get on board
zombies.forEach(zombie => {
  zombie[0]--;
})
moves++;

// if zombieRows all empty or any zombie has move to 0
noZombies = zombiesRows.every(zRow => zRow.length === 0) && zombies.length === 0;
const zombieMoveIsZero = zombiesRows.some(zRow => zRow.some(zombie => zombie[0] == lawn[0].length));
if (noZombies || zombieMoveIsZero) break
}
return noZombies ? null : moves;
}*/
