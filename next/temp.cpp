#include <iostream>
#include <vector>
#include <map>

using namespace std;
using u64 = unsigned long int;

enum {pawn, rook, bishop, knight, queen, king};

namespace bit {
    bool chk (u64 num, u64 ix) { return num >> ix &1UL; }
    u64 set (u64 num, u64 ix) { return num |= 1UL << ix; }
    u64 tog (u64 num, u64 ix) { return num ^= 1UL << ix; }
    u64 clr (u64 num, u64 ix) { return num &= ~(1UL << ix); }

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


int player_id (vector<u64> player, int pos) {

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
        static void board (vector<u64> &black, vector<u64> &white) {
            cout << "\n";
            for (unsigned long i = 0; i < 64; i++) {
                int x = i % 8, y = i / 8;
                int ix = i;
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

int main () {

    vector<u64> white = {65280,129, 66,36,16,8};
    vector<u64> black = { 71776119061217280UL, 9295429630892703744UL, 4755801206503243776UL, 2594073385365405696UL, 1152921504606846976UL, 576460752303423488UL };

    vector<vector<string>> grid (8, vector<string> (8, "."));

    white[pawn] ^= 1UL << 8;
    white[pawn] ^= 1UL << 16;

    for (int i = 0; i < 64; i++) {
        int x = i % 8, y = 7 - i / 8;


         
        cout << "[" << x << " " << y << "]";
        if (x == 7) cout << "\n";
    }
    // display::board(black, white);

}
