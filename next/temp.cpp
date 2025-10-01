#include <iostream>
#include <vector>
#include <map>

using namespace std;
using u64 = unsigned long int;

enum {white, black};
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

int idx (int x, int y) { return x + y * 8; }
int player_id (vector<u64> player, int pos) {

    for (int i = 0; i < 6; i++) {
        if (bit::chk(player[i], pos)) return i;
    }

    return -1;
}

pair<int,int> notation (const string &txt) {
    char id = txt[0];
    int x = txt[1] - 97, y = txt[2] - 49;

    switch (id) {
      case 'P' : return {pawn, idx(x,y)}; break;
      case 'R' : return {rook, idx(x,y)}; break;
      case 'K' : return {king, idx(x,y)}; break;
      case 'Q' : return {queen, idx(x,y)}; break;
      case 'B' : return {bishop, idx(x,y)}; break;
      case 'N' : return {knight, idx(x,y)}; break;
    }
    return {};
}



class Board {
private :
  public :
      vector<vector<u64>> grid;

      Board() { // create an empty new board
          grid = {
              {0,0,0,0,0,0},
              {0,0,0,0,0,0}
          };
          // black = {0,0,0,0,0,0};
          // white = {0,0,0,0,0,0};
      }

      void place(const string &txt) {
          int color = white;
          auto [piece, index] = notation(txt);
          grid[color][piece] |= 1UL << index;
      }

      void create() { // create a new board all pieces in place
        grid = {{
            0xff00,
            0x0081,
            0x0042,
            0x0024,
            0x0010,
            0x0008 }, {
            0x00ff000000000000,
            0x8100000000000000,
            0x4200000000000000,
            0x2400000000000000,
            0x1000000000000000,
            0x0800000000000000 }};
      }
};
class display {
    private :
        // inline static map<int, string> black = { {pawn, "♙"}, {rook, "♖"}, {bishop, "♗"}, {knight, "♘"}, {queen, "♕"}, {king, "♔"} };
        // inline static map<int, string> white = { {pawn, "♟"}, {rook, "♜"}, {bishop, "♝"}, {knight, "♞"}, {queen, "♛"}, {king, "♚"} };

    public :
        // static void board (const vector<u64> &black, const vector<u64> &white) {
        //     cout << "\n";
        //     for (unsigned long i = 0; i < 64; i++) {
        //         int x = i % 8, y = 7 - i / 8;
        //         int ix = x + y * 8;
        //         int pla = player_id(white, ix), opp = player_id(black, ix);
        //
        //         if (pla >= 0) {
        //             cout << display::white[pla];
        //         } else if (opp >= 0) {
        //             cout << display::black[opp];
        //         } else {
        //             cout << (((x + y) % 2) ? "□" : " ");
        //         }
        //         cout << " ";
        //         if (x == 7) cout << "\n";
        //     }
        // }
        static void limited (const Board &curr) {
            cout << "\n";
            map<int, string> disblack = { {pawn, "p"}, {rook, "r"}, {bishop, "b"}, {knight, "n"}, {queen, "q"}, {king, "k"} };
            map<int, string> diswhite = { {pawn, "P"}, {rook, "R"}, {bishop, "B"}, {knight, "N"}, {queen, "Q"}, {king, "K"} };

            for (unsigned long i = 0; i < 64; i++) {
                int x = i % 8, y = 7 - i / 8;
                int ix = x + y * 8;
                int pla = player_id(curr.grid[white], ix), opp = player_id(curr.grid[black], ix);

                if (pla >= 0) {
                    cout << diswhite[pla];
                } else if (opp >= 0) {
                    cout << disblack[opp];
                } else {
                    cout << ".";
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

    Board chess;

    string txt = "Kc8";
    auto [piece, index] = notation("Kc8");

    chess.grid[black][piece] |= 1UL << index;
    // chess.place("Ke8");
    // chess.place("Rh7");


    display::limited(chess);


    // for (int i = 0; i < 64; i++) {
    //     int x = i % 8, y = 7 - i / 8;
    //
    //     cout << "[" << x << " " << y << "]";
    //     if (x == 7) cout << "\n";
    // }

}
