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
            iss << setw(2);
            switch (id) {
                case grass: iss << " "; break;
                case plant: iss << "p" ; break;
                case multi: iss << "s"; break;
                case zombe: iss << pt; break;
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

string tofile (vector<vector<cell>> grid) {

    const int height = grid.size(), width = grid[0].size();
    stringstream ofs;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            switch (id) {
                case grass: ofs << " "; break;
                case plant: ofs << pt; break;
                case multi: ofs << "s"; break;
                case zombe: ofs << pt; break;
                default: break;
            }
            ofs << ";";
        }
        ofs << endl;
    }
    ofs << endl;

    return ofs.str();
}

int plantzombie1 (vector<string> lawn, vector<vector<int>> zombies) {

    int cycles = zombies.front()[0], minc = zombies.back()[0];
    const int height = lawn.size(), width = lawn[0].size();
    vector<vector<cell>> grid = mkgraph(lawn);
    stringstream iss;
    //ofstream ofs ("plant.csv");
    //cout << showforces (grid, zombies);

    for (auto inv : zombies) {
        int move = inv[0], row = inv[1], hp = inv[2];
        cout << move << " " << row << " " << hp << endl;
    }
    cout << display (grid);
    while (true) {
        int nzomb = 0;
        for (auto inv : zombies) {
            int move = inv[0], y = inv[1], hp = inv[2];
            if (move == cycles)
                grid[y][width - 1] = { zombe, hp };
        }
        cout << "cycle " << cycles  << "\n";
        //cout << display (grid);
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

int main () {

    int res;
    vector<string> lawn = { "2       ", "  S     ", "21  S   ", "13      ", "2 3     " };
    vector<vector<int>> zombies = { {0,4,28}, {1,1,6}, {2,0,10}, {2,4,15}, {3,2,16}, {3,3,13} };

lawn = {"3S1 S         ","61            ","22 SS         ","2111          ","S 3 31        ","1 32          ","2             ","4             "};

zombies = {{0,0,52},{0,1,60},{0,2,52},{0,4,69},{0,5,52},{0,6,17},{0,7,34},{1,3,46},{3,0,27},{3,1,32},{3,4,36},{3,5,27},{3,6,9},{4,3,24},{4,7,21},{6,2,34},{11,0,29},{11,1,34},{11,4,39},{13,5,32},{13,6,11},{14,3,27},{14,7,22},{16,2,39},{17,0,29},{17,1,34},{17,4,39},{17,5,27},{17,6,9},{18,3,23},{19,2,26},{19,7,21},{21,0,26},{21,1,31},{21,4,35},{22,5,29},{22,6,9},{23,3,24},{25,7,23},{26,1,33},{26,4,38},{26,5,26},{26,6,9},{27,0,32},{29,2,44},{29,3,25},{31,7,21},{32,0,27},{32,1,34},{32,2,27},{33,3,22},{33,4,44},{33,5,32},{33,6,11},{37,1,31},{37,7,21}};
//assertion failed: `(left == right)` left: `31`, right: `25`
//assert_eq!(pnz::plants_and_zombies(&lawn, &zombies), 25);

   lawn = {"31S 1      ","3  1       ","21 21      "," 21 1      ","41  1      "," 131       "};
   zombies = {{0,0,29},{0,2,29},{1,2,19},{1,3,21},{2,0,22},{2,1,23},{2,3,13},{2,5,29},{3,1,14},{3,4,37},{3,5,18},{8,0,17},{8,1,10},{8,2,18},{8,3,11},{8,4,23},{8,5,12},{11,0,15},{11,2,16},{11,3,10},{11,4,18},{14,1,13},{14,4,16},{14,5,16},{15,0,16},{15,2,17},{15,3,11},{16,1,10},{16,5,13}};
   //res = plantzombie1 (lawn, zombies);
   //cout << res;

   lawn = {"S211     ","1132     ","1 22     ","3 S      ","2        ","1121     ","4        ","41 2     "};
   zombies = {{0,0,20},{0,1,28},{0,2,20},{0,3,16},{0,4,8},{0,5,20},{0,6,16},{2,2,13},{2,3,10},{2,5,13},{2,6,10},{2,7,31},{3,1,22},{3,4,6},{3,7,20},{4,0,18},{4,2,12},{4,3,9},{4,5,12},{4,6,9},{4,7,14},{6,1,18},{6,4,5},{7,0,14},{7,3,9},{7,5,11},{7,6,9},{7,7,14},{9,1,16},{9,4,4},{10,0,12},{10,2,15},{10,3,9},{10,5,10},{10,6,9},{11,2,11},{11,7,18},{12,0,11},{12,5,10}};
   res = plantzombie1 (lawn, zombies);
   // assertion failed: `(left == right)` left: `13`, right: `0`

   // assertion failed: `(left == right)` left: `14`, right: `19`
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
