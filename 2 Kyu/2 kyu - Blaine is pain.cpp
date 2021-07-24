#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>

#include <list>
#include <chrono>
#include <thread>

using namespace std;

pair<int,int> operator+ (const pair<int,int> &a, const pair<int,int> &b) {
    return {a.first + b.first, a.second + b.second};
}
pair<int,int> operator- (const pair<int,int> &a, const pair<int,int> &b) {
    return {a.first - b.first, a.second - b.second};
}
void operator+= (pair<int,int> &a, const pair<int,int> &b) {
    a.first += b.first, a.second += b.second;
}
void operator-= (pair<int,int> &a, const pair<int,int> &b) {
    a.first -= b.first, a.second -= b.second;
}

// if (i % 2 == 0) // cardina
// if (i % 2 == 1) // corners
// if (i % 4 == 0)  // N -> S
// if (i % 4 == 1)  // NE -> SW
// if (i % 4 == 2) // E -> W
// if (i % 4 == 3) // SE -> NW

                                //    0      1      2    3      4    5       6      7
                                //    N      NE     E    SE     S    SW      W      NW
const vector<pair<int,int>> direct {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
vector<map<char,bool>> base {{{'\\',1},{'/',1},{'x',1},{'|',1},{'+',1}},{{'/',1}},
                             {{'/',1},{'/',1},{'x',1},{'-',1},{'+',1}},{{'\\',1}}};

class train {
    public :
        pair<int,int> curr, prev;
        char id;
        int unit;
        list<pair<int,int>> wagon;

        void move2 (const pair<int,int> p) {
            wagon.push_front (curr);
            wagon.pop_back();
            prev = curr;
            curr += p;
        }
};

class graph {
    public :
        size_t width, height;
        map<pair<int,int>,char> vmap;

        graph (string src) {
            width = 0, height = count (begin(src), end(src), '\n');
            size_t x = 0, y = 0;

            for (int i = 0; i < src.size(); i++, x++) {
                pair<size_t, size_t> p {x, y};
                if (src[i] == '\n') {
                    width = max (x, width);
                    y++, x = -1;
                } else {
                   if (src[i] != ' ')
                        vmap[p] = src[i];
                }
            }
        }

        char operator[] (const pair<int,int> &p) { return vmap[p] ? vmap[p] : ' ';}
};

class Display {
    public :
        static void track (graph &G) {
          pair<int,int> p;
          for (p.second = 0; p.second < G.height; p.second++) {
              for (p.first = 0; p.first < G.width; p.first++) {
                    cout << G[p];
                }
                cout << "\n";
            }
        }
        static void rail (graph &G, train &a) {
            cout << "\033c";
            pair<int,int> p;
            for (p.second = 0; p.second < G.height; ++p.second) {
                for (p.first = 0; p.first < G.width; ++p.first) {
                    if (a.curr == p)
                        cout << a.id;
                    else if (find (begin(a.wagon), end(a.wagon), p) != end(a.wagon))
                        cout << 'a';  //tolower (a.id);
                    else
                        cout << G[p];
                }
                cout << "\n";
            }
            this_thread::sleep_for(100ms);
        }
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << ',' << p.second << "]";
        }
};

pair<int,int> get_origin (graph &track) {
    pair<int,int> p;
    for (p.second = 0; p.second < track.height; ++p.second) {
        for (p.first = 0; p.first < track.width; ++p.first) {
            if (track[p] != ' ') return p;
        }
    }
    return {0,0};
}
bool is_valid (graph &track, train &a, int index) {
    pair<int,int> next = a.curr + direct[index];
    char curr = track[a.curr], last = track[a.prev];

    if (base[index % 4][track[next]] == false) return false;
    if (next == a.prev) return false;

    if (curr == '+') {
        if (last != '-' ) curr = '|';
        else curr = '-';
    }


    switch (curr) {
        case 's' : break;
        case '|' : if (index % 4 == 0) return true; break;
        case '-' : if (index % 4 == 2) return true; break;
        default  : return true; break;
    }
    return false;
    //base[i % 4][track[next]]
}

train mk_train (graph &track, const string &src, int moves) {
    train out;
    int size = src.size() - 1;
    out.curr = get_origin (track);
    out.unit = size;

    for (int i = 5; i < direct.size(); i++) {
          pair<int, int> nxt = out.curr + direct[i];
          if (base[i % 4][track[nxt]]) {
              out.prev = nxt;
          }
    }
    pair<int,int> dir, p;
    while (moves-->0) {
        for (int i = 0; i < direct.size(); i++) {
            if (is_valid (track, out, i))
                dir = direct[i];

        }
        out.move2 (dir); //dir = direct[i];
    }

    p = out.curr;
    if (isupper (src.front())) {
        out.id = src.front();

        while (size-->0)
            for (int i = 0; i < 4; i++) {
                  pair<int, int> nxt = p + direct[i];
                  if (base[i % 4][track[nxt]]) {
                      p += direct[i];
                      out.wagon.push_back (p);
                  }
            }

    } else {
        out.id = src.back();

        while (size-->0)
            for (int i = 5; i < direct.size(); i++) {
                  pair<int, int> nxt = p + direct[i];
                  //cout << track[nxt];
                  if (base[i % 4][track[nxt]]) {
                      p += direct[i];
                      out.wagon.push_back (p);
                  }
            }
    }
    out.prev = out.wagon.front();
    //Display::rail (track, out);
    return out;
}

int train_crash (const string &src, const string &a_train, int a_train_pos, const string &b_train, int b_train_pos, int limit) {

    graph track (src);
    pair<int,int> dir;
    train A = mk_train (track, b_train, b_train_pos);

    int index = 50;

    while (true) {
        for (int i = 0; i < direct.size(); i++) {
            if (is_valid (track, A, i))
                dir = direct[i];
        }
        A.move2 (dir); //dir = direct[i];
        Display::rail (track, A);
    }
    //A.prev = make_pair (31,1);
        //update (track, A);

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
