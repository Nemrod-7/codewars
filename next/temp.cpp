#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>

///////////////////////////////////////chess.hpp////////////////////////////////////
using namespace std;
using u64 = unsigned long int;

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
                   //
enum {black, white};
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
pair<int,int> notation (const string &txt) {
    char id = txt[0];
    int x = txt[1] - 97, y = 7 - (txt[2] - 49);
    // printf("%c %i %i\n", id, x, y);
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

class Board {
    private :

    public :
        vector<vector<u64>> grid;

        Board() { // create an empty new board
            grid = {
                {0,0,0,0,0,0},
                {0,0,0,0,0,0}
            };
        }

        bool is_inside (int x, int y) { return x >= 0 and y >= 0 and x < 8 and y < 8; }
        void place(const string &txt) {
            int color = white;
            auto [piece, curr] = notation(txt);
            grid[color][piece] |= 1UL << curr;
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
        int player_id (int color, int pos) const {

            for (int i = 0; i < 6; i++) {
                if (bit::chk(grid[color][i], pos)) return i;
            }

            return -1;
        }

        int cntboard_proto (vector<u64> &black, vector<u64> &white) {

            int total = 0;

            for (int i = 0; i < 6; i++) {
                total += (bit::cnt(black[i]) - bit::cnt(white[i])) * score(i);
            }

            return total;
        }

};

class display {
    private :
        inline static map<int, string> utblack = { {pawn, "♙"}, {rook, "♖"}, {bishop, "♗"}, {knight, "♘"}, {queen, "♕"}, {king, "♔"} };
        inline static map<int, string> utwhite = { {pawn, "♟"}, {rook, "♜"}, {bishop, "♝"}, {knight, "♞"}, {queen, "♛"}, {king, "♚"} };

    public :
        static void board (const Board &curr) {
            cout << "\n";
            for (unsigned long i = 0; i < 64; i++) {
                int x = i % 8, y = 7 - i / 8;
                int pla = curr.player_id(white, i), opp = curr.player_id(black, i);

                if (pla >= 0) {
                    cout << display::utwhite[pla];
                } else if (opp >= 0) {
                    cout << display::utblack[opp];
                } else {
                    cout << (((x + y) % 2) ? "□" : " ");
                }
                cout << " ";
                if (x == 7) cout << "\n";
            }
        }
        static void limited (const Board &curr) {
            cout << "\n";
            map<int, string> disblack = { {pawn, "p"}, {rook, "r"}, {bishop, "b"}, {knight, "n"}, {queen, "q"}, {king, "k"} };
            map<int, string> diswhite = { {pawn, "P"}, {rook, "R"}, {bishop, "B"}, {knight, "N"}, {queen, "Q"}, {king, "K"} };

            for (unsigned long i = 0; i < 64; i++) {
                int x = i % 8, y = 7 - i / 8;
                int pla = curr.player_id(white, i), opp = curr.player_id(black, i);

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main () {

    Board board;

    string txt = "Kc8";
    auto [piece, place] = notation("Kc8");
   
    board.grid[black][piece] |= 1UL << place;
    board.place("Ke8");
    board.place("Rh7");

    u64 grid = 0;
    int curr = 0;
    int kg = bit::pos(board.grid[black][king])[0];
    vector<node> hist;

    for (int i = 0; i < 6; i++) {
        grid |= board.grid[white][i];
    }

    do {
        if (grid & 1) {
            int x = curr % 8, y = curr / 8, mode = 0, type = -1;
            vector<int> direction;

            if (bit::chk(board.grid[white][pawn], curr)) {
                type = pawn, mode = 1, direction = {-8};
                if ((curr / 8) == 6) direction = {-8, -16};
            } else if (bit::chk(board.grid[white][rook], curr)) {
                type = rook, mode = 8, direction = compass;
            } else if (bit::chk(board.grid[white][king], curr)) {
                type = king, mode = 1, direction = complete;
            } else if (bit::chk(board.grid[white][queen], curr)) {
                type = queen, mode = 8, direction = complete;
            } else if (bit::chk(board.grid[white][bishop], curr)) {
                type = bishop, mode = 8, direction = diagonal;
            } else if (bit::chk(board.grid[white][knight], curr)) {
                type = knight, mode = 1, direction = {-17,-15,-10,-6,6,10,15,17};
            }

            for (auto &dir : direction) {
                int dx = dir % 8, dy = dir / 8;

                for (int j = 1; j <= mode; j++) {
                    int nx = x + dx * j, ny = y + dy * j;
                    // int nxt = curr + dir * j;

                    if (board.is_inside(nx,ny)) {
                        int next = idx(nx,ny);
                        if (board.player_id(white, next) >= 0) break;

                        // printf("%i %i\n", next, idx(nx,ny));
                        int opp = score(board.player_id(black, next)); // score of black piece taken, if any
                        // int heur = heuristic[type][next] - heuristic[type][curr] + cntboard(black, white);
                //         // int dist = 10 - distance(nx,ny, kg % 8, kg / 8); // distance from white king
                //         // countpoint(black, white);
                //
                //         hist.push_back( {heur, curr, next});
                //         if (opp >= 0) break;
                    }
                }
            }
        }

        curr++;
    } while (grid >>= 1);





    display::limited(board);
}
