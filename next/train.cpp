#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <vector>
#include <map>
#include <list>
#include <algorithm>

using namespace std;

using pixel = std::pair<wchar_t,int>;
const pixel empty = {' ', 0};
// #define update() printf("\033[H\033[J")
// #define gotoxy(x, y) printf("\033[%d;%dH", x, y)

//////////////////////////////////////screen////////////////////////////////////
class Display {
    private :
        static inline struct winsize win;
        static inline int width, height;
        static inline wchar_t *prev, *buffer;

        static bool isinside (int x, int y) { return x >= 0 && y >= 0 && x < width && y < height; }
        static void mark (int x, int y, wchar_t ch) {
            if (isinside(x,y)) buffer[y * width + x] = ch;
        }

    public :
        static void init() {
            // Disable iostream sync and increase stdout buffering:
            std::ios::sync_with_stdio(false); // for effiency
            std::cin.tie(nullptr);
            setvbuf(stdout, nullptr, _IOFBF, 1<<20);

            ioctl(0, TIOCGWINSZ, &win); // get terminal height and width
            height = win.ws_row - 1, width = win.ws_col;
            buffer = new wchar_t[width * height];
            // prev = new wchar_t[width * height];
            clear();
        }

        static void render() {
            // std::wcout << "\033c";

            std::wcout << buffer << std::flush;
            this_thread::sleep_for(100ms);
        }
        static void clear () {
            // std::fill_n (prev, width * height, ' ');
            std::fill_n (buffer, height * width, ' ');
        }
        static void draw(const std::string &sprite) {
            int nx = 0, ny = 0;

            for (auto &ch : sprite) {
                if (ch == '\n') {
                    nx = 0, ny++;
                } else {
                    mark (nx, ny, ch);
                    nx++;
                }
            }
        }
};

////////////////////////////////////////////////////////////////////////////////

pair<int,int> operator+ (const pair<int,int> &a, const pair<int,int> &b) {
    return {a.first + b.first, a.second + b.second};
}

const vector<pair<int,int>> direct {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

vector<map<char,bool>> base {
  {{'|',1},{'+',1},{'S',1}},
  {{'/',1},{'X',1},{'S',1}},
  {{'-',1},{'S',1}},
  {{'\\',1},{'X',1},{'S',1}}};

struct train {
    char id;
    size_t size;
    list<pair<int,int>> trn;

    void move (const pair<int,int> p) {
        trn.push_front (trn.front() + p);
        if (trn.size() > size) trn.pop_back();
    }
};

class graph {
    public :
        int width, height;
        map<pair<int,int>,char> vmap;

        graph (string src) {
            width = 0, height = count (begin(src), end(src), '\n');
            int x = 0, y = 0;

            for (size_t i = 0; i < src.size(); i++, x++) {
                pair<int, int> p {x, y};
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

string stringify (graph &G, const train &a, const train &b) {
    pair<int,int> p;
    string img;

    for (p.second = 0; p.second < G.height; ++p.second) {
        for (p.first = 0; p.first < G.width; ++p.first) {
            char pixel = G[p];

            if (a.trn.front() == p)
                pixel = a.id;
            else if (b.trn.front() == p)
                pixel = b.id;
            else if (find (begin(a.trn), end(a.trn), p) != end(a.trn))
                pixel = static_cast<char>(tolower (a.id));
            else if (find (begin(b.trn), end(b.trn), p) != end(b.trn))
                pixel = static_cast<char>(tolower (b.id));

            img.push_back (pixel);
        }
        img.push_back ('\n');
    }

    return img;
}

pair<int,int> get_origin (graph &track) {
    pair<int,int> p;
    for (p.second = 0; p.second < track.height; ++p.second) {
        for (p.first = 0; p.first < track.width; ++p.first) {
            if (track[p] != ' ') return p;
        }
    }
    return {0,0};
}

bool is_valid (graph &track, pair<int,int> &prev, pair<int,int> &curr, int index) {

    pair<int,int> next = curr + direct[index];
    if (next == prev) return false;
    char rail = track[curr], last = track[prev], frwd = track[next];

    index %= 4;

    switch (rail) {
        case '+' : if ((last == '-' && frwd == '-') || (last != '-' && index == 0)) return true; break;
        case 'X' : if (last == frwd) return true; break;
        case 'S' : if (last == frwd) return true; break;
        case '|' : if (index == 0) return true; break;
        case '-' : if (index == 2) return true; break;
        default  : return base[index][frwd]; break;
    }

    return false;
}
void getnext (graph &track, train &x) {
    pair<int,int> prev = *std::next (begin(x.trn), 1), curr = x.trn.front();

    for (size_t i = 0; i < direct.size(); i++) {
        if (is_valid (track, prev, curr , i))
            x.move (direct[i]);
    }
}
pair<int,int> getstart (graph &track, int pos) {
    pair<int,int> origin = get_origin(track);
    train curr {'X', 2, {origin}};

    for (size_t i = 4; i < direct.size(); i++) {
          pair<int, int> nxt = origin + direct[i];

          if (is_valid (track, origin, origin, i))
              curr.trn.push_back (nxt);
    }

    while (pos-->0)
        getnext (track, curr);

    return curr.trn.front();
}

void advance (graph &track, train &x, size_t &wait) {

    if (x.id == 'X') getnext (track, x);
    else {
        if (track[x.trn.front()] == 'S' && wait < x.size - 1) {
            wait++;
        } else {
            if (wait > 0) wait = 0;
            getnext (track, x);
        }

    }
}
train mktrain (graph &track, const string &src, int pos) {

    const pair<int,int> origin = get_origin (track);
    pair<int,int> lim;
    train curr;
    auto &[id, size, wagon] = curr;

    if (isupper(src.front())) {
        id = src.front();
        lim.first = 0, lim.second = 3;
    } else {
        id = src.back();
        lim.first = 4, lim.second = direct.size();
    }

    size = src.size();
    wagon.push_front (origin);

    for (int i = lim.first; i < lim.second; i++) {            //set clockwise or counterclockwise
        pair<int, int> nxt = wagon.back() + direct[i];

        if (base[i % 4][track[nxt]])
            wagon.push_back (nxt);
    }

    while (wagon.size() < size) {                             // make wagons
        for (size_t i = 0; i < direct.size(); i++) {
            pair<int,int> prv = *std::prev(wagon.end(), 2);

            if (is_valid (track, prv, wagon.back(), i) && wagon.size() < size) {
                wagon.push_back (wagon.back() + direct[i]);
            }
        }
    }

    if (isupper(src.front())) {
        pair<int,int> dest = getstart (track, pos);

        while (curr.trn.front() != dest) {                           // go to start point
            getnext (track, curr);
        }

    } else {

        while (pos-->0)
            getnext (track, curr);
    }

    return curr;
}

bool collision (train a,  train b) {
    auto &[ia, sa, va] = a;
    auto &[ib, sb, vb] = b;

    list<pair<int,int>>::iterator starta = va.begin(), startb = vb.begin();
    map<pair<int,int>, int> mapa, mapb;

    for (auto ita = starta; distance (starta, ita) < sa; ita++) {
        for (auto itb = startb; distance (startb, itb) < sb; itb++) {
            if (*ita == *itb) return true;
        }
    }

    for (auto &p : va) {
        mapa[p]++;
        if (mapa[p] > 1) return true;
    }

    for (auto &p : vb) {
        mapb[p]++;
        if (mapb[p] > 1) return true;
    }

    return false;
}
int train_crash (const string &src, const string &a_train, int a_train_pos, const string &b_train, int b_train_pos, int limit) {
    Display::init();
    graph track (src);
    train A = mktrain (track, a_train, a_train_pos);
    train B = mktrain (track, b_train, b_train_pos);

    size_t sleep = A.size, sleepb = B.size, cnt = 0;
    // string img = draw(track,A,B);
    if (collision (A, B)) return 0;

    while (limit-->0) {
        advance (track, A, sleep);
        advance (track, B, sleepb);

        Display::draw(stringify(track,A,B));
        Display::render();

        cnt++;
        if (collision (A, B)) return cnt;
    }
    return -1;
}

int main() {
  const std::string track =
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


    train_crash(track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
    cout << "\nexit\n";
}
