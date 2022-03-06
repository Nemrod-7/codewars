#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
  int x, y;
};

class Graph {
public:
  int width, height;
  vector<vector<char>> Vmap;
  char nul = -1;

  Graph(const string &src) {

    width = src.find('\n'), height = count(src.begin(), src.end(), '\n');
    Vmap.resize(height, vector<char>(width));
    int nxt = 0, c;

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        c = src[nxt];

        if (c == '\n') {
          if (x == 0) {
            nxt++;
            c = src[nxt++];
          } else
            c = ' ';
        } else {
          nxt++;
        }

        Vmap[y][x] = c;
      }
      // cout << endl;
    }
  }

  bool is_inside(Point p) {
    return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
  }
  bool is_border(Point p) {
    return p.x == 0 || p.y == 0 || p.x == width - 1 || p.y == height - 1;
  }

  char &operator[](const Point p) {
    return is_inside(p) ? Vmap[p.y][p.x] : nul;
  }
};

const vector<Point> compass{{1, 0}, {0, 1},  {-1, 0},  {0, -1},
                            {1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

vector<int> neigh(Graph &G, const Point &p, const int id) {
  vector<int> near(4);

  for (auto &dir : compass) {
    Point nxt = {p.x + dir.x, p.y + dir.y};

    if (G[nxt] == '+')
      near[0]++;
    if (G[nxt] == '-')
      near[1]++;
    if (G[nxt] == '|')
      near[2]++;
    if (G[nxt] == id)
      near[3]++;
  }

  return near;
}

string segment(Graph &G, int id, const pair<Point, Point> &bnd) {
  Point p, no = bnd.first, se = bnd.second;
  string os;
  int cell;
  vector<int> near;
  bool flag;
  for (p.y = no.y; p.y <= se.y; p.y++) {

    for (p.x = no.x; p.x <= se.x; p.x++) {
      near = neigh(G, p, id);
      cell = G[p];

      if (near[3] && cell != id) {
        if (cell == '+' && near[3] > 1) {
          if (near[2] > 1)
            cell = '|';
          if (near[1] > 1)
            cell = '-';
        }
        os += cell;
      } else
        os += ' ';
    }
    os.erase(os.find_last_not_of(" ") + 1);
    os += '\n';
  }

  return os;
}
string join(const string &sep, const vector<string> &to_join) {
  string os;

  for (auto &it : to_join)
    os += it + sep;

  return os;
}

string identify(Graph &G, Point p, int id) {

  vector<Point> pile = {p};
  Point up{999, 999}, down{0, 0};

  while (pile.size()) {
    p = pile.back();
    pile.pop_back();
    G[p] = id;

    up.x = min(p.x - 1, up.x), down.x = max(p.x + 1, down.x);
    up.y = min(p.y - 1, up.y), down.y = max(p.y + 1, down.y);

    for (auto &dir : compass) {
      Point nxt = {p.x + dir.x, p.y + dir.y};

      if (G[nxt] == ' ')
        pile.push_back(nxt);
    }
  }

  return segment(G, id, {up, down});
}

vector<string> break_piece(const string &shape) {

  Graph G(shape);
  vector<string> part;
  Point p;
  int id = 0;

  for (p.y = 0; p.y < G.height; p.y++) {
    for (p.x = 0; p.x < G.width; p.x++) {
      if (G.is_border(p) && G[p] == ' ')
        identify(G, p, id++);
    }
  }

  for (p.y = 0; p.y < G.height; p.y++) {
    for (p.x = 0; p.x < G.width; p.x++) {
      if (G[p] == ' ')
        part.push_back(identify(G, p, id++));
    }
  }

  // for (auto &shape : part) cout << shape;

  return part;
}

int main() {

  string &&shape =
      join("\n", {"+------------+", "|            |", "|            |",
                  "|            |", "+------+-----+", "|      |     |",
                  "|      |     |", "+------+-----+"});
  break_piece(shape);
  shape = join("\n", {"+-------------------+--+", "|                   |  |",
                      "|                   |  |", "|  +----------------+  |",
                      "|  |                   |", "|  |                   |",
                      "+--+-------------------+"});
  // display (shape);
  shape = join("\n", {"+-----------------+", "|                 |",
                      "|   +-------------+", "|   |", "|   |", "|   |",
                      "|   +-------------+", "|                 |",
                      "|                 |", "+-----------------+"});

  break_piece(shape);
  // break_piece (shape);

  shape = join("\n",
               {"+----------+", "|          |", "|          |", "|   +---+  |",
                "|   |   |  |", "|   |   +--+", "|   +-+     ", "|     | +--+",
                "|   +-+ |  |", "|   |   |  |", "|   +---+  |", "|          |",
                "+----------+"});
  // break_piece (shape);
  shape = join("\n", {"         +------------+--+      +--+",
                      "         |            |  |      |  |",
                      "         | +-------+  |  |      |  |",
                      "         | |       |  |  +------+  |",
                      "         | |       |  |            |",
                      "         | |       |  |    +-------+",
                      "         | +-------+  |    |        ",
                      " +-------+            |    |        ",
                      " |       |            |    +-------+",
                      " |       |            |            |",
                      " +-------+            |            |",
                      "         |            |            |",
                      "    +----+---+--+-----+------------+",
                      "    |    |   |  |     |            |",
                      "    |    |   |  +-----+------------+",
                      "    |    |   |                     |",
                      "    +----+---+---------------------+",
                      "    |    |                         |",
                      "    |    | +----+                  |",
                      "+---+    | |    |     +------------+",
                      "|        | |    |     |             ",
                      "+--------+-+    +-----+             "});
}
void Test() {
  vector<string> solution = {
      join("\n", {"+------------+", "|            |", "|            |",
                  "|            |", "+------------+"}),
      join("\n", {"+------+", "|      |", "|      |", "+------+"}),
      join("\n", {"+-----+", "|     |", "|     |", "+-----+"})};
}
/*

int neigh (Graph &G, Point &p, int id) {
    int cnt = 0;
    for (auto &dir : compass) {
        Point nxt = {p.x + dir.x, p.y + dir.y};

        if (G[nxt] == id)
            cnt++;
    }

    return cnt;
}

Point search (Graph &G, char c) {
  Point p;
  for (p.y = 0; p.y < G.height; p.y++) {
    for (p.x = 0; p.x < G.width; p.x++) {
      if (G[p] == c) return p;
    }
  }

  return p;
}

bool is_valid (Point p, vector<Point> bnd) {
    if (p.equals(bnd[0])) return false;
    if (p.equals(bnd[1])) return false;
    if (p.equals({bnd[1].x, bnd[0].y})) return false;
    if (p.equals({bnd[0].x, bnd[1].y})) return false;
    return true;
}
void display(const string &shape) {
  int width = shape.find('\n') + 1, height = shape.size() / width;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (x != width - 1)

      cout << shape[y * width + x];
    }
    cout << endl;
  }
}
bool is_free (Graph &G, Point p) { return (G[p] == ' '); }

void mark (Graph &maze, Point &now, int id) {

  Point left = {now.x, now.y - 1}, right = {now.x, now.y + 1};
  Point top = {now.x - 1, now.y}, back = {now.x + 1, now.y};

  maze[now] = id;

  if (is_free (maze, left))  mark (maze,left, id);
  if (is_free (maze, right)) mark (maze,right,id);
  if (is_free (maze, top))   mark (maze,top , id);
  if (is_free (maze, back))  mark (maze,back, id);
}
vector<Point> mkbound (Graph &G, int id) {
  Point p, up {999, 999}, down {0, 0};

  for (p.y = 0; p.y < G.height; ++p.y)
  for (p.x = 0; p.x < G.width; ++p.x)
  if (G[p] == id) {
    up.x = min (p.x - 1, up.x), down.x = max (p.x + 1, down.x);
    up.y = min (p.y - 1, up.y), down.y = max (p.y + 1, down.y);
  }

  return {up, down};
}
*/
