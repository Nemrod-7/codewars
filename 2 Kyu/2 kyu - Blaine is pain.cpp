#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

pair<int,int> operator+ (pair<int,int> &a, const pair<int,int> &b) {
    return {a.first + b.first, a.second + b.second};
}
void operator+= (pair<int,int> &a, const pair<int,int> &b) {
    a.first += b.first, a.second += b.second;
}

                                    //  0      1      2     3       4     5     6      7
                                    //  W     NW      N     NE      E     SE    S     SW
const vector<pair<int,int>> compass {{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1}};

using direction = pair<pair<int,int>,pair<int,int>>;
map<char, direction> base;

/*

'|' == compass[2] || compass[6]
'-' == compass[0] || compass[4]

*/

enum {clockwise, counterclockwise};
struct train {
    //pair<int,int> p;
    int unit, way;
};

class graph {
    public :
        size_t width, height;
        map<pair<int,int>,char> vmap;

        graph (string src) {
            width = 0, height = count (begin(src), end(src), '\n');
            size_t x = 0, y = 0;

            for (size_t i = 0, x = 0; i < src.size(); i++, x++) {
                pair<size_t, size_t> p {x, y};
                if (src[i] == '\n') {
                    width = max (x, width);
                    y++, x = 0;
                } else
                    vmap[p] = src[i];
            }
        }
};

class Display {
    public :
        static void graph (graph &G) {
            for (int y = 0; y < G.height; ++y) {
                for (int x = 0; x < G.width; ++x) {
                    cout << G.vmap[{x,y}];
                }
                cout << "\n";
            }
        }
};

train mk_train (const string &src) { return {src.size() - 1, isupper (src[0]) ? clockwise : counterclockwise};}

int train_crash(const string &track, const string &a_train, int a_train_pos, const string &b_train, int b_train_pos, int limit) {

    graph railroad (track);
    train A = mk_train (a_train), B = mk_train (b_train);

    pair<int,int> from, next ;
                          //      E       W
    vector<pair<int,int>> horiz {{0,1}, {0,-1}};
    char tile;
    switch (tile) {

        case '\\' : break;
        case '|'  : break;
        case '-'  : next = horiz[A.way]; break;
        case '/'  : break;
        case '+'  : break;
        case 'X'  : break;

        default: break;
    }

    return 42;
}

int main () {

  const std::string example_track =
          "                                /------------\\\n"
          "/-------------\\                /             |\n"
          "|             |               /              S\n"
          "|             |              /               |\n"
          "|        /----+--------------+------\\        |   \n"
          "\\       /     |              |      |        |     \n"
          " \\      |     \\              |      |        |                    \n"
          " |      |      \\-------------+------+--------+---\\\n"
          " |      |                    |      |        |   |\n"
          " \\------+--------------------+------/        /   |\n"
          "        |                    |              /    | \n"
          "        \\------S-------------+-------------/     |\n"
          "                             |                   |\n"
          "/-------------\\              |                   |\n"
          "|             |              |             /-----+----\\\n"
          "|             |              |             |     |     \\\n"
          "\\-------------+--------------+-----S-------+-----/      \\\n"
          "              |              |             |             \\\n"
          "              |              |             |             |\n"
          "              |              \\-------------+-------------/\n"
          "              |                            |               \n"
          "              \\----------------------------/ \n";


  train_crash (example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);

}
