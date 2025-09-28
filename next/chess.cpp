#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>

using namespace std;
using u64 = unsigned long int;

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
                   
enum {pawn, rook, bishop, knight, queen, king};

struct node { int alt, now, nxt; };

const vector<int> compass = {-8, 1, 8, -1}, diagonal = {-9, 9, 7, -7};
const vector<int> complete = {-8, 1, 8, -1, -9, 9, 7, -7};

const vector<vector<int>> heuristic {
    { // pawn
 0,  0,  0,  0,  0,  0,  0,  0,
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 25, 25, 10,  5,  5,
 0,  0,  0, 20, 20,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0
    },{ // rook
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0
    }, { // bishop
-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20,
    },{ // knight
-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50,
    },{ // queen
-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20
    },{ // king
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20,-20,-20,-20,-20,-20,-10,
 20, 20,  0,  0,  0,  0, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20
    }
};
                   
namespace bit {
    bool chk (u64 num, u64 ix) { return num >> ix &1UL; }
    u64 set (u64 num, u64 ix) { return num | 1UL << ix; }
    u64 tog (u64 num, u64 ix) { return num ^ 1UL << ix; }
    u64 clr (u64 num, u64 ix) { return num & ~(1UL << ix); }

    u64 cnt (u64 num) {
        u64 cnt = 0;

        do { cnt += num &1; } while (num >>= 1);

        return cnt;
    }
    vector<int> pos(u64 num) {
        int ix = 0;
        vector<int> vs;

        do {
            if (num & 1) {
                vs.push_back(num);
            }

            ix++;
        } while (num >>= 1);

        return vs;
    }
};

int distance (int x1, int y1, int x2, int y2) { return abs(x1-x2) + abs(y1-y2); }
bool is_inside (int x, int y) { return x >= 0 and y >= 0 and x < 8 and y < 8; }

int idx (int x, int y) { return x + y * 8; }
int player_id (const vector<u64> &player, int pos) {

    for (int i = 0; i < 6; i++) {
        if (bit::chk(player[i], pos)) return i;
    }

    return -1;
}

class display {
    private :
        inline static map<int, string> black = { {pawn, "♙"}, {rook, "♖"}, {bishop, "♗"}, {knight, "♘"}, {queen, "♕"}, {king, "♔"} };
        inline static map<int, string> white = { {pawn, "♟"}, {rook, "♜"}, {bishop, "♝"}, {knight, "♞"}, {queen, "♛"}, {king, "♚"} };

    public :
        static void board (const vector<u64> &black, const vector<u64> &white) {
            cout << "\n";
            for (unsigned long i = 0; i < 64; i++) {
                int x = i % 8, y = 7 - i / 8;
                int ix = x + y * 8;
                int pla = player_id(white, ix), opp = player_id(black, ix);

                if (pla >= 0) {
                    cout << display::white[pla];
                } else if (opp >= 0) {
                    cout << display::black[opp];
                } else {
                    cout << (((x + y) % 2) ? "□" : " ");
                }
                cout << " ";
                if (x == 7) cout << "\n";
            }
        }

        static string identify (int type) {

            switch (type) {
                case pawn : return "pawn"; break;
                case rook : return "rook"; break; 
                case king : return "king"; break;
                case queen : return "queen"; break;
                case bishop : return "bishop"; break;
                case knight : return "knight";break;
            }

            return "";
        }
};


int score (int type) {

    switch (type) {
        case pawn : return 1 ; break;
        case rook : return 5; break; 
        case bishop : return 3; break;
        case knight : return 3;break;
        case queen : return 8; break;
        case king : return 99; break;
    }

    return 0;
}
node rnd_walk (vector<node> vs) {

    sort(vs.begin(), vs.end(), [](node a, node b) { return a.alt > b.alt; });

    for (auto [alt, now, nxt] : vs) {
        cout<< alt << " ";
    }
    cout << "\n";
    std::geometric_distribution dist;
    // uniform_int_distribution<int> dist (0, vs.size() - 1);

    return vs[dist(gen) % vs.size()];
}

int cntboard (vector<u64> &black, vector<u64> &white) {

    int total = 0;

    for (int i = 0; i < 6; i++) {
        total += (bit::cnt(black[i]) - bit::cnt(white[i])) * score(i);
    }

    return total;
}
vector<node> get_moves (vector<u64> &black, vector<u64> &white) { // v0.0

    u64 grid = 0;
    int kg = bit::pos(white[king])[0];
    int index = 0;
    vector<node> hist;

    for (int i = 0; i < 6; i++) {
        grid |= black[i];
        // grid |= white[i];
    }

    do {
        if (grid & 1) {
            int x = index % 8, y = index / 8, mode = 0, type = -1;
            vector<int> direction;

            if (bit::chk(black[pawn], index)) { 
                type = pawn, mode = 1, direction = {-8};
                if ((index / 8) == 6) direction = {-8, -16};
            } else if (bit::chk(black[rook], index)) {
                type = rook, mode = 8, direction = compass;
            } else if (bit::chk(black[king], index)) {
                type = king, mode = 1, direction = complete;
            } else if (bit::chk(black[queen], index)) {
                type = queen, mode = 8, direction = complete;
            } else if (bit::chk(black[bishop], index)) {
                type = bishop, mode = 8, direction = diagonal;
            } else if (bit::chk(black[knight], index)) {
                type = knight, mode = 1, direction = {-17,-15,-10,-6,6,10,15,17};
            }

            for (auto &dir : direction) {
                int dx = dir % 8, dy = dir / 8;

                for (int j = 1; j <= mode; j++) {
                    int nx = x + dx * j, ny = y + dy * j;

                    if (is_inside(nx,ny)) {
                        int nxt = index + dir * j;
                        if (player_id(black, index + dir * j) >= 0) break;
                        int opp = score(player_id(white, nxt)); // score of white piece taken, if any
                        int heur = heuristic[type][nxt] - heuristic[type][index] + cntboard(black, white);
                        // int dist = 10 - distance(nx,ny, kg % 8, kg / 8); // distance from white king
                        // countpoint(black, white);

                        hist.push_back( {heur, index, nxt});
                        if (opp >= 0) break;
                    }
                }
            }
        }

        index++;
    } while (grid >>= 1);

    return hist;
}

void play (string play, vector<u64> &black, vector<u64> &white) {

    int now = idx(play[0] - 97, play[1] - 49);
    int nxt = idx(play[3] - 97, play[4] - 49);

    int player = player_id(white, now);
    int automa = player_id(black, nxt);

    int x = now % 8, y = now / 8;
    int type, mode;
    vector<int> direction;

    if (bit::chk(white[pawn], now)) { 
        type = pawn, mode = 1, direction = {-8};
        // if ((now / 8) == 1) direction = {-8, -16};
    } else if (bit::chk(white[rook], now)) {
        type = rook, mode = 8, direction = compass;
    } else if (bit::chk(white[king], now)) {
        type = king, mode = 1, direction = complete;
    } else if (bit::chk(white[queen], now)) {
        type = queen, mode = 8, direction = complete;
    } else if (bit::chk(white[bishop], now)) {
        type = bishop, mode = 8, direction = diagonal;
    } else if (bit::chk(white[knight], now)) {
        type = knight, mode = 1, direction = {-17,-15,-10,-6,6,10,15,17};
    }

    for (auto dir : direction) {
        int dx = dir % 8, dy = dir / 8;

        for (int i = 1; i <= mode; i++) {
            int nx = x + dx * i, ny = y + dy * i;

            if (is_inside(nx,ny)) {
                int nxt = now + dir * i;
                if (player_id(white, now + dir * i) >= 0) break;

                int opp = score(player_id(black, nxt)); // score of white piece taken, if any

            }
        }
    }

    white[player] ^= 1UL << now;
    white[player] ^= 1UL << nxt;

    if (automa >= 0) {
        black[automa] ^= 1UL << nxt;
    }
}

int main () {

    vector<u64> white = {
        0xff00, 
        0x0081, 
        0x0042, 
        0x0024, 
        0x0010, 
        0x0008 };

    vector<u64> black = {
        0x00ff000000000000,
        0x8100000000000000,
        0x4200000000000000,
        0x2400000000000000,
        0x1000000000000000,
        0x0800000000000000 };


    int index = 0;

    while (index-->0) {
        auto [score, now, nxt] = rnd_walk( get_moves(black,white));
        int machin = player_id(black, now);
        int player = player_id(white, nxt);

        black[machin] ^= 1UL << now;
        black[machin] ^= 1UL << nxt;

        if (player >= 0) {
            white[player] ^= 1UL << nxt;
        }
        // cout << now << " " << nxt << '\n';
    }

    // play("a2 a4", black, white);

    display::board(black, white);
}
