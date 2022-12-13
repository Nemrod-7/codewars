#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
// samsic fY65C2X3RQGkQ2b
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
void showforces (vector<vector<cell>> &grid, vector<vector<int>> zmb) {

    const int height = grid.size(), width = grid[0].size();

    ofstream oss ("plant.csv");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto &[id, pt] = grid[y][x];

            switch (id) {
                case grass: oss << ";"; break;
                case plant: oss << pt << ";"; break;
                case multi: oss << "s" << ";"; break;
                default: oss << " " << ";"; break;
            }
        }
        oss << "  ::  ;";
        for (auto vec : zmb) {
            int mov = vec[0], row = vec[1], hp = vec[2];
            if (row == y) oss << hp << ';';
        }
        oss << endl;
    }
    oss << endl;
    oss.close();
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

    int cycles = zombies.front()[0];
    const int height = lawn.size(), width = lawn[0].size();
    vector<vector<cell>> grid = mkgraph(lawn);

    ofstream ofs ("plant.csv");
   // showforces (grid, zombies);
    while (true) {
        int nzomb = 0;
        ofs << "cycle " << cycles  << " :: \n";
        for (auto inv : zombies) {
            int move = inv[0], y = inv[1], hp = inv[2];
            if (move == cycles)
                grid[y][width - 1] = { zombe, hp };
        }
        ofs << tofile(grid);
        //display (grid);
        for (int y = 0; y < height; y++) {
            for (int x = 0, cnt = 0; x < width; x++) {
                auto &[id, pt] = grid[y][x];

                switch (id) {
                    case plant: cnt += pt; break;
                    case multi: disperse (grid,x,y); break;
                    case zombe:
                        if (x == 0) return cycles;

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

        if (nzomb == 0) break;

        for (int y = 0; y < height; y++) {
            for (int x = 1; x < width; x++) {
                auto &[id, pt] = grid[y][x];

                if (id == zombe) {
                    grid[y][x-1] = { grass, 0 };
                    swap (grid[y][x-1], grid[y][x]);
                }
            }
        }
        
        cycles++;
    }

    ofs.close();
    return 0;
}

int main () {

    vector<string> lawn = { "2       ", "  S     ", "21  S   ", "13      ", "2 3     " };
    vector<vector<int>> zombies = { {0,4,28}, {1,1,6}, {2,0,10}, {2,4,15}, {3,2,16}, {3,3,13} };
 
lawn = {"3S1 S         ","61            ","22 SS         ","2111          ","S 3 31        ","1 32          ","2             ","4             "};

zombies = {{0,0,52},{0,1,60},{0,2,52},{0,4,69},{0,5,52},{0,6,17},{0,7,34},{1,3,46},{3,0,27},{3,1,32},{3,4,36},{3,5,27},{3,6,9},{4,3,24},{4,7,21},{6,2,34},{11,0,29},{11,1,34},{11,4,39},{13,5,32},{13,6,11},{14,3,27},{14,7,22},{16,2,39},{17,0,29},{17,1,34},{17,4,39},{17,5,27},{17,6,9},{18,3,23},{19,2,26},{19,7,21},{21,0,26},{21,1,31},{21,4,35},{22,5,29},{22,6,9},{23,3,24},{25,7,23},{26,1,33},{26,4,38},{26,5,26},{26,6,9},{27,0,32},{29,2,44},{29,3,25},{31,7,21},{32,0,27},{32,1,34},{32,2,27},{33,3,22},{33,4,44},{33,5,32},{33,6,11},{37,1,31},{37,7,21}};

   int res = plantzombie1 (lawn, zombies);

    cout << res;
//assertion failed: `(left == right)` left: `31`, right: `25`
//assert_eq!(pnz::plants_and_zombies(&lawn, &zombies), 25);



    cout << "\nend\n";
}

