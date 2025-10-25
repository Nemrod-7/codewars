#include <iostream>
#include <vector>

#include "chess.hpp"

using namespace std;

class Display1 {

    private :
        inline static const std::vector<std::string> utblack = {"♙","♗","♘","♖","♕","♔"};
        inline static const std::vector<std::string> utwhite = {"♟","♝","♞","♜","♛","♚"};

        static string header() {
            string txt = "┌";

            for (int i = 0; i < 7 ; i++ ) {
                for (int j = 0; j < 7; j++) {
                    txt +=  "─";
                }
                txt +=  "┬";
            }
            for (int j = 0; j < 7; j++) {
                txt +=  "─";
            }

            return txt + "┐";
        }
        static string footer() {
            string txt =  "└";

            for (int i = 0; i < 7 ; i++ ) {
                for (int j = 0; j < 7; j++) {
                    txt +=  "─";
                }
                txt +=  "┴";
            }
            for (int j = 0; j < 7; j++) {
                txt +=  "─";
            }

            return txt +  "┘";
        }
        static string middle() {
            string txt =  "├";

            for (int i = 0; i < 7 ; i++ ) {
                for (int j = 0; j < 7; j++) {
                    txt +=  "─";
                }
                txt +=  "┼";
            }
            for (int j = 0; j < 7; j++) {
                txt +=  "─";
            }

            return txt +  "┤";
        }
        static string head() {
            string txt;
            for (int i = 0; i < 8; i++) {
                txt += "    ";
                txt += char(i) + 'A';
                txt += "   ";
            }

            return txt;
        }
    public :
        static void large(Board &board) {
            cout << "   " << head() << "\n";
            cout << "   " << header() << "\n";

            char disblack[6] = {'p','b','n','r','q','k'};
            char diswhite[6] = {'P','B','N','R','Q','K'};

            // cell
            for (int y = 0; y < 8; y++) {
                for (int j = 0; j < 3; j++) {

                    cout << " ";
                    if (j == 1) {
                        cout << 8 - y;
                    } else {
                        cout << " ";
                    }

                    cout << " ";
                    cout << "│"; 
                    for (int x = 0; x < 8; x++) {
                        bool pat = (x + y) % 2;
                        int ix = move::index(x,y);
                        string pattern = pat ? "▒" : " ";

                        for (int l = 0; l < 7; l++) {

                            if (j == 1 && l == 3) {
                                int pla = board.player_id(white, ix), opp = board.player_id(black, ix);

                                if (pla >= 0) {
                                    std::cout << diswhite[pla];
                                } else if (opp >= 0) {
                                    std::cout << disblack[opp];
                                } else {
                                    cout << pattern;
                                }


                            } else {
                                cout << pattern;
                            }
                        }

                        cout << "│"; 
                    }

                    // cout << "│"; 

                    cout << "\n";

                }

                if (y < 7) {
                    cout << "   " << middle() << "\n";
                } else {
                    cout << "   " << footer() << "\n";
                }

            }

        }
};

int main() {

    Board board;
    board.create();


    Display::board(board);



}
