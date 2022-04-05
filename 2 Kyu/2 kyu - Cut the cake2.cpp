#include <iostream>
#include <fstream>
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
        vector<point> seed;

        graph (const string &src = {}) {

            width = src.find ('\n') + 1, height = count (begin(src), end(src), '\n') + 1;

            for (size_t i = 0; i < src.size(); i++) {
                if (src[i] != '\n') {
                    grid.push_back ({src[i], -1});

                    if (src[i] == 'o')
                        seed.push_back({i % width, i / width});
                }
            }

            //vert = seed;
            //sort (vert.begin(), vert.end());

            if (width) {
                width -= 1;
                capacity = height * width / seed.size();
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
      static void file (const string &src) {

          ofstream os ("partition.csv");

          for (auto &it : src) {
              os << it;
              if (it != '\n') os << ',';

          }
          os.close();
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

void write (graph &curr, int up, int left, int height, int width, int id) {

    for (int y = up; y < up + height; y++) { // write sector id
        for (int x = left; x < left + width; x++) {
            curr[{x,y}].id = id;
        }
    }
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

vector<point> dimension (int width, int height, int size) {

    vector<point> dimens;

    for (int i = 1; i <= height; i++) {
        if (size % i == 0  && size / i <= width ) {
            dimens.push_back({i, size / i});
        }
    }

    return dimens;
}

bool lateralcut4 (graph curr, point p, int id) {

  auto &[x,y] = p;
  auto [left, right] = scan_h (curr, p);
  int width = right - left, height, up = 0, dwn = 99;
  int size = curr.capacity;

  for (x = left; x < right; x++) { // scan for highest possible horizontal rectangle
      pair<int,int> bndy = scan_v (curr, p);
      up = max (up, bndy.first), dwn = min (dwn, bndy.second);
      height = dwn - up;
  }
    vector<point> dimens = dimension (width, height, curr.capacity);

    for (auto &[row, col] : dimens) {
        cout << col << " " << row << endl;
        write (curr, up, left, row, col, id);

        Display::partition(curr);

        write (curr, up, left, row, col, -1);
        /*
        */
    }
    /*
    width = right - left;

    while ((up + height - 1) < p.second) up++; // if seed isnt visible then adjust
    for (y = up; y < up + height; y++) {  // write sector id
        for (x = left; x < left + width; x++) {
            curr[p].id = id;
        }
    }
    */
    //Display::partition(curr);
    return true;
}
bool verticalcut4 (graph curr, point p, int id) {

  auto &[x,y] = p;
  auto [up, dwn] = scan_v (curr, p);
  int left = 0, right = 99;

  for (y = up; y < dwn; y++) { // scan for highest possible vertical rectangle
      pair<int,int> bndx = scan_h (curr, p);
      left = max (left, bndx.first), right = min (right, bndx.second);
  }
  int height = dwn - up, width = right - left;

  if (height * width < curr.capacity) return false;
  vector<point> dimens = dimension (height, width, curr.capacity);

  for (auto &[col, row] : dimens) {
      cout << col << " " << row << endl;

      write (curr, up, left, row, col, id);

      Display::partition(curr);

      write (curr, up, left, row, col, -1);
  }

  return true;
}

void search (graph curr, point p, int id) {

}

vector<string> format (graph curr) {

    point p = {0,0};
    auto &[x,y] = p;
    int cnt;
    vector<int> hist (curr.seed.size(),0), order;
    vector<string> part;

    for (y = 0; y < curr.height; y++) {    // get sector order
        for (x = 0; x < curr.width; x++) {

            if (hist[curr[p].id] == 0) {
                order.push_back(curr[p].id);
                hist[curr[p].id] = 1;
            }
        }
    }

    for (auto &id : order) {
        string segm;

        for (cnt = 0, y = 0; y < curr.height; y++) {
            for (x = 0; x < curr.width; x++) {
                if (curr[p].id == id) {
                    segm += curr[p].type;
                    cnt++;
                }
            }
            if (cnt && cnt < curr.capacity) segm += '\n';
        }
        part.push_back (segm); //if (cnt) part.push_back (segm);
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

     cout << src << "\n";
    //Display::file (src);
    graph curr (src);
    vector<graph> sector;

    //lateralcut4 (curr, curr.seed[0], 0);
    //sort (curr.seed.begin(), curr.seed.end());
    verticalcut4 (curr, curr.seed[0], 0);

    /*
    search (curr, sector, 0); // horizontal segmentation
    search (curr, sector, 0);  // vertical segmentation
    */
    if (sector.size() == 0) return {};


    for (auto &it : sector) {
        Display::partition (it);
        //cout << "\n";
    }
    /*
    */
    return format (sector.front());
}

int main () {

    string 	cake;

    cake =
		".o......\n"
		"......o.\n"
		"....o...\n"
		"..o.....";

    cut (cake);


    cake = {46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,111,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46};

    cake = {46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,10,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111};
    //cut (cake);

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



    //Test ();

    cout << "end";
}

//#include "../../templates/Assert.hpp"

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
    /*
    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};
    cut (cake);

    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};
    cut (cake);

    cake = {46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,111,46,46,111,46,46,46,46,46,46,46,46,46,46,111,46,111,46,46,46,46,46};

    cut (cake);

    */
}
