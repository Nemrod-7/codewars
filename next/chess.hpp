#include <iostream>
#include <vector>

///////////////////////////////////header/////////////////////////////////////
using u64 = unsigned long int;

enum {black, white};
enum {pawn, bishop, knight, rook, queen, king};

namespace move {
    const int x[64] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};
    const int y[64] = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7};

    const std::vector<std::pair<int,int>> compass = {{0,-1},{1,0},{0,1},{-1,0}};
    const std::vector<std::pair<int,int>> diagonal = {{-1,-1},{1,-1},{1,1},{-1,1}};
    const std::vector<std::pair<int,int>> complete = {{0,-1},{1,0},{0,1},{-1,0},{-1,-1},{1,-1},{1,1},{-1,1}};
    const std::vector<std::pair<int,int>> knight_m = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };

    const int index(int sx, int sy) { return sx + sy * 8; }

    const std::pair<int, std::vector<std::pair<int,int>>> identify (int piece) {

        switch (piece) {
            case pawn : break;
            case rook : return { 8, move::compass } ;
            case bishop : return { 8, move::diagonal } ;
            case knight : return { 1, move::knight_m } ;
            case queen : return { 8, move::complete } ;
            case king : return { 1, move::complete } ;
        }

        return {-1,{}};
    }
    const std::vector<int> get (int dist, const std::vector<std::pair<int,int>> &direction, int cell) {
        std::vector<int> place;

        for (auto &[dx,dy] : direction) {
            for (int i = 1; i <= dist; i++) {
                int sx = x[cell] + dx * i, sy = y[cell] + dy * i;

                if (sx >= 0 && sy >= 0 && sx < 8 && sy < 8) {
                    place.push_back(sx + sy * 8);
                }
            }
        }

        return place;
    }
    const std::vector<int> get (const std::pair<int, std::vector<std::pair<int,int>>> &mm, int cell) {
        const auto &[dist, direction] = mm;
        std::vector<int> place;

        for (auto &[dx,dy] : direction) {
            for (int i = 1; i <= dist; i++) {
                int sx = x[cell] + dx * i, sy = y[cell] + dy * i;

                if (sx >= 0 && sy >= 0 && sx < 8 && sy < 8) {
                    place.push_back(sx + sy * 8);
                }
            }
        }

        return place;
    }

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
    std::vector<int> pos(u64 num) {
        int ix = 0;
        std::vector<int> vs;

        do {
            if (num & 1) {
                vs.push_back(ix);
            }

            ix++;
        } while (num >>= 1);

        return vs;
    }
};

class Board {
    private :
        // u64 grid[2][6] = {{0,0,0,0,0,0},{0,0,0,0,0,0}};
        std::vector<std::vector<u64>> bitboard = {{0,0,0,0,0,0},{0,0,0,0,0,0}};
    public :
        std::vector<u64> & operator [] (int);

        const int player_id(int, int) const;
        void create();
        void place (const std::string &) ;
        const bool is_inside(int, int);
        const int count ();
};
class Display {
    private :
        // inline static std::map<int, std::string> utblack = { {pawn, "♙"}, {rook, "♖"}, {bishop, "♗"}, {knight, "♘"}, {queen, "♕"}, {king, "♔"} };
        // inline static std::map<int, std::string> utwhite = { {pawn, "♟"}, {rook, "♜"}, {bishop, "♝"}, {knight, "♞"}, {queen, "♛"}, {king, "♚"} };
        inline static const std::vector<std::string> utblack = {"♙","♗","♘","♖","♕","♔"};
        inline static const std::vector<std::string> utwhite = {"♟","♝","♞","♜","♛","♚"};

    public :
        static void board (const Board &board) {
            std::cout << "\n";

            for (unsigned long i = 0; i < 64; i++) {
                const int x = move::x[i], y = move::y[i];
                int pla = board.player_id(white, i), opp = board.player_id(black, i);

                if (pla >= 0) {
                    std::cout << utwhite[pla];
                } else if (opp >= 0) {
                    std::cout << utblack[opp];
                } else {
                    std::cout << ".";
                }

                std::cout << " ";
                if (x == 7) std::cout << "\n";
            }
        }

        static void limited (const Board &board) {
            std::cout << "\n";
            char disblack[6] = {'p','b','n','r','q','k'};
            char diswhite[6] = {'P','B','N','R','Q','K'};

            std::vector<std::vector<u64>> bitboard = {{0,0,0,0,0,0},{0,0,0,0,0,0}};
            std::vector<std::string> grid (8, std::string(8, '.'));

            for (unsigned long i = 0; i < 64; i++) {
                const int x = move::x[i], y = move::y[i];
                int pla = board.player_id(white, i), opp = board.player_id(black, i);

                if (pla >= 0) {
                    std::cout << diswhite[pla];
                } else if (opp >= 0) {
                    std::cout << disblack[opp];
                } else {
                    std::cout << ".";
                }

                std::cout << " ";
                if (x == 7) std::cout << "\n";
            }

        }

        static std::string identify (int type) {

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

const std::vector<std::vector<int>> heuristic {
    { // pawn
        0,  0,  0,  0,  0,  0,  0,  0,
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
            5,  5, 10, 25, 25, 10,  5,  5,
            0,  0,  0, 20, 20,  0,  0,  0,
            5, -5,-10,  0,  0,-10, -5,  5,
            5, 10, 10,-20,-20, 10, 10,  5,
            0,  0,  0,  0,  0,  0,  0,  0
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
    }, { // rook
        0,  0,  0,  0,  0,  0,  0,  0,
            5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            0,  0,  0,  5,  5,  0,  0,  0
    }, { // queen
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
