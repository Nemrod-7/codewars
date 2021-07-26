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

const vector<pair<int,int>> direct {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
vector<map<char,bool>> base {{{'/',1},{'\\',1},{'|',1},{'+',1},{'S',1}},{{'/',1},{'x',1},{'S',1}},
                             {{'/',1},{'\\',1},{'-',1},{'+',1},{'S',1}},{{'\\',1},{'x',1},{'S',1}}};

struct train {
    char id;
    size_t size;
    list<pair<int,int>> trn;

    void move (const pair<int,int> &p) {
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

class Display {
    public :
        static void grph (graph &G, const train &a, const train &b) {
            cout << "\033c";
            pair<int,int> p;
            //auto [aid, size, awagon] = a;

            for (p.second = 0; p.second < G.height; ++p.second) {
                for (p.first = 0; p.first < G.width; ++p.first) {
                    if (a.trn.front() == p)
                        cout << a.id;
                    else if (b.trn.front() == p)
                        cout << b.id;
                    else if (find (begin(a.trn), end(a.trn), p) != end(a.trn))
                        cout << static_cast<char>(tolower (a.id));
                    else if (find (begin(b.trn), end(b.trn), p) != end(b.trn))
                        cout << static_cast<char>(tolower (b.id));
                    else
                        cout << G[p];

                }
                cout << "\n";
            }
            this_thread::sleep_for(100ms);
        }
        static void trn (graph &G, const train &a) {
            cout << "\033c";
            pair<int,int> p;

            for (p.second = 0; p.second < G.height; ++p.second) {
                for (p.first = 0; p.first < G.width; ++p.first) {
                    if (a.trn.front() == p)
                        cout << a.id;

                    else if (find (begin(a.trn), end(a.trn), p) != end(a.trn))
                        cout << static_cast<char>(tolower (a.id));

                    else
                        cout << G[p];

                }
                cout << "\n";
            }
            this_thread::sleep_for(50ms);
        }
        static void point (const pair<int,int> &p) {
            cout << "[" << p.first << ',' << p.second << "]";
        }
        static void test (const string &src, const string &a_t, int a_pos, const string &b_t, int b_pos, int lim) {

            cout << "src = {";
            for (size_t i = 0; i < src.size(); i++) {
                cout << static_cast<int> (src[i]);
                if (i < src.size() - 1) cout << ',';
            }
            cout << "};\n train_crash (src,\"",
            cout << a_t << "\"," << a_pos << ",\"";
            cout << b_t << "\"," << b_pos << "," << lim;
            cout << ");\n" ;
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

bool is_valid (graph &track, pair<int,int> prev, pair<int,int> curr, int index) {

    pair<int,int> next = curr + direct[index];
    char rail = track[curr], last = track[prev];

    if (next == prev) return false;
    if (base[index % 4][track[next]] == false) return false;

    if (rail == '+') {
        if (last != '-' ) rail = '|';
        else rail = '-';
    }

    switch (rail) {
        case 'S' : if (track[prev] == track[next]) return true; break;
        case '|' : if (index % 4 == 0) return true; break;
        case '-' : if (index % 4 == 2) return true; break;
        default  : return true; break;
    }

    return false;
}
void getnext (graph &track, train &x) {
    pair<int,int> prev, curr, nxt;
    for (size_t i = 0; i < direct.size(); i++) {
        curr = x.trn.front(), prev = *std::next (begin(x.trn), 1);

        if (is_valid (track, prev, curr, i)) {
            nxt = direct[i];
        }
    }

    x.move (nxt);
}
pair<int,int> getstart (graph &track, int pos) {
    pair<int,int> origin = get_origin(track);
    train curr {' ', 2, {origin}};

    for (size_t i = 4; i < direct.size(); i++) {
          pair<int, int> nxt = origin + direct[i];
          if (base[i % 4][track[nxt]])
              curr.trn.push_back (nxt);
    }

    while (pos-->0) {
        getnext (track, curr);
    }
    return curr.trn.front();
  }

bool collision (train &a,  train &b) {
    auto &[ia, sa, va] = a;
    auto &[ib, sb, vb] = b;
    list<pair<int,int>>::iterator ita = va.begin(), itb;

    itb = vb.begin();

    while (distance (va.begin(), ita) < sa) {
        itb = vb.begin();
        while (distance (vb.begin(), itb) < sb) {
            if (*ita == *itb) return true;
            itb++;
        }
        ita++;
    }


    return false;
}
void advance (graph &track, train &x, size_t &wait) {

    if (x.id == 'x') getnext (track, x);
    else {
        if (track[x.trn.front()] == 'S' && wait < x.size - 1) {
            wait++;
        } else {
            if (wait > 0) wait = 0;
            getnext (track, x);
        }

    }
}

train mktrain2 (graph &track, const string &src, int pos) {
    pair<int,int> origin = get_origin(track), lim;
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

    for (int i = lim.first; i < lim.second; i++) {
      pair<int, int> nxt = wagon.back() + direct[i];
      if (base[i % 4][track[nxt]])
          wagon.push_back (nxt);
    }

    const pair<int,int> dest = getstart (track, pos);

    while (wagon.front() != dest) {
        getnext (track, curr);
    }

    return curr;
}
int train_crash (const string &src, const string &a_train, int a_train_pos, const string &b_train, int b_train_pos, int limit) {

    graph track (src);
    //Display::test (src, a_train, a_train_pos, b_train, b_train_pos, limit);
    train A = mktrain2 (track, a_train, a_train_pos);
    train B = mktrain2 (track, b_train, b_train_pos);

    size_t sleep = A.size, sleepb = B.size, cnt = 0;

    while (limit-->0) {
        if (collision (A, B)) return cnt;
        Display::grph(track, A,B);
        cnt++;

        advance (track, A, sleep);
        advance (track, B, sleepb);
    }
    /*
    */
    return -1;
}

int main () {

  string src;

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


  int cnt = train_crash (example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
 /*
*/
}

void Test () {

  string src;

  // crash kamikaze
  src = {47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,92,45,45,45,45,45,45,45,45,45,83,45,45,45,45,45,45,45,47,10};
 train_crash (src,"xX",15,"Zzzzzzzzzzzzzz",40,100);

 // Crash_cabooser
 src = {47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,92,45,45,45,45,45,45,45,45,45,83,45,45,45,45,45,45,45,47,10};
 train_crash (src,"aA",10,"bbbbbB",30,200);

 // crash T-bone
 src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
 train_crash (src,"aaaA",0,"bbbbbbbbbbbbbB",30,100);

 // crash T-bone at station
 src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
 train_crash (src,"aaaA",22,"bbbbB",0,100);

 // crash self destruct
 src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,32,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,10};
 train_crash (src,"aA",10,"oooooooooooooooooooooooooO",70,200);

 // crashes tricky
 src = {47,45,45,45,45,92,32,32,32,32,32,47,45,45,45,45,92,32,10,124,32,32,32,32,32,92,32,32,32,47,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,47,32,32,32,92,32,32,32,32,32,124,32,10,92,45,45,45,45,47,32,32,32,32,32,92,45,45,45,45,47,10};
 train_crash (src,"Eeeeeeee",32,"Xxxx",23,100);


  src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
 train_crash (src, "aaaaaA",15,"bbbbbB",5,100);

 src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
 train_crash (src,"aaaaaA",15,"bbbbbB",5,100);

 src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
train_crash (src, "aaaaaA",15,"bbbbbB",5,100);

src = {47,45,45,45,45,92,32,32,32,32,32,47,45,45,45,45,92,32,10,124,32,32,32,32,32,92,32,32,32,47,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,47,32,32,32,92,32,32,32,32,32,124,32,10,92,45,45,45,45,47,32,32,32,32,32,92,45,45,45,45,47,10};
 cout << train_crash (src,"aaaaaA",8,"bbbbbB",20,100); // expected -1



  /*
  train_crash (src, "aaaaaA", 10, "bbbbbB", 30,100);

  src = {47,45,45,45,45,45,45,83,45,45,45,45,45,45,45,45,45,45,92,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,92,45,45,45,45,45,45,45,45,45,45,83,45,45,45,45,45,45,47,10};
  train_crash (src, "aaaaaA", 10, "bbbbbB", 30,100);

  src = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,83,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10}; aaaaaA,bbbbbB,10,20,100
  {47,45,45,45,45,92,32,32,32,32,32,47,45,45,45,45,92,32,10,124,32,32,32,32,32,92,32,32,32,47,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,47,32,32,32,92,32,32,32,32,32,124,32,10,92,45,45,45,45,47,32,32,32,32,32,92,45,45,45,45,47,10};
  train_crash (src, "aaaaaA",8, "bbbbbB",20,100);

*/
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


  int cnt = train_crash (example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
  cout << cnt;

}
