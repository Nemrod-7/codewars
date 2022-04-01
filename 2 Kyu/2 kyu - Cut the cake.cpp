#include <iostream>

#include <vector>
#include <algorithm>

#include "../../templates/Assert.hpp"
using namespace std;

using point = pair<int,int>;
struct vertex {int tile; int id;};

class graph {
    private :
        vertex nul = {'#', -2};
        vector<vertex> grid;
    public :
        int width, height, secsize = 0;
        vector<point> seeds, seeds2;

        graph (const string &src = {}) {

            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;

            for (size_t i = 0; i < src.size(); i++) {
                if (src[i] != '\n') {
                    grid.push_back ({src[i], -1});

                    if (src[i] == 'o')
                        seeds.push_back({i % width, i / width});
                }
            }

            seeds2 = seeds;
            sort (seeds2.begin(), seeds2.end());

            if (width) {
                width -= 1;
                secsize = height * width / seeds.size();
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
            cout << "\n\n";
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

bool isvalid (graph &curr) {
    for (int y = 0; y < curr.height; y++)
        for (int x = 0; x < curr.width; x++) {
            if (curr[make_pair (x,y)].id == -1)
                return false;
        }
    return true;
}
bool scanh3 (graph &curr, int id) {

  point p = curr.seeds[id];
  auto &[x,y] = p;
  auto [left, right] = horiz (curr, p);
  int up = 0, dwn = 99;

  for (x = left; x < right; x++) { // scan for highest possible horizontal rectangle
      pair<int,int> bndy = vertic (curr, p);
      up = max (up, bndy.first), dwn = min (dwn, bndy.second);
  }

  int size = curr.secsize, height = dwn - up, width = right - left;
  if (height * width < size) return false;

  for (int i = 1; i <= width; i++) { // check for valid grid size inside it
      if (size % i == 0 && size / i <= height) {
          dwn = up + size / i, right = left + i;
      }
  }

  for (y = up; y < dwn; y++) { // write sector id
      for (x = left; x < right; x++) {
          curr[p].id = id;
      }
  }

  return true;
}
bool scanv3 (graph &curr, int id) {

  point p = curr.seeds2[id];
  auto &[x,y] = p;
  auto [up, dwn] = vertic (curr, p);
  int left = 0, right = 99;

  for (y = up; y < dwn; y++) { // scan for highest possible vertical rectangle
      pair<int,int> bndx = horiz (curr, p);
      left = max (left, bndx.first), right = min (right, bndx.second);
  }
  int size = curr.secsize, height = dwn - up, width = right - left;
  if (height * width < size) return false;
  //cout << height * width << endl;
  for (int i = 1; i <= height; i++) { // check for valid grid size inside it
      if (size % i == 0 && size / i <= width) {
          dwn = up + i, right = left + size / i;
      }
  }

  for (y = up; y < dwn; y++) { // write sector id
      for (x = left; x < right; x++) {
            curr[p].id = id;
        }
  }
  return true; // return {{left,up},{right,dwn}};
}
void dfs (graph horiz, vector<graph> &res, int id) {

    if (isvalid (horiz) && !res.size()) {
        res.push_back(horiz);
        return ;
    }
    graph vert = horiz;

    if (scanh3 (horiz, id)) {
        dfs (horiz, res, id + 1);
    }

    if (scanv3 (vert, id)) {
        dfs (vert, res, id + 1);
    }

    return ;
}
void dfs2 (graph curr, vector<graph> &res, int id) {

    if (isvalid (curr) && !res.size()) {
        res.push_back(curr);
        return ;
    }
    graph vert = curr;

    if (scanh3 (curr, id)) {
        dfs2 (curr, res, id + 1);
    }

    if (scanv3 (vert, id)) {
        dfs2 (vert, res, id + 1);
    }

    return ;
}
vector<string> format (graph curr) {

    vector<string> part;
    point p = {0,0};
    auto &[x,y] = p;
    int cnt;

    for (size_t i = 0; i < curr.seeds.size(); i++) {
        string segm;

        for (cnt = 0, y = 0; y < curr.height; y++) {
            for (x = 0; x < curr.width; x++) {
                if (curr[p].id == i) {
                    segm += curr[p].tile;
                    cnt++;
                }
            }
            if (cnt && cnt < curr.secsize) segm += '\n';
        }
        if (cnt) part.push_back (segm);
    }

    return part;
}
vector<string> cut (const string &src) {

    graph curr (src);
    vector<graph> sector;


    dfs (curr, sector, 0); // vertical segmentation
    //sort (curr.seeds.begin(), curr.seeds.end());
    //dfs (curr, sector, 0); // horizontal segmentation

    if (sector.size() == 0) return {};

    for (auto &it : sector) {
        Display::partition (it);
        cout << "\n";
    }

    return format (sector.front());
}

bool scanh4 (graph &curr, int id) {

  point p = curr.seeds[id];
  auto &[x,y] = p;
  auto [left, right] = horiz (curr, p);
  int up = 0, dwn = 99;


  for (x = left; x < right; x++) { // scan for highest possible horizontal rectangle
      pair<int,int> bndy = vertic (curr, p);
      //Display::showpoint (p);
      //curr[bndy].id = id;
      up = max (up, bndy.first), dwn = min (dwn, bndy.second);
  }

  int size = curr.secsize, height = dwn - up, width = right - left;
  //cout << height * width;
  if (height * width < size) return false;

  for (int i = 1; i <= width; i++) { // check for valid grid size inside it
      if (size % i == 0 && size / i <= height) {
          dwn = up + size / i, right = left + i;
      }
  }

  for (y = up; y < dwn; y++) { // write sector id
      for (x = left; x < right; x++) {
          curr[p].id = id;
      }
  }
  /*
  */

  return true;
}
bool scanv4 (graph &curr, int id) {

  point p = curr.seeds[id];
  auto &[x,y] = p;
  auto [up, dwn] = vertic (curr, p);
  int left = 0, right = 99;

  for (y = up; y < dwn; y++) { // scan for highest possible vertical rectangle
      pair<int,int> bndx = horiz (curr, p);
      left = max (left, bndx.first), right = min (right, bndx.second);
  }
  int size = curr.secsize, height = dwn - up, width = right - left;
  if (height * width < size) return false;
  //cout << height * width << endl;
  for (int i = 1; i <= height; i++) { // check for valid grid size inside it
      if (size % i == 0 && size / i <= width) {
          dwn = up + i, right = left + size / i;
      }
  }

  for (y = up; y < dwn; y++) { // write sector id
      for (x = left; x < right; x++) {
            curr[p].id = id;
        }
  }
  return true; // return {{left,up},{right,dwn}};
}

int main () {

    string 	cake;

    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};

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


    cake =
		".o.o....\n"
		"........\n"
		"....o...\n"
		"........\n"
		".....o..\n"
		"........";

    vector<string> part = cut (cake);
    /*
    cout << cake << "\n";
    graph curr (cake);

    for (auto &p : curr.seeds) {
        Display::showpoint (p);
    }

    //cout << cake;
    scanv3 (curr, 2);
    scanv3 (curr, 0);
    scanv3 (curr, 1);

    scanv3 (curr, 0);
    scanv3 (curr, 1);
    Display::partition (curr);
    */
    //scanh3 (curr, 1);

    /*
    for (auto &it : part) {
        cout << it << "\n\n";
    }

    Test ();
    */

    cout << "end";
}


void Test () {
  /*
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
void write (graph &curr, const point &a, const point &b, int id) {
    for (int y = a.second; y < b.second; y++) { // write sector id
        for (int x = a.first; x < b.first; x++) {
              curr[make_pair (x,y)].id = id;
          }
    }
}
bool scanv2 (graph &curr, int id) {

  point p = curr.seeds[id];
  auto &[x,y] = p;
  auto [up, dwn] = vertic (curr, p);
  int left = 0, right = 99, cnt = 0;

  for (y = up; y < dwn; y++) {
    pair<int,int> bndx = horiz (curr, p);
    left = max (left, bndx.first), right = min (right, bndx.second);
  }

  if ((dwn - up) * (right - left) < curr.secsize) return false;
  while (((dwn - up) * (right - left)) > curr.secsize) right--;

  //cout << ((dwn - up) * (right - left))  << " " << curr.size << endl;
  for (x = left; x < right; x++) {
    for (y = up; y < dwn; y++) {
      curr[p].id = id;
      cnt++;
    }
  }

  return true; // return {{left,up},{right,dwn}};
}
bool scanh2 (graph &curr, int id) {

  point p = curr.seeds[id];
  auto &[x,y] = p;
  auto [left, right] = horiz (curr, p);
  int up = 0, dwn = 99, cnt = 0;

  for (x = left; x < right; x++) { // scan for highest possible horizontal rectangle
    pair<int,int> bndy = vertic (curr, p);
    up = max (up, bndy.first), dwn = min (dwn, bndy.second);
  }

  while (((dwn - up) * (right - left)) > curr.secsize) {
      (dwn - up) > 1 ? dwn-- : right--;
  }

  if ((dwn - up) * (right - left) < curr.secsize) return false;
  for (y = up; y < dwn; y++) {
    for (x = left; x < right; x++) {
      curr[p].id = id;
      cnt++;
    }
  }

  return true;
}

bool scanv (graph &curr, int id) {

  point p = curr.seeds[id];
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
        if (cnt == curr.secsize) return true;
      }
    }
  }

  return false; // return {{left,up},{right,dwn}};
}
bool scanh (graph &curr, int id) {

  point p = curr.seeds[id];
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
        if (cnt == curr.secsize) return true;
      }
    }
  }

  return false;
}

string join (const string &sep, const vector<string> &to_join) {
    string os;

    for (auto &it : to_join)
        os += it + sep;

    return os;
}
