#define ABSOLUTE(x) ((x) < 0 ? (-x) : (x))

const char board[5][8]=
{
{'a','b','c','d','e','1','2','3'},
{'f','g','h','i','j','4','5','6'},
{'k','l','m','n','o','7','8','9'},
{'p','q','r','s','t','.','@','0'},
{'u','v','w','x','y','z','_','/'},
};

typedef unsigned short int Index;
typedef const char *Input;

typedef struct {
  int x;
  int y;
} Point;

int tv_remote (Input source) {

  Index Length (Input source);
  Point position (char search);

  Index total = Length(source);
  Index now = 0, cnt = 0;
  Point letter[total];

  for (; now < total; ++now) {
    letter[now] = position (source[now]);
    int last = now - 1;
      if (now == 0) {
        cnt = letter[now].x + letter[now].y + 1;
      }
      if (now > 0) {
        cnt += ABSOLUTE((letter[now].x - letter[last].x)) + ABSOLUTE((letter[now].y - letter[last].y)) + 1;
      }
  }
  return cnt;
}

Point position (char search) {

  Index i, j;
  Point now;

  for (i = 5; i-->0;) {
    for (j = 8; j-->0;) {
      if (board[i][j] == search) {
        now.x = i;
        now.y = j;
      }
    }
  }
  return now;
}

Index Length (Input source) {
  Index cnt = 0;
  while(*source++) cnt++;
  return cnt;
}
