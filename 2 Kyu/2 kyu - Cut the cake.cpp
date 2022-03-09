#include <iostream>
#include <vector>
//#include <tuple>
#include <algorithm>

using namespace std;
using point = pair<int,int>;
//using vertex = tuple<char,int>;
struct vertex {int tile; int id;};

class graph {
    private :
        vertex nul = {'#', -2};
        vector<vertex> grid;
    public :
        int width, height, size = 0;
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

            if (width) { //
                width -= 1;
                size = height * width / pts.size();
            }
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
                    int part = curr[p].id;
                    if (part < 0) cout << '.';
                    else cout << part;
                }
                cout << "\n";
            }
            cout << '\n';
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

    do { right.first++; } while ((grid[right]).tile == '.');
    do { left.first--; } while ((grid[left]).tile== '.') ;

    return {left.first + 1, right.first};
}
pair<int,int> vertic (graph &grid, const point &p) {

    point up = p, dwn = p;

    do { up.second++; } while ((grid[up]).tile == '.') ;
    do { dwn.second--; } while ((grid[dwn]).tile == '.');

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

            if (curr[p].id == -1) {
                curr[p].id = id;
                cnt++;
                if (cnt == curr.size) return true;
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
            if (curr[p].id == -1) {
                curr[p].id = id;
                cnt++;
                if (cnt == curr.size) return true;
            }
        }
    }

    return false;
}

bool isvalid (graph &curr) {
    for (int y = 0; y < curr.height; y++) {
        for (int x = 0; x < curr.width; x++) {
            if (curr[{x,y}].id == -1) return false;
        }
    }
    return true;
}

void dfs (graph curr, graph &res, int id) {

    if (isvalid (curr)) {
        //Display::partition (curr);
        res = curr;
        return ;
    }
    graph right = curr;
    if (scanh (curr, id)) {
        dfs (curr, res, id + 1);
    }

    if (scanv (right, id)) {
         dfs (right, res, id + 1);
    }
    /*

    cout << endl;
    */
    return ;
}
vector<string> format (graph curr) {

    vector<string> part;
    point p = {0,0};
    auto &[x,y] = p;
    int cnt;

    for (int i = 0; i < curr.pts.size(); i++) {
        string segm;

        for (cnt = 0, y = 0; y < curr.height; y++) {
            for (x = 0; x < curr.width; x++) {
                if (curr[p].id == i) {
                    segm += curr[p].tile;
                    cnt++;
                }
            }
            if (cnt && cnt < curr.size) segm += '\n';
        }

        part.push_back (segm);
    }
    return part;
}
vector<string> cut (const string &src) {


    graph curr (src), final;
    point p = {0,0};
    auto &[x,y] = p;

    dfs (curr, curr, 0);
    vector<string> part = format (curr);

    for (auto &it : part) {
        cout << it << "\n\n";
    }
    //Display::partition (final);

    cout << "end";
    return part;
}
vector<string> cut2 (const string &src) {
    int id = 0;
    graph start (src);
    vector<string> part;
    vector<graph> q1 {start};
    int index = 5;

    while (index-->0) {

        graph curr = q1.front(), right = curr;
        q1.pop_back();

        if (isvalid (curr)) break;

        if (scanh (curr, id)) {
            q1.push_back(curr);
            Display::partition (curr);
        }
        if (scanv (right, id)) {
            q1.push_back(right);
            Display::partition (right);
        }
        id++;
    }

    return part;
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

void erase (graph &curr, int id) {
    for (int y = 0; y < curr.height; y++) {
    for (int x = 0; x < curr.width; x++) {
      if (curr[{x,y}].id == id) curr[{x,y}].id = -1;
    }
  }
}
