#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

enum class Result { Win, Loss, Tie };

struct PokerHand  {

    int suit[4], card[15], rank, pts; // C H S D // 0 1 2 3 4 5 6 7 8 9 T J Q K A

    PokerHand (const string &hand ) {
        rank = 0, pts = 0;

        std::fill (&suit[0], &suit[4], 0);
        std::fill (&card[0], &card[15], 0);

        for (auto &c : hand) {
            switch (c) {
                case ' ': break;
                case 'T': card[10]++; break;
                case 'J': card[11]++; break;
                case 'Q': card[12]++; break;
                case 'K': card[13]++; break;
                case 'A': card[14]++; break;
                case 'C': suit[00]++; break;
                case 'H': suit[01]++; break;
                case 'S': suit[02]++; break;
                case 'D': suit[03]++; break;
                default : card[c - '0']++; break;
            }
        }

        mkcount ();
        ranking ();
        //cout << hand << " :: ";
    }
    void ranking () {

        int shft = 0;

        switch (rank) {
            case 5  : shft = 10; break; // High cards
            case 7  : shft = 9; break;  // One pair
            case 9  : shft = 8; break;  // Two pair
            case 11 : shft = 7; break;  // Three of a kind
            case 8  : shft = 6; break;  // Straight
            case 12 : shft = 5; break;  // Flush
            case 13 : shft = 4; break;  // Full house
            case 17 : shft = 3; break;  // Four of a kind
            case 15 : shft = 2; break;  // Straight Flush
            case 25 : shft = 1; break;  // Five of a kind
        }

        rank = shft;
    }
    void mkcount () {

        int  now, prev, cnt = 1;

        for (int i = 2; i < 15; i++) {
            now = card[i], prev = card[i - 1];
            if (now != 0) {
                if (now == prev) { // look for straight
                    ++cnt;
                    if (cnt == 5) {
                        rank += 3;
                    }
                }
            }
        }

        // look for low ace straight
        if (cnt == 4 && card[2] != 0 && card[14] != 0) {
            swap (card[1], card[14]);
            rank += 3;
        }
        // look for Flush
        for (int i = 0; i < 5; i++) {
            rank += (suit[i] == 5) ? 7 : 0;
        }

        for (int i = 1; i < 15; i++) {
            rank += (card[i] * card[i]);
            pts += pow (card[i], 5) * i;
        }
    }
};

Result compare (const PokerHand &player, const PokerHand &opponent) {

    int play = player.rank, opp = opponent.rank;

    if (play == 10 && opp == 10) {
        int  i = 15;
        while (i-->0) {
            int pcard = player.card[i], ocard = opponent.card[i];
            if (pcard != ocard)
                return (pcard > ocard) ? Result::Win : Result::Loss;
        }
    }

    if (play == opp) {
        if (player.pts > opponent.pts) return Result::Win;
        else if (player.pts == opponent.pts) return Result::Tie;
        else return Result::Loss;
    } else if (play < opp)
        return Result::Win;

    return Result::Loss;
}

bool run_test_hands (const PokerHand &player, const PokerHand &opponent, Result outcome) {
    return outcome != compare (player, opponent);
}
void run_test (const string &player, const string &opponent, Result outcome) {
    PokerHand pl1(player), pl2 (opponent);
    if (run_test_hands (pl1, pl2, outcome)) {
        cout << player << " " << pl1.pts << " :: " << opponent << " " << pl2.pts << " error\n";
    }
}
void test () {

    run_test ("2H 3H 4H 5H 6H", "KS AS TS QS JS", Result::Loss); // "Highest straight flush wins"
    run_test ("2H 3H 4H 5H 6H", "AS AD AC AH JD", Result::Win ); // "Straight flush wins of 4 of a kind"
    run_test ("AS AH 2H AD AC", "JS JD JC JH 3D", Result::Win ); // "Highest 4 of a kind wins"
    run_test ("2S AH 2H AS AC", "JS JD JC JH AD", Result::Loss); // "4 Of a kind wins of full house"
    run_test ("2S AH 2H AS AC", "2H 3H 5H 6H 7H", Result::Win ); // "Full house wins of flush"
    run_test ("AS 3S 4S 8S 2S", "2H 3H 5H 6H 7H", Result::Win ); // "Highest flush wins"
    run_test ("2H 3H 5H 6H 7H", "2S 3H 4H 5S 6C", Result::Win ); // "Flush wins of straight"
    run_test ("2S 3H 4H 5S 6C", "3D 4C 5H 6H 2S", Result::Tie ); // "Equal straight is tie"
    run_test ("2S 3H 4H 5S 6C", "AH AC 5H 6H AS", Result::Win ); // "Straight wins of three of a kind"
    run_test ("2S 3H 4H 5S AC", "AH AC 5H 6H AS", Result::Win ); // "Low-ace straight wins of three of a kind"
    run_test ("2S 2H 4H 5S 4C", "AH AC 5H 6H AS", Result::Loss); // "3 Of a kind wins of two pair"
    run_test ("2S 2H 4H 5S 4C", "AH AC 5H 6H 7S", Result::Win ); // "2 Pair wins of pair"
    run_test ("6S AD 7H 4S AS", "AH AC 5H 6H 7S", Result::Loss); // "Highest pair wins"
    run_test ("2S AH 4H 5S KC", "AH AC 5H 6H 7S", Result::Loss); // "Pair wins of nothing"
    run_test ("2S 3H 6H 7S 9C", "7H 3C TH 6H 9S", Result::Loss); // "Highest card loses"
    run_test ("4S 5H 6H TS AC", "3S 5H 6H TS AC", Result::Win ); // "Highest card wins"
    run_test ("2S AH 4H 5S 6C", "AD 4C 5H 6H 2C", Result::Tie ); // "Equal cards is tie"
}



int main () {


    /*
       std::string line;
       std::fstream file ("p054_poker.txt");

       while (getline (file, line)) {
       std::string player1 = line.substr (0, 14);
       std::string player2 = line.substr (15, line.size() - 1);
    //std::cout << player1 << " :: " << player2 << "\n" ;
    }

    file.close();

*/

/*

  test();

    vector<pair<string,string>> test =
    {{"5H 5C 6S 7S KD", "2C 3S 8S 8D TD"},
        {"5D 8C 9S JS AC", "2C 5C 7D 8S QH"},
        {"2D 9C AS AH AC", "3D 6D 7D TD QD"},
        {"4D 6S 9H QH QC", "3D 6D 7H QD QS"},
        {"2H 2D 4C 4D 4S", "3C 3D 3S 9S 9D"}};

    for (auto [player1, player2] : test) {
        //cout << player1 << " :: " << player2 << " => ";
        Result res = compare (PokerHand(player1), PokerHand(player2));

        switch (res) {
            case Result::Win : cout << "winner : Player 1\n"; break;
            case Result::Loss : cout << "winner : Player 2\n"; break;
            case Result::Tie : cout << "no winner\n"; break;
        }
    }

    */


}
