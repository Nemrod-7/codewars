#include <iostream>
#include <vector>
//#include <tuple>
#include <algorithm>

#include "../../templates/Assert.hpp"

using namespace std;

using point = pair<int,int>;
struct vertex {int tile; int id;}; //using vertex = tuple<char,int>;

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
        bool is_free (const point &p) {
            if (!is_inside(p)) return false;
            vertex v = grid[p.second * width + p.first];
            return v.tile == '.' && v.id == -1;
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

    do { right.first++; } while (grid.is_free (right));
    do { left.first--; } while (grid.is_free (left)) ;

    return {left.first + 1, right.first};
}
pair<int,int> vertic (graph &grid, const point &p) {

    point up = p, dwn = p;

    do { up.second++; } while (grid.is_free (up)) ;
    do { dwn.second--; } while (grid.is_free (dwn));

    return {dwn.second + 1, up.second};
}

bool scanv2 (graph &curr, int id) {

  point p = curr.pts[id];
  auto &[x,y] = p;
  auto [up, dwn] = vertic (curr, p);
  int left = 0, right = 99, cnt = 0;

  for (y = up; y < dwn; y++) {
    pair<int,int> bndx = horiz (curr, p);
    left = max (left, bndx.first), right = min (right, bndx.second);
  }

  if ((dwn - up) * (right - left) < curr.size) return false;
  while (((dwn - up) * (right - left)) > curr.size) right--;

  for (x = left; x < right; x++) {
    for (y = up; y < dwn; y++) {
      curr[p].id = id;
      cnt++;
    }
  }

  return true; // return {{left,up},{right,dwn}};
}
bool scanh2 (graph &curr, int id) {

  point p = curr.pts[id];
  auto &[x,y] = p;
  auto [left, right] = horiz (curr, p);
  int up = 0, dwn = 99, cnt = 0;

  for (x = left; x < right; x++) { // scan for highest possible horizontal rectangle
  	pair<int,int> bndy = vertic (curr, p);
  	up = max (up, bndy.first), dwn = min (dwn, bndy.second);
  }

  while (((dwn - up) * (right - left)) > curr.size)
      (dwn - up) > 1 ? dwn-- : right--;

  if ((dwn - up) * (right - left) < curr.size) return false;
  //cout << (dwn - up) * (right - left)  << " " << curr.size << endl;
  for (y = up; y < dwn; y++) {
    for (x = left; x < right; x++) {
      curr[p].id = id;
      cnt++;
    }
  }

  return true;
}

bool isvalid (graph &curr) {
    for (int y = 0; y < curr.height; y++)
        for (int x = 0; x < curr.width; x++) {
            point p = {x,y};
            if (curr[p].id == -1)
                return false;
        }
    return true;
}

void dfs (graph curr, vector<graph> &res, int id) {

    Display::partition (curr);

    if (isvalid (curr)) {
        res.push_back(curr);
        return ;
    }
    graph right = curr;

    if (scanh2 (curr, id)) {
        dfs (curr, res, id + 1);
    }
    if (scanv2 (right, id)) {
         dfs (right, res, id + 1);
    }

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
        if (cnt)
            part.push_back (segm);
    }

    return part;
}

vector<string> cut (const string &src) {

    graph curr (src);
    vector<graph> solutions;
    point p = {0,0};
    auto &[x,y] = p;

    dfs (curr, solutions, 0);

    if (solutions.size() == 0) return {};

    vector<string> part;//
    part = format (solutions.front());

    //cout << part.size();
    /*
    for (auto &it : part) {
        cout << it << "\n\n";
    }
    */

    return part;
}

int main () {


    string 	cake;

    cake =
		".o....o.\n"
		".o....o.\n"
		"........\n"
		"o..oo..o";

    vector<string> part = cut (cake);

    /*
    for (auto &it : part) {
        cout << it << "\n\n";
    }

    Test ();
    */

    cout << "end";
}

void Test () {
  std::string cake;
  std::vector<std::string> result;
    cake =
		"........\n"
		"..o.....\n"
		"...o....\n"
		"........";

		result =
    {
		"........\n"
		"..o.....",

		"...o....\n"
		"........"
    };
		Assert::That (cut(std::string(cake)), result);

		cake =
		".o......\n"
		"......o.\n"
		"....o...\n"
		"..o.....";
		result = {".o......","......o.","....o...","..o....."};
		Assert::That (cut(std::string(cake)), result);

		cake =
		".o.o....\n"
		"........\n"
		"....o...\n"
		"........\n"
		".....o..\n"
		"........";
		result = {
		".o\n"
		"..\n"
		"..\n"
		"..\n"
		"..\n"
		"..",

		".o....\n"
		"......",

		"..o...\n"
		"......",

		"...o..\n"
		"......"
		};
		Assert::That (cut(std::string(cake)), result);

		cake =
		".o.o....\n"
		".o.o....\n"
		"........\n"
		"........";
		result.clear();
		Assert::That (cut(std::string(cake)), result);

		cake =
		".o....o.\n"
		".o....o.\n"
		"........\n"
		"o..oo..o";
		result = {
		".o..",

		"..o.",

		".o..",

		"..o.",

		"..\n"
		"o.",

		"..\n"
		".o",

		"..\n"
		"o.",

		"..\n"
		".o"
		};
		Assert::That (cut(std::string(cake)), result);

    /*
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
		result = {
		"................\n"
		".....o..........",

		"................\n"
		"...............o",

		"........\n"
		"........\n"
		"........\n"
		".....o..",

		"....\n"
		"....\n"
		"....\n"
		"o...\n"
		"....\n"
		"....\n"
		"....\n"
		"....",

		"....\n"
		"....\n"
		"....\n"
		"..o.\n"
		"....\n"
		"....\n"
		"....\n"
		"...." ,

		"........\n"
		"........\n"
		"...o....\n"
		"........",

		"................\n"
		"...............o" ,

		"................\n"
		".o.............."
		};
		//Assert::That (cut(std::string(cake)), result);

    */
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
