#include <iostream>
#include <vector>

enum class Result { Win, Loss, Tie };

struct PokerHand  {
  std::string hand;
  int suit[4], card[15], rank, pts; // C H S D // 0 1 2 3 4 5 6 7 8 9 T J Q K A

  void mkcount (), ranking ();

  PokerHand (const char *src) {
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
              default :
                  card[c - '0']++; break;
          }
      }

      mkcount ();
      ranking ();
  }
};

void PokerHand::ranking () {

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
void PokerHand::mkcount () {

  int i, now, prev, cnt;
  i = 15, cnt = 1;
  while (i--> 0) {
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
    card[1] = card[14];
    card[14] = 0;
    rank += 3;
  }
  // look for Flush
  now = 5;
  while (now--> 0)
    rank += (suit[now] == 5) ? 7 : 0;

    i = 15;
    while (i--> 0) {
      now = card[i];
      rank += (now * now);
      pts += (now * now * now) * i;
    }

}

Result compare (const PokerHand player, const PokerHand opponent) {

    int play = player.rank, opp = opponent.rank;

    if (play == 10 && opp == 10) {
        int  i = 15;
        while (i-->0) {
            int pcard = player.card[i], ocard = opponent.card[i];
            if (pcard != ocard)
                return (pcard > ocard) ? Result::Win : Result::Loss;
        }
    }

    if (play == opp ) {
        if (player.pts > opponent.pts)
            return Result::Win;
        else if (player.pts == opponent.pts)
            return Result::Tie;
        else
            return Result::Loss;
    } else if (play < opp)
        return Result::Win;

    return Result::Loss;
}

int main () {



}
