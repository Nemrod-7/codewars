#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;
using point = pair<int,int>;
using vertex = tuple<char,int>;

class graph {
    private :
        vertex nul = {'#', -2};
        vector<vertex> grid;
    public :
        int width, height, size;
        vector<point> pts;

        graph (const string &src = {}) {

            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;

            for (int i = 0; i < src.size(); i++) {
                if (src[i] != '\n') {
                    grid.push_back ({src[i], -1});
                    if (src[i] == 'o')
                        pts.push_back({i % width, i / width});
                }
            }

            width -= 1;
            size = height * width / pts.size();
        }

        bool is_inside (const point &p) {
            return p.first >= 0 && p.second >= 0 && p.first < width && p.second < height;
        }
        vertex &operator[] (const point &p) { return is_inside (p) ? grid[p.second * width + p.first] : nul; }
};

class Display {
    public :
        static void showpoint (point p) {
            cout << "[" << p.first << "," << p.second << "]" << endl;
        }
        static void partition (graph &curr) {

            point p = {0,0};
            auto &[x,y] = p;

            for (y = 0; y < curr.height; y++) {
                for (x = 0; x < curr.width; x++) {
                    cout << get<1>(curr[p]);
                }
                cout << "\n";
            }
        }
};

string join (const string &sep, const vector<string> &to_join) {
    string os;

    for (auto &it : to_join)
        os += it + sep;

    return os;
}

pair<int,int> horiz (graph &grid, const point &p) {

    point right = p, left = p;

    do { right.first++; } while (get<0>(grid[right]) == '.');
    do { left.first--; } while (get<0>(grid[left]) == '.') ;

    return {left.first + 1, right.first};
}
pair<int,int> vertic (graph &grid, const point &p) {

    point up = p, dwn = p;

    do { up.second++; } while (get<0>(grid[up]) == '.') ;
    do { dwn.second--; } while (get<0>(grid[dwn]) == '.');

    return {dwn.second + 1, up.second};
}
bool scanv (graph &curr, int id) {
    point p = curr.pts[id];
    auto &[x,y] = p;
    auto [up, dwn] = vertic (curr, p);
    int left = 0, right = 99, cnt = 0;

    for (y = up; y < dwn; y++) {
        pair<int,int> bndx = horiz (curr, p);
        left = max (left, bndx.first), right = min (right, bndx.second);
    }

    for (x = left; x < right; x++) {
        for (y = up; y < dwn; y++) {

            if (get<1>(curr[p]) == -1) {
                if (cnt == curr.size) return true;
                get<1>(curr[p]) = id;
                cnt++;
            }
        }
    }
    return false; // return {{left,up},{right,dwn}};
}
bool scanh (graph &curr, int id) {
    point p = curr.pts[id];
    auto &[x,y] = p;
    auto [left, right] = horiz (curr, p);
    int up = 0, dwn = 99, cnt = 0;

    for (x = left; x < right; x++) {
        pair<int,int> bndy = vertic (curr, p);
        up = max (up, bndy.first), dwn = min (dwn, bndy.second);
    }

    for (y = up; y < dwn; y++) {
        for (x = left; x < right; x++) {
            if (get<1>(curr[p]) == -1) {
                if (cnt == curr.size) return true;
                get<1>(curr[p]) = id;
                cnt++;
            }
        }
    }

    return false;
}

bool isvalid (graph &curr) {
    for (int y = 0; y < curr.height; y++) {
        for (int x = 0; x < curr.width; x++) {
            if (get<1>(curr[{x,y}]) == -1) return false;
        }
    }
    return true;
}
void erase (graph &curr, int id) {
    for (int y = 0; y < curr.height; y++) {
        for (int x = 0; x < curr.width; x++) {
            if (get<1>(curr[{x,y}]) == id) get<1>(curr[{x,y}]) = -1;
        }
    }
}
bool backtrack (graph &curr, int id) {

    if (isvalid (curr)) return true;



    return false;
}
vector<string> cut (const string &src) {

    vector<string> parts;

    graph curr (src);

    scanv (curr, 0);
    scanh (curr, 1);
    scanh (curr, 2);
    scanh (curr, 3);


    Display::partition (curr);

    return parts;
}

int main () {

    string cake =
    "........\n"
    "..o.....\n"
    "...o....\n"
    "........";


    cake =
    ".o......\n"
    "......o.\n"
    "....o...\n"
    "..o.....";

    cake =
    ".o.o....\n"
    "........\n"
    "....o...\n"
    "........\n"
    ".....o..\n"
    "........";



    cut (cake);
    cake =
    ".o....o.\n"
    ".o....o.\n"
    "........\n"
    "o..oo..o";


    //cout << ((curr.height * (curr.width - 1)) - seeds.size()) / seeds.size();




}
void Tes () {
        std::string cake =
				"........\n"
				"..o.....\n"
				"...o....\n"
				"........";

				cake =
				".o......\n"
				"......o.\n"
				"....o...\n"
				"..o.....";

				cake =
				".o.o....\n"
				"........\n"
				"....o...\n"
				"........\n"
				".....o..\n"
				"........";


				cake =
				".o.o....\n"
				".o.o....\n"
				"........\n"
				"........";


				cake =
				".o....o.\n"
				".o....o.\n"
				"........\n"
				"o..oo..o";

				// A complex example ;-)
			  cake =
				"................\n"
				".....o..........\n"
				"................\n"
				"...............o\n"
				"................\n"
				"................\n"
				"................\n"
				".....o..o.....o.\n"
				"................\n"
				"................\n"
				"...o............\n"
				"................\n"
				"................\n"
				"...............o\n"
				"................\n"
				".o..............";

}
