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

struct node { int type, alt, now, nxt; };

const vector<pair<int,int>> compass = {{0,-1},{1,0},{0,1},{-1,0}};
const vector<pair<int,int>> diagonal = {{-1,-1},{1,-1},{1,1},{-1,1}};
const vector<pair<int,int>> complete = {{0,-1},{1,0},{0,1},{-1,0},{-1,-1},{1,-1},{1,1},{-1,1}};
const vector<pair<int,int>> knight_m = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };
// const vector<int> compass = {-8, 1, 8, -1}, diagonal = {-9, 9, 7, -7};
// const vector<int> complete = {-8, 1, 8, -1, -9, 9, 7, -7};
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
    },
//     { // king
// -30,-40,-40,-50,-50,-40,-40,-30,
// -30,-40,-40,-50,-50,-40,-40,-30,
// -30,-40,-40,-50,-50,-40,-40,-30,
// -30,-40,-40,-50,-50,-40,-40,-30,
// -20,-30,-30,-40,-40,-30,-30,-20,
// -10,-20,-20,-20,-20,-20,-20,-10,
//  20, 20,  0,  0,  0,  0, 20, 20,
//  20, 30, 10,  0,  0, 10, 30, 20
//     },
    {
// king end game
-50,-40,-30,-20,-20,-30,-40,-50,
-30,-20,-10,  0,  0,-10,-20,-30,
-30,-10, 20, 30, 30, 20,-10,-30,
-30,-10, 30, 40, 40, 30,-10,-30,
-30,-10, 30, 40, 40, 30,-10,-30,
-30,-10, 20, 30, 30, 20,-10,-30,
-30,-30,  0,  0,  0,  0,-30,-30,
-50,-30,-30,-30,-30,-30,-30,-50
    }

};

namespace ix {
    const int x[64] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};
    const int y[64] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7};
}
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
                vs.push_back(ix);
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
        // u64 bitboard;
        vector<vector<u64>> grid;
    public :

        Board() { // create an empty new board
            grid = {
                {0,0,0,0,0,0},
                {0,0,0,0,0,0}
            };
        }

        vector<u64> &operator [] (int color) {
            return grid[color];
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

        int count () {

            int total = 0;

            for (int i = 0; i < 6; i++) {
                total += ( bit::cnt(grid[white][i]) - bit::cnt(grid[black][i])) * score(i);
            }

            return total;
        }

        vector<node> get_moves () {
            int curr = 0;
            u64 player = 0;
            u64 enemy = 0;
            vector<node> hist;

            for (int i = 0; i < 6; i++) {
                player |= grid[white][i];
                enemy |= grid[black][i];
            }

            for (int curr = 0; curr < 64; curr++) {
                if (bit::chk(player, curr)) {
                    int x = ix::x[curr], y = ix::y[curr], dist = 0, type = -1;
                    vector<pair<int,int>> direction;
                    // cout << x << " " << y << endl;

                    if (bit::chk(grid[white][pawn], curr)) {
                        type = pawn, dist = 1, direction = {{0,1}};
                        if (y == 6) direction = {{0,1},{0,2}};
                    } else if (bit::chk(grid[white][rook], curr)) {
                        type = rook, dist = 8, direction = compass;
                    } else if (bit::chk(grid[white][king], curr)) {
                        type = king, dist = 1, direction = complete;
                    } else if (bit::chk(grid[white][queen], curr)) {
                        type = queen, dist = 8, direction = complete;
                    } else if (bit::chk(grid[white][bishop], curr)) {
                        type = bishop, dist = 8, direction = diagonal;
                    } else if (bit::chk(grid[white][knight], curr)) {
                        type = knight, dist = 1, direction = knight_m;
                    }

                    for (auto &[dx,dy] : direction) {

                        for (int j = 1; j <= dist; j++) {
                            int nx = x + dx * j, ny = y + dy * j;
                            int next = idx(nx,ny);
                
                            if (is_inside(nx,ny)) {
                                int heur = heuristic[type][next] - heuristic[type][curr];

                                if (bit::chk(player, next)) break;

                                hist.push_back( {type, heur, curr, next});

                                if (bit::chk(enemy, next)) break;
                            }
                        }
                    }
                }
            }

            return hist;
        }
};

class Display {
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
                    cout << Display::utwhite[pla];
                } else if (opp >= 0) {
                    cout << Display::utblack[opp];
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
node rnd_walk (vector<node> &vs) {
    sort(vs.begin(), vs.end(), [](node a, node b) { return a.alt > b.alt; });
    std::geometric_distribution dist;

    return vs[dist(gen) % vs.size()];
}
int threat(Board &board, u64 bitboard, int next) {

    int damage[6] = {0,0,0,0,0,0};

    for (auto &[dx,dy] : knight_m) {
        int sx = ix::x[next] + dx , sy = ix::y[next] + dy;

        if (board.is_inside(sx,sy) && bit::chk(bitboard, sx + sy * 8)) {
            if (bit::chk(board[black][knight], sx + sy * 8)) 
                damage[knight]++;
        }
    }

    for (auto &[dx,dy] : complete) {
        for (int i = 1; i < 9; i++) {
            int sx = ix::x[next] + dx * i, sy = ix::y[next] + dy * i;
            int pos = sx + sy * 8;

            if (board.is_inside(sx,sy) && bit::chk(bitboard, pos) == true) {
                if (i < 2 && bit::chk(board[black][king], pos)) damage[king]++;
                if (bit::chk(board[black][queen], pos)) damage[queen]++;

                if (dx == 0 || dy == 0) { // ie : cross
                    if (bit::chk(board[black][rook], pos)) damage[rook]++;
                } else {                  // ie : diagonal
                    if (bit::chk(board[black][bishop], pos)) damage[bishop]++;
                }

                break;
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        if (damage[i]) return true;
        // cout << damage[i] << " ";
    }

    return false;
}

node evaluate (Board &board) {

    if (board[black][king] == 0) {
        cout << "checkmate\n";
        return {-1,-1,0,0};
    }

    u64 bitboard = 0;
    vector<node> vs = board.get_moves();
    int damage[6] = {0,0,0,0,0,0};

    for (int i = 0; i < 6; i++) {
        bitboard |= board[white][i];
        bitboard |= board[black][i];
    }

    for (auto &nod : vs) {
        auto &[type, alt, curr, next] = nod;

        threat(board, bitboard, next);
        cout << Display::identify(type) ;
        cout << "[" << ix::x[curr] << " " << ix::y[curr]  << "]";
        cout << "[" << ix::x[next] << " " << ix::y[next]  << "]";

        // cout << dmg << " ";
        cout << alt << "\n";
    }

    // Display::limited(board);
    // return rnd_walk(vs);
    return {};
}

int main () {

    Board board;

    string txt = "Kc8";
    auto [piece, place] = notation("Kc8");


    board[black][piece] |= 1UL << place;
    board.place("Ke8");
    board.place("Rh7");


    evaluate(board);

    //     if (is_inside(nx,ny)) {
    //            int opp = score(player_id(black, next)); // score of black piece taken, if any
    //            int heur = heuristic[type][next] - heuristic[type][curr];// + board.count();
    //            int dist = 10 - distance(nx,ny, kg % 8, kg / 8); // distance from white king
    //
    //            hist.push_back( {type, curr, next});
    //            if (opp >= 0) break;
    //     }



}
