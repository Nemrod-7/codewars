#include <iostream>
#include <vector>
#include <algorithm>

#include <fstream>
#include "../../templates/Assert.hpp"

using namespace std;

using point = pair<int,int>;
struct cell {int type; int id;};

class graph {
    private :
        cell nul = {'#', -2};
        std::vector<cell> grid;
    public :
        int width, height, segsize, size;
        std::vector<point> comb;

        graph (const std::string &src = {}) {

            width = src.find ('\n')  + 1, height = count (begin(src), end(src), '\n') + 1;
            size = count (begin(src), end(src), 'o');

            for (size_t i = 0; i < src.size(); i++) {
                if (src[i] != '\n')
                    grid.push_back ({src[i], -1});

            }

            if (width) {
                width -= 1;
                segsize = height * width / size;
                int i = width;

                do {
                    if (segsize % i == 0 && segsize / i <= height)
                        comb.push_back ({i, segsize / i});
                } while (i-->1);
            }
        }

        bool is_inside (const point &p) {
            return p.first >= 0 && p.second >= 0 && p.first < width && p.second < height;
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

graph write (graph curr, point p, int width, int height, int id) {

    auto &[x,y] = p;
    int starty = p.second, startx = p.first;

    for (y = starty; y < starty + height; y++) {
        for (x = startx; x < startx + width; x++) {
            curr[p].id = id;
        }
    }
    return curr;
}
bool scan (graph &curr, point p, int width, int height) {

    auto &[x,y] = p;
    int starty = p.second, startx = p.first;
    int cnt = 0;

    if (startx + width > curr.width || starty + height > curr.height) return false;

    for (y = starty; y < starty + height; y++) {
        for (x = startx; x < startx + width; x++) {
            cell cow = curr[p];
            if (cow.type == 'o') cnt++;
            if (cow.id != -1) return false;
        }
    }
    return cnt == 1 ? true : false;
}
point skip (graph &curr, point p) {

  auto &[x,y] = p;

  for (y = 0; y < curr.height; y++) {
      for (x = 0; x < curr.width; x++) {
          if (curr[p].id == -1) return p;
      }
  }
  return {0,0};
}

void search (graph curr, point p, graph &sector, int id) {

    if (id == curr.size  && sector.size == 0) {
        sector = curr;
        return;
    }

    if (curr[p].id != -1) p = skip (curr, p);

    for (auto &[col, row] : curr.comb) {
        if (scan (curr, p, col, row)) {
            graph next = write (curr, p, col, row, id);
            search (next, {p.first + col, p.second}, sector, id + 1);
        }
    }
}
std::vector<std::string> cut (const std::string &src) {

    graph curr (src), area;
    point p = {0,0};
    auto &[x,y] = p;
    std::vector<std::string> parts;

    search (curr, p, area, 0);

    for (int id = 0, cnt; id < area.size; id++) {
        std::string segm;

        for (cnt = 0, y = 0; y < area.height; y++) {
            for (x = 0; x < area.width; x++) {
                if (area[p].id == id) {
                    segm += area[p].type;
                    cnt++;
                }
            }
            if (cnt && cnt < area.segsize) segm += '\n';
        }
        parts.push_back (segm);
    }

    return parts;
}

int main () {

    string 	cake;

    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,111,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46};


    cake =
		".o.o....\n"
		"........\n"
		"....o...\n"
		"........\n"
		".....o..\n"
		"........";

    cut (cake);

    // Test ();

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

		Assert::That (cut(std::string(cake)), result);
    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};
    cut (cake);

    cake = {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};
    cut (cake);

    cake = {46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,10,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,46,111,46,46,46,46,46,46,46,46,46,46,46,46,46,10,46,46,46,46,46,46,46,46,46,46,111,46,46,111,46,46,46,46,46,46,46,46,46,46,111,46,111,46,46,46,46,46};

    cut (cake);
    /*

    */
}
