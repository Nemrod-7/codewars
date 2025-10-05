#include "chess.hpp"
#include <random>
#include <algorithm>

using namespace std;
struct vertex { int type, now, nxt, alt; };

random_device rd;  //Will be used to obtain a seed for the random number engine
mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

std::pair<int,int> notation (const std::string &txt) { // chess notation to (type, index) => ex : kb6 -> (5,38)
    const char id = txt[0];
    const int x = txt[1] - 97, y = 7 - (txt[2] - 49);
    // printf("%c %i %i\n", id, x, y);
    switch (id) {
      case 'P' : return {pawn, move::index(x,y)}; break;
      case 'R' : return {rook, move::index(x,y)}; break;
      case 'K' : return {king, move::index(x,y)}; break;
      case 'Q' : return {queen, move::index(x,y)}; break;
      case 'B' : return {bishop, move::index(x,y)}; break;
      case 'N' : return {knight, move::index(x,y)}; break;
    }

    return {-1, 64};
}
string format(int piece, int curr) {
    std::string place = "   ";
    int x = move::x[curr] , y = move::y[curr];

    switch (piece) {
        case pawn : place[0] = 'P'; break;
        case rook : place[0] = 'R'; break;
        case king : place[0] = 'K'; break;
        case queen : place[0] = 'Q'; break;
        case bishop : place[0] = 'B'; break;
        case knight : place[0] = 'N'; break;
    }

    place[1] = x + 97;
    place[2] = y + 49;
    return place;
}

int score (int type) {

    switch (type) {
        case pawn   : return 1 ; break;
        case bishop : return 3; break;
        case knight : return 3;break;
        case rook   : return 5; break;
        case queen  : return 8; break;
        case king   : return 99; break;
    }

    return 0;
}

std::vector<u64> &Board::operator [] (int color) {
    return bitboard[color];
}
const int Board::player_id(int color, int pos) const {
  for (int j = 0; j < 6; j++) {
      if (bit::chk(bitboard[color][j], pos)) return j;
  }
  return -1;
}
const bool Board::is_inside (int x, int y) {
    return x >= 0 && y >= 0 && x < 8 && y < 8;
}
const int Board::count () {

    int total = 0;

    for (int i = 0; i < 6; i++) {
        // cout << bit::cnt(bitboard[white][i]) << " " << bit::cnt(bitboard[black][i]) << "\n";
        total += ( bit::cnt(bitboard[white][i]) - bit::cnt(bitboard[black][i])) * score(i);
    }

    return total;
}

void Board::create () {

    bitboard = {{
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
void Board::place (const std::string &txt) {
    int color = white;
    auto [piece, curr] = notation(txt);
    bitboard[color][piece] |= 1UL << curr;
}

vertex rnd_walk (vector<vertex> &vs) {
    sort(vs.begin(), vs.end(), [](vertex a, vertex b) { return a.alt > b.alt; });
    std::geometric_distribution dist;

    return vs[dist(gen) % vs.size()];
}

pair<int,int> threat (Board &board, u64 bitboard, int next) {

    int minv = 999, maxv = -1;
    int damage[6] = {0,0,0,0,0,0};

    for (auto &[dx,dy] : move::knight_m) {
        int sx = move::x[next] + dx , sy = move::y[next] + dy;

        if (board.is_inside(sx,sy) && bit::chk(bitboard, sx + sy * 8)) {
            if (bit::chk(board[black][knight], sx + sy * 8)) {
                damage[knight]++;
            }
        }
    }

    for (auto &[dx,dy] : move::complete) {
        for (int i = 1; i < 9; i++) {
            int sx = move::x[next] + dx * i, sy = move::y[next] + dy * i;
            int pos = sx + sy * 8;

            if (board.is_inside(sx,sy) && bit::chk(bitboard, pos)) {
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
        if (damage[i]) {
          minv = min(minv, i), maxv = max(maxv, i);
        }
    }

    return { score(minv),  score(maxv)};
}
vector<vertex> get_moves (Board &board) {
    u64 player = 0;
    u64 enemy = 0;
    vector<vertex> hist;

    for (int i = 0; i < 6; i++) {
        enemy |= board[black][i];
        player |= board[white][i];
    }

    for (int curr = 0; curr < 64; curr++) {
        if (bit::chk(player, curr)) {
            int x = move::x[curr], y = move::y[curr];
            int piece = board.player_id(white, curr);
            const auto &[dist, direction] = move::identify(piece);
            // if (y == 6) direction = {{0,1},{0,2}};

            for (auto &[dx,dy] : direction) {
                for (int i = 1; i <= dist; i++) {
                    int nx = move::x[curr] + dx * i, ny = move::y[curr] + dy * i;

                    if (board.is_inside(nx, ny)) {
                        int next = move::index(nx, ny);

                        if (bit::chk(player, next)) break;

                        if (bit::chk(enemy, next)) {
                            hist.push_back( {piece, curr, next});
                            break;
                        } else {
                            hist.push_back( {piece, curr, next});
                        }
                    }
                }
            }
        }
    }

    return hist;
}

int evaluate (Board &board, int depth) {

    if (depth == 0) {
        return board.count();
    }

    int maxv = 0;
    const vector<vertex> vs = get_moves(board);

    for (auto &[piece, curr, next, heur] : vs) {
        const int enemy = board.player_id(black, next);
        if (enemy != -1) board[black][enemy] ^= 1UL << next;
        board[white][piece] ^= 1UL << curr;
        board[white][piece] ^= 1UL << next;

        maxv += evaluate(board, depth - 1);

        board[white][piece] ^= 1UL << curr;
        board[white][piece] ^= 1UL << next;
        if (enemy != -1) board[black][enemy] ^= 1UL << next;
    }

    return maxv / vs.size();
}

vertex select2 (Board &board) {

    if (board[black][king] == 0) {
        cout << "checkmate\n";
        return {-1,0,0};
    }

    u64 bitboard = 0;
    vector<vertex> vs = get_moves(board), valid;

    for (int i = 0; i < 6; i++) {
        bitboard |= (board[white][i] | board[black][i]);
    }

    for (auto &node : vs) {
        const auto &[piece, curr, next, heur] = node;
        const auto &[minv, maxv] = threat(board, bitboard, next);
        int enemy = board.player_id(black, next);
        // int heur = heuristic[piece][next] - heuristic[piece][curr];
        board[white][piece] ^= 1UL << curr;
        board[white][piece] ^= 1UL << next;

        if (enemy != -1) {
            board[black][enemy] ^= 1UL << next;
        }
        int res = evaluate(board, 2);

        cout << res << " ";
        board[white][piece] ^= 1UL << curr;
        board[white][piece] ^= 1UL << next;

        if (enemy != -1) {
            board[black][enemy] ^= 1UL << next;
        }
        // cout << Display::identify(piece) ;
        // cout << "[" << move::x[curr] << " " << move::y[curr]  << "]";
        // cout << "[" << move::x[next] << " " << move::y[next]  << "]";
        // cout << heur << "\n";

        if (bit::chk(board[black][king], next)) return node;
        if (minv || maxv) continue;
        valid.push_back(node);
    }

    // Display::limited(board);
    return rnd_walk(vs);
}
vertex select1 (Board &board) {

    if (board[black][king] == 0) {
        cout << "checkmate\n";
        return {-1,0,0};
    }

    u64 bitboard = 0;
    vector<vertex> vs = get_moves(board), valid;

    for (int i = 0; i < 6; i++) {
        bitboard |= (board[white][i] | board[black][i]);
    }

    for (auto &node : vs) {
        auto &[piece, curr, next, heur] = node;
        const auto &[minv, maxv] = threat(board, bitboard, next);

        heur = heuristic[piece][next] - heuristic[piece][curr];

        // cout << Display::identify(piece) ;
        // cout << "[" << move::x[curr] << " " << move::y[curr]  << "]";
        // cout << "[" << move::x[next] << " " << move::y[next]  << "]";
        // cout << heur << "\n";

        if (bit::chk(board[black][king], next)) return node;
        if (minv || maxv) continue;
        valid.push_back(node);
    }


    return rnd_walk(valid);
}

string play (Board &board, const string &txt) {

    const auto [opp, post] = notation(txt);
    bool valid_move = false;

    for (auto &curr : move::get( move::identify(opp), post) ) {
        if (bit::chk(board[black][opp], curr)) {
            board[black][opp] ^= 1UL << curr;
            board[black][opp] ^= 1UL << post;
            valid_move = true;
            break;
        }
    }

    if (valid_move) {
        auto [piece, curr, next, heur] = select1(board);

        board[white][piece] ^= 1UL << curr;
        board[white][piece] ^= 1UL << next;

        return format(piece, next);
    } else {
        return "invalid move.";
    }

    return "";
}



int main () {

    Board board;
    auto [piece1, idx] = notation("Kc8");
    board[black][piece1] |= 1UL << idx;

    board.place("Ke8");
    board.place("Rh7");

    play(board, "Kb8");
    // Display::limited(board);
    // board[white][piece] ^= 1UL << curr;
    // board[white][piece] ^= 1UL << next;
    // string txt;
    // cout << "play your next move : \n";
    // cin >> txt;
    // // play(board, txt);
    //
    Display::limited(board);



    std::cout << "\nexit\n";


}
