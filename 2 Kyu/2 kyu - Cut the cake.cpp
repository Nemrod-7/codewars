#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using point = pair<int,int>;

class graph {
    private :
        string grid;
    public :
        int width, height;
        vector<point> pts;

        graph (const string &src = {}) : grid (src) {
            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;
        }
        bool is_inside (const point &p) {
            return p.first >= 0 && p.second >= 0 && p.first < width && p.second < height;
        }
        char operator[] (const point &p) { return is_inside (p) ? grid[p.second * width + p.first] : '#'; }
};

string join (const string &sep, const vector<string> &to_join) {
    string os;

    for (auto &it : to_join)
        os += it + sep;

    return os;
}

vector<string> cut (const string &src) {

    vector<string> part;


    return part;
}

struct rect {
    point up, down;
};

int main () {

    string cake =
    ".o.o....\n"
    "........\n"
    "....o...\n"
    "........\n"
    ".....o..\n"
    "........";

    graph curr (cake);

    point p = {0,0};
    auto &[x,y] = p;

    vector<point> coord;

    for (y = 0; y < curr.height; y++) {
        for (x = 0; x < curr.width - 1; x++) {
            if (curr[p] == 'o') coord.push_back (p);
            //cout << curr[p];
        }
        //cout << "\n";
    }

    p = coord[0];

    while (true) {
        x++;
        if (curr[p] != '.') break;
    }


}
