#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

using point = pair<int,int>;
struct cell {int type; int id;};

class graph {
    private :
        cell nul = {'#', -2};
        vector<cell> grid;
    public :
        int width, height, capacity = 0;
        vector<point> seeds;

        graph (const string &src = {}) {

            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;

            for (size_t i = 0; i < src.size(); i++) {
                if (src[i] != '\n') {
                    grid.push_back ({src[i], -1});

                    if (src[i] == 'o')
                        seeds.push_back({i % width, i / width});
                }
            }

            //vert = seeds;
            //sort (vert.begin(), vert.end());

            if (width) {
                width -= 1;
                capacity = height * width / seeds.size();
            }
        }

        bool is_inside (const point &p) {
            return p.first >= 0 && p.second >= 0 && p.first < width && p.second < height;
        }
        bool is_free (const point &p) {
            if (!is_inside(p)) return false;
            cell v = grid[p.second * width + p.first];
            return v.type == '.' && v.id == -1;
        }
        cell &operator[] (const point &p) { return is_inside (p) ? grid[p.second * width + p.first] : nul; }
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

bool isvalid (graph &curr) {
  for (int y = 0; y < curr.height; y++)
  for (int x = 0; x < curr.width; x++) {
    if (curr[make_pair (x,y)].id == -1)
    return false;
  }
  return true;
}

pair<int,int> scan_h (graph &grid, const point &p) {

    point right = p, left = p;

    do { right.first++; } while (grid.is_free (right));
    do { left.first--; } while (grid.is_free (left)) ;

    return {left.first + 1, right.first};
}
pair<int,int> scan_v (graph &grid, const point &p) {

    point up = p, dwn = p;

    do { up.second++; } while (grid.is_free (up)) ;
    do { dwn.second--; } while (grid.is_free (dwn));

    return {dwn.second + 1, up.second};
}

int get_id (graph &curr) {
  int maxv = 0;
  for (int y = 0; y < curr.height; y++) {
      for (int x = 0; x < curr.width; x++) {
          maxv = max (curr[{x,y}].id, maxv);
      }
  }

  return maxv;
}

bool lateralcut3 (graph &curr, point p, int id) {

  auto &[x,y] = p;
  auto [left, right] = scan_h (curr, p);
  int up = 0, dwn = 99;

  for (x = left; x < right; x++) { // scan for highest possible horizontal rectangle
      pair<int,int> bndy = scan_v (curr, p);
      up = max (up, bndy.first), dwn = min (dwn, bndy.second);
  }

  int size = curr.capacity, height = dwn - up, width = right - left;
  if (height * width < size) return false;

  for (int i = 1; i <= width; i++) { // reduce size to fit sector capacity
      if (size % i == 0) {
          height = size / i;
      }
  }

  while ((up + height - 1) < p.second) up++; // if seed isnt visible then adjust

  for (y = up; y < up + height; y++) {  // write sector id
      for (x = left; x < left + width; x++) {
          curr[p].id = id;
      }
  }

  return true;
}
bool verticalcut3 (graph &curr, point p, int id) {

  auto &[x,y] = p;
  auto [up, dwn] = scan_v (curr, p);
  int left = 0, right = 99;

  for (y = up; y < dwn; y++) { // scan for highest possible vertical rectangle
      pair<int,int> bndx = scan_h (curr, p);
      left = max (left, bndx.first), right = min (right, bndx.second);
  }
  int size = curr.capacity, height = dwn - up, width = right - left;
  if (height * width < size) return false;

  for (int i = 1; i <= height; i++) { // reduce to fit sector capacity
      if (size % i == 0 /* && size / i <= width */) {
          dwn = up + i, right = left + size / i;
      }
  }

  for (y = up; y < dwn; y++) { // write sector id
      for (x = left; x < right; x++) {
            curr[p].id = id;
        }
  }

  return true;
}

void search (graph curr, vector<graph> &res, int id) {

    if (isvalid (curr) && !res.size()) {
        res.push_back(curr);
        return ;
    }
    graph vert = curr;
    point p = curr.seeds[id];
    if (lateralcut3 (curr, p, id)) {
        search (curr, res, id + 1);
    }

    if (verticalcut3 (vert, p, id)) {
        search (vert, res, id + 1);
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
                    segm += curr[p].type;
                    cnt++;
                }
            }
            if (cnt && cnt < curr.capacity) segm += '\n';
        }
        if (cnt) part.push_back (segm);
    }

    return part;
}
vector<string> cut (const string &src) {
  /*
  cout << "\n";
  for (auto &it : src)
      cout << static_cast<int> (it) << ',';
  cout << '\n';
  */

    // cout << src << "\n";

    graph curr (src);
    vector<graph> sector;

    search (curr, sector, 0); // horizontal segmentation
    sort (curr.seeds.begin(), curr.seeds.end());
    search (curr, sector, 0);  // vertical segmentation

    if (sector.size() == 0) return {};

    /*
    for (auto &it : sector) {
        Display::partition (it);
        //cout << "\n";
    }
    */
    return format (sector.front());
}

int main () {

    string 	cake;



    //Test ();

    cout << "end";
}

#include "../../templates/Assert.hpp"

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
    /*
    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};
    cut (cake);

    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};
    cut (cake);

    cake = {46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,111,46,46,111,46,46,46,46,46,46,46,46,46,46,111,46,111,46,46,46,46,46};

    cut (cake);

    */
}


string join (const string &sep, const vector<string> &to_join) {
    string os;

    for (auto &it : to_join)
        os += it + sep;

    return os;
}
bool lateralcut2 (graph &curr, point p, int id) {

  auto &[x,y] = p;
  auto [left, right] = scan_h (curr, p);
  int up = 0, dwn = 99;

  for (x = left; x < right; x++) { // scan for highest possible scan_hontal rectangle
      pair<int,int> bndy = scan_v (curr, p);
      up = max (up, bndy.first), dwn = min (dwn, bndy.second);
  }

  int size = curr.capacity, height = dwn - up, width = right - left; // capcity, height = max height, width =
  if (height * width < size) return false;

  for (int i = 1; i <= width; i++) { // check for valid gri size inside it
      if (size % i == 0 /* && size / i <= height */ ) {
          height = size / i;
          //cout << size / i << ' ';
          // right = left + i;
      }
  }

  //height = dwn - up;
  while ((up + height - 1) < p.second) up++;
  //cout << (up + height - 1) << " ";
  for (y = up; y < up + height; y++) { // write sector id
      for (x = left; x < left + width; x++) {
          curr[p].id = id;
      }
  }

  return true;
}
bool verticalcut2 (graph &curr, point p, int id) {

  auto &[x,y] = p;
  auto [up, dwn] = scan_v (curr, p);
  int left = 0, right = 99;

  for (y = up; y < dwn; y++) { // scan for highest possible scan_val rectangle
      pair<int,int> bndx = scan_h (curr, p);
      left = max (left, bndx.first), right = min (right, bndx.second);
  }
  int size = curr.capacity, height = dwn - up, width = right - left;
  if (height * width < size) return false;

  for (int i = 1; i <= height; i++) { // check for valid gri size inside it
      if (size % i == 0 && size / i <= width) {
          dwn = up + i, right = left + size / i;
      }
  }

  for (y = up; y < dwn; y++) { // write sector id
      for (x = left; x < right; x++) {
            curr[p].id = id;
        }
  }

  return true;
}
