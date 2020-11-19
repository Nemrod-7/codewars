#include <stdio.h>
#include <stdlib.h>

#define IS_UPPER(x) (((x) >= 'A') && ((x) <= 'Z'))
#define IS_DIGIT(x) (((x) >= '0') && ((x) <= '9'))
#define SUIT_END 5
#define ACE 15

#define CONVERT(x) ((x) - '0')
#define SQUARE(x) ((x) * (x))
#define TRIPLE(x) ((x) * (x) * (x))

typedef enum {Win, Loss, Tie} Result;
typedef const char * Input;
typedef unsigned short int index;
typedef struct {
    int suit[4];  // C H S D
    int card[15]; // 0 1 2 3 4 5 6 7 8 9 T J Q K A
    int rank;
    int pts;
  } Hand;


Hand* PokerHand (Input cards);
Result compare (Hand* player, Hand* opponent);
Hand *scan (Input src);
void ranking (Hand *src);
void init (Hand *src);
void format (Hand *src);

Hand* PokerHand (Input cards) {

  Hand *now = scan (cards);
  format (now);
  ranking (now);

  return now;
}

Result compare (Hand* player, Hand* opponent) {

    int play = player->rank;
    int opp = opponent->rank;

    if (play == 10 && opp == 10) {
      index  i = ACE;
      while (i--> 0) {
        int pcard = player->card[i], ocard = opponent->card[i];
        if (pcard != ocard)
          return (pcard > ocard) ? Win : Loss;
      }
    }

    if (play == opp ) {
      if (player->pts > opponent->pts)
        return Win;
      else if (player->pts == opponent->pts)
        return Tie;
      else
        return Loss;
      }
    else if (play < opp)
      return Win;
    else
      return Loss;
}

Hand *scan (Input src) {

  Hand *item = malloc (sizeof (Hand));
  init (item);
  do {
    if (IS_DIGIT (*src))
      item->card[CONVERT (*src)]++;
    if (IS_UPPER (*src)) {
      switch (*src) {
        case 'T': item->card[10]++; break;
        case 'J': item->card[11]++; break;
        case 'Q': item->card[12]++; break;
        case 'K': item->card[13]++; break;
        case 'A': item->card[14]++; break;
        case 'C': item->suit[00]++; break;
        case 'H': item->suit[01]++; break;
        case 'S': item->suit[02]++; break;
        case 'D': item->suit[03]++; break;
      }
    }
  }
  while (*src++);
  return item;
}

void ranking (Hand* src) {

  Hand *actual = src;
  index shft = 0;

  switch (actual->rank) {
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
  actual->rank = shft;
}

void format (Hand *src) {

  index i, now, prev, cnt;
  i = 15, cnt = 1;
  while (i--> 0) {
    now = src->card[i], prev = src->card[i - 1];
    if (now != 0) {
      if (now == prev) { // look for straight
        ++cnt;
        if (cnt == 5) {
          src->rank += 3;
        }
      }
    }
  }

  // look for low ace straight
  if (cnt == 4 && src->card[2] != 0 && src->card[14] != 0) {
    src->card[1] = src->card[14];
    src->card[14] = 0;
    src->rank += 3;
  }
  // look for Flush
  now = SUIT_END;
  while (now--> 0)
    src->rank += (src->suit[now] == 5) ? 7 : 0;

    i = 15;
    while (i--> 0) {
      now = src->card[i];
      src->rank += SQUARE (now);
      src->pts += TRIPLE (now) * i;
    }

}

void init (Hand* src) {
  index i;
  for (i = 15; i--> 0;) src->card[i] = 0;
  for (i = 5; i--> 0;) src->suit[i] = 0;
  src->pts = 0;
  src->rank = 0;
}
