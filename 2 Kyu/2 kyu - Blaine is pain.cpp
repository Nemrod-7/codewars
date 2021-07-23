#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>

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

                                //  0       1      2      3      4    5     6     7
                                //     N      NE     E    SE     S    SW      W      NW
const vector<pair<int,int>> direct {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
// if (i % 2 == 0) // corners
// if (i % 2 == 1) // cardinals
// if (i % 4 == 2) // horizontal
//if (i % 4 == 0)  // vertical
//const pair<int, int> vert {0,1}, horiz { 1,0};
map<char, pair<int,int>> base {{'-', {1,0}}, {'|', {0,1}}, {'\\', {1,1}}, {'/', {-1,1}}};

const vector< pair<int,int>> compass {{1,0},{0,1},{-1,0},{0,-1}};
const vector< pair<int,int>> corner {{1,1},{-1,1},{-1,-1},{1,-1}};

struct train {
    pair<int,int> curr, prev;
    char id;
    int unit;

    void move (const pair<int,int> &p) {
        prev = curr;
        curr = p;
    }
    void move2 (const pair<int,int> p) {
        pair<int,int> tmp = prev;
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
                    //cout << x << ' ';
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
train mk_train (graph &track, const string &src) {
    train out;
    out.curr = get_origin (track);
    out.unit = src.size() - 1;
    if (isupper(src[0])) {
        out.id = src[0];
    }

    return out;
}

train update (graph &track, train &x) {
    char curr = track[x.curr], last = track[x.prev];
    pair<int,int> dir;

    switch (curr) {
        case '+' :
            if (last != '-' ) curr = '|';
            else curr = '-';

        case '/' :
            if (track[x.curr + base[curr]] == curr) { x.move2 (base[curr]); return x; }

            for (auto &p : compass) {
                pair<int,int> next = x.curr + p;
                if (track[next] != ' ' && next != x.prev)
                    dir = p;
            }
            x.move2 (dir);
            break;
        case '-' :

            for (int i = 0; i < direct.size(); i++) {
                pair<int,int> next = x.curr + direct[i];

                if (i % 4 == 2) {
                    if (track[next] != ' ' && next != x.prev) {
                        dir = direct[i];
                    }
                }
            }
            x.move2 (dir);
            break;
        case '|' :
            for (int i = 0; i < direct.size(); i++) {
                pair<int,int> next = x.curr + direct[i];

                if (i % 4 == 0) {
                    if (track[next] != ' ' && next != x.prev) {
                        dir = direct[i];
                    }
                }
            }

            x.move2 (dir);
            break;
        case '\\' :
            for (int i = 0; i < direct.size(); i++) {
                pair<int,int> next = x.curr + direct[i];

                if (track[next] != ' ' && next != x.prev) {
                    dir = direct[i];
                }
            }

        /*
            if (track[x.curr + base['\\']] == curr) { x.move2 (base['\\']); return x; }

            for (auto &p : compass) {
                pair<int,int> next = x.curr + p;
                if (track[next] != ' ' && next != x.prev) {
                    dir = p;
                }
            }
            */
            x.move2 (dir);
            break;

        default : break;
    }

    return x;
}
int train_crash(const string &src, const string &a_train, int a_train_pos, const string &b_train, int b_train_pos, int limit) {

    graph track (src);
    pair<int,int> origin = get_origin (track);
    train A = mk_train (track, a_train);
    A.prev = {33,0};
    //Display::point (origin);
    int index = 50;
    //A.curr = make_pair (55,19), A.prev = make_pair (54,19);
    while (index-->0) {
        update (track, A);
        // Display::rail (track, A);
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
