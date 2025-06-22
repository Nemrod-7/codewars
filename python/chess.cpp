#include <iostream>
#include <vector>
#include <map>

using namespace std;

using point = pair<int,int>;
using node = pair<char,char>;

const vector<point> DIAGONAL = { {-1,-1},{1,-1},{1,1},{-1,1} }, COMPASS = { {0,-1},{1,0},{0,1},{-1,0} };
const vector<point> COMPLETE { {-1,-1},{1,-1},{1,1},{-1,1}, {0,-1},{1,0},{0,1},{-1,0} };

const bool is_inside (int x, int y) { return x >= 0 and y >= 0 and x < 8 and y < 8; }
const int distance (int x1, int y1, int x2, int y2) { return abs(x1-x2) - abs(y1-y2); }
// const bool is_free(int x1, int y1, int x2, int y2)

int score (char type) {
    
    switch (type) {
        case 'P' : return 1; break;
        case 'N' : return 3; break;
        case 'B' : return 3; break;
        case 'R' : return 5; break;
        case 'Q' : return 9; break;
        case 'K' : return 100; break;
    }
    return 0;
}

pair<int, vector<point>> get_moves (char type) {

    switch (type) {
        case 'N': return {1, {}}; break;
        case 'R' : return {8, COMPASS}; break;
        case 'B' : return {8, DIAGONAL}; break;
        case 'Q' : return {8, COMPLETE}; break;
        case 'K' : return {1, COMPLETE}; break;
    }

    return {};
}
void opponent (vector<vector<node>> &board) {

    vector<pair<int, pair<point, point>>> hist; // <score, point, point> 

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            auto [type, color] = board[y][x];
            auto [move, direction] = get_moves(type);

            if (type == 'P') {
                if (color == 'B') {
                    if (is_inside(x , y+1) ) direction.push_back({0,1});
                    // if (is_inside(x-1 , y+1) and board[y+1][x-1]) direction.push_back({-1,1});
                    // if (is_inside(x+1 , y+1) and board[y+1][x+1]) direction.push_back({1,1});
                } else {
                    if (is_inside(x , y-1) ) direction.push_back({0, -1});
                    // if (is_inside(x-1 , y-1) and board[y-1][x-1]) poss.push_back(next);
                    // if (is_inside(x+1 , y-1) and board[y-1][x+1]) poss.push_back(next);
                }
            }

            for (auto &[dx, dy] : direction) {
                for (int i = 1; i < move; i++) {
                    int nx = x + dx * i, ny = y + dy * i;

                    if (is_inside(nx,ny)) {
                        int score = board[ny][nx].first;
                        pair<int, pair<point, point>> nextv = {score, {{x,y}, {nx,ny}}};

                        if (board[ny][nx].first == ' ') {
                            hist.push_back(nextv);
                        } else {
                            if (board[ny][nx].second != color) hist.push_back(nextv);
                            break;
                        }
                    }
                }
            }
        }
    }




}

vector<vector<node>> mk_board() {
    vector<vector<node>> board(8, vector<node> (8));

    for (int i = 0; i < 8; i++) {
        board[1][i] = {'P','B'};
        board[6][i] = {'P','W'};
    }

    board[0][0] = board[0][7] = {'R','B'};
    board[7][0] = board[7][7] = {'R','W'};
    board[0][1] = board[0][6] = {'B','B'};
    board[7][1] = board[7][6] = {'B','W'};
    board[0][2] = board[0][5] = {'N','B'};
    board[7][2] = board[7][5] = {'N','W'};
    board[0][3] = {'Q','B'};
    board[7][3] = {'Q','W'};
    board[0][4] = {'K','B'}; 
    board[7][4] = {'K','W'};

    return board;
}

void display (vector<vector<node>> &board) {

    map<char,string> white = { {'P',"♟"}, {'R',"♜"}, {'B',"♝"}, {'N',"♞"}, {'Q',"♛"}, {'K',"♚"} };
    map<char,string> black = { {'P',"♙"}, {'R',"♖"}, {'B',"♗"}, {'N',"♘"}, {'Q',"♕"}, {'K',"♔"} };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto &[type, color] = board[i][j];

            if (type == 0) {
                if ((i + j) % 2) {
                    cout << ". ";
                } else {
                    cout << "  ";
                }
            } else if (color == 'B') {
                cout << black[type] << ' ';
            } else if (color == 'W') {
                cout << white[type] << ' ';
            }
        }

        cout << "\n";
    }
}

int main () {

    vector<vector<node>> board = mk_board();





}
