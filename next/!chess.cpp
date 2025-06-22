#include <iostream>
#include <vector>
#include <map>
#include <random>

using namespace std;
using i64 = long int;
using u64 = unsigned long int;

enum {black, white};
enum {pawn, rook, bishop, knight, queen, king};

struct node { int alt, now, nxt; };

const vector<int> compass = {-8, 1, 8, -1}, diagonal = {-9, 9, 7, -7};
const vector<int> complete = {-8, 1, 8, -1, -9, 9, 7, -7};

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
                   
namespace bit {
    bool chk (i64 num, i64 ix) { return num >> ix &1L; }
    i64 set (i64 num, i64 ix) { return num |= 1L << ix; }
    i64 tog (i64 num, i64 ix) { return num ^= 1L << ix; }
    i64 clr (i64 num, i64 ix) { return num &= ~(1L << ix); }

    i64 cnt (i64 num) {
        i64 cnt = 0;

        do { cnt += num &1; } while (num >>= 1);

        return cnt;
    }
    vector<int> pos(i64 num) {
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

int player_id (vector<u64> player, int pos) {

    for (int i = 0; i < 6; i++) {
        if (bit::chk(player[i], pos)) return i;
    }

    return -1;
}
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
node rnd_walk (const vector<node> &vs) {

    uniform_int_distribution<int> dist (0, vs.size() - 1);

    return vs[dist(gen)];
}
int heuristic (vector<u64> &black, vector<u64> &white) {

    int total = 0;

    for (int i = 0; i < 6; i++) {
        total += (bit::cnt(black[i]) - bit::cnt(white[i])) * score(i);
    }

    return total;
}

vector<node> get_moves(vector<u64> &black, vector<u64> &white) {

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
            int x = index % 8, y = index / 8, mode = 0;
            vector<int> direction;
            
            if (bit::chk(black[pawn], index)) { 
                mode = 1, direction = {8};
                if ((index / 8) == 1) direction = {8, 16};
            } else if (bit::chk(black[rook], index)) {
                mode = 8, direction = compass;
            } else if (bit::chk(black[king], index)) {
                mode = 1, direction = complete;
            } else if (bit::chk(black[queen], index)) {
                mode = 8, direction = complete;
            } else if (bit::chk(black[bishop], index)) {
                mode = 8, direction = diagonal;
            } else if (bit::chk(black[knight], index)) {
                mode = 1, direction = {-17,-15,-10,-6,6,10,15,17};
            }

            for (auto &dir : direction) {
                int dx = dir % 8, dy = dir / 8;
                
                for (int j = 1; j <= mode; j++) {
                    int nx = x + dx * j, ny = y + dy * j;

                    if (nx >= 0 and ny >= 0 and nx < 8 and ny < 8) {
                        int nxt = index + dir * j;
                        if (player_id(black, index + dir * j) >= 0) break;

                        int opp = score(player_id(white, nxt)); // score of white piece taken, if any
                        int dist = distance(nx,ny, kg % 8, kg / 8); // distance from white king

                        hist.push_back( {opp + dist, index, nxt});
                        if (opp >= 0) break;
                    }
                }
            }
        }

        index++;
    } while (grid >>= 1);

    return hist;
}

class display {
    private :
        inline static map<int, string> white = { {pawn, "♙"}, {rook, "♖"}, {bishop, "♗"}, {knight, "♘"}, {queen, "♕"}, {king, "♔"} };
        inline static map<int, string> black = { {pawn, "♟"}, {rook, "♜"}, {bishop, "♝"}, {knight, "♞"}, {queen, "♛"}, {king, "♚"} };

    public :
        static void board (vector<u64> &black, vector<u64> &white) {
            cout << "\n";
            for (unsigned long i = 0; i < 64; i++) {
                int x = i % 8, y = i / 8;
                int pla = player_id(white, i), opp = player_id(black, i);

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
int main () {

    string play = "Ka1";

    char piece = play[0];
    int x = play[1] - 97, y = play[2] - 49;
    //cout << x <<  " " <<  y;

    vector<u64> black = {65280,129, 66,36,16,8};
    vector<u64> white = { 71776119061217280UL, 9295429630892703744UL, 4755801206503243776UL, 2594073385365405696UL, 1152921504606846976UL, 576460752303423488UL };

    int index = 10;

    while (index-->0) {
        auto [score, now, nxt] = rnd_walk( get_moves(black,white));
        int type = player_id(black, now);

        cout << type << " " << now << '\n';
        black[type] ^= 1UL << now;
        black[type] ^= 1UL << nxt;
        display::board(black, white);
    }


}
