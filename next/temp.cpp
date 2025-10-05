#include <iostream>

enum {pawn, bishop, knight, rook, queen, king};


int evaluate() {
    // int score = materialWeight * (numWhitePieces - numBlackPieces) * who2move 
    
    return 1;
}

int minimax(std::vector<int> board, int depth, int alpha, int beta, int maxim) {

    if (depth == 0) {
        return evaluate();
    }

    if (maxim) {
        int maxv = -99999;

        for (int i = 0; i < 5; i++) {
            maxv = max(maxv, minimax(board, depth - 1, alpha, beta, false));

            alpha = max(alpha, maxv);
            if (beta <= alpha) break;
        }

        return maxv;
    } else {
        int minv = 99999; 

        for (int i = 0; i < 5; i++) {
            minv = max(minv, minimax(board, depth - 1, alpha, beta, true));

            alpha = max(alpha, minv);
            if (beta <= alpha) break;
        }

        return minv;
    }
}

// def find_best_move(board, depth):
//     best_move = None
//     max_eval = float('-inf')
//     alpha = float('-inf')
//     beta = float('inf')
//
//     for move in board.legal_moves:
//         board.push(move)
//         eval = minimax(board, depth - 1, -9999, 99999, false)
//         board.pop()
//
//         if eval > max_eval:
//             max_eval = eval
//             best_move = move
//
//     return best_move
//


int negaMax( int depth ) {

    if ( depth == 0 ) return evaluate();
    
    int maxv = -999;
    
    for (int i = 0; i < 5; i++) {
        int score = -negaMax( depth - 1 );
        if( score > maxv )
            maxv = score;
    }

    return maxv;
}

int main() {

    // int dfs(4);

}
