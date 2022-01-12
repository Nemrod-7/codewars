#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include <time.h>
#include <unistd.h>

#define MAX(x,y) (((x) > (y)) ? (x) : (y))

typedef struct _point { int x, y; } point;
typedef struct _train {
    char id;
    int size;
    point *trn;
} train;
typedef struct _graph {
    int width, height;
    char **grid;
} graph;

const point direct[8] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
const char base[4][3] = {{'|', '+', 'S'}, {'/','X','S'}, {'-','S','-'}, {'\\', 'X','S'}};

point add (const point a, const point b) {
    point p = {a.x + b.x, a.y + b.y};
    return p;
}
bool is_inside (graph *track, point p) { return (p.x >= 0 && p.y >= 0 && p.x < track->width && p.y < track->height); }
char tile (graph *track, const point p) { return is_inside (track, p) ? track->grid[p.y][p.x] : ' '; }
bool equals (const point a, const point b) { return a.x == b.x && a.y == b.y; }
bool is_valid (graph *track, const point prev, const point curr, int index) {

    point nxtp = add (curr, direct[index]);

    if (equals (nxtp, prev)) return false;
    char rail = tile (track, curr), last = tile (track, prev), next = tile (track, nxtp);

    index %= 4;

    switch (rail) {
        case '+' : if ((last == '-' && next == '-') || (last != '-' && index == 0)) return true; break;
        case 'X' : if (last == next) return true; break;
        case 'S' : if (last == next) return true; break;
        case '|' : if (index == 0) return true; break;
        case '-' : if (index == 2) return true; break;
        default : {
            for (int i = 0; i < 3; i++) {
                if (base[index][i] == next) return true;
            }
        }; break;
    }

    return false;
}

//////////////////////////////////display//////////////////////////////////////
void printtest (const char *src, const char *a_train, int a_train_pos, const char *b_train, int b_train_pos, int limit) {

    const int size = strlen (src);

    printf ("src = {");
    for (int i = 0; i < size; i++) {
         printf ("%i", src[i]);
        if (i < size - 1) printf (",");
    }

    printf ("}; train_crash (track, %s, %i, %s, %i, %i);", a_train, a_train_pos, b_train, b_train_pos, limit);
}
bool fndwagon (const train *a, const point p) {
  for (int i = 0; i < a->size; i++)
  if (equals (a->trn[i], p)) return true;
  return false;
}
void showpoint (const point p) { printf ("%i %i\n", p.x, p.y); }
void display2 (graph *curr, const train *a, const train *b) {
  point p;
  printf ("\033c");
  usleep(50000);

  for (p.y = 0; p.y < curr->height; p.y++) {
    for (p.x = 0; p.x < curr->width; p.x++) {
      char tile = curr->grid[p.y][p.x];

      if (equals (a->trn[0], p)) {
        printf ("%c", a->id);
      } else if (equals (b->trn[0], p)) {
        printf ("%c", b->id);
      } else {
        if (fndwagon (a, p))
        printf ("%c", tolower (a->id));
        else if (fndwagon (b, p))
        printf ("%c", tolower (b->id));
        else
        printf ("%c", tile);
      }
    }

    printf("\n");
  }
}
void display1 (graph *curr, const train *a) {
  point p;
  printf ("\033c");
  usleep(50000);

  for (p.y = 0; p.y < curr->height; p.y++) {
    for (p.x = 0; p.x < curr->width; p.x++) {
      char tile = curr->grid[p.y][p.x];

      if (equals (a->trn[0], p)) {
        printf ("%c", a->id);
      } else {
        if (fndwagon (a, p))
        printf ("%c", tolower (a->id));
        else
        printf ("%c", tile);
      }
    }

    printf("\n");
  }
}
////////////////////////////////////////////////////////////////////////////////
void move (train *x, point to) {
    int i = x->size;

    while (i-->1)
        x->trn[i] = x->trn[i - 1];

    x->trn[0] = to;
}
void getnext (graph *track, train *x) {
    point prev = x->trn[1], curr = x->trn[0];

    for (int i = 0; i < 8; i++) {
        if (is_valid (track, prev, curr, i)) {
            move (x, add (curr, direct[i]));
            break;
        }
    }
}
void advance (graph *track, train *x, int *wait) {

    if (x->id == 'X') getnext (track, x);
    else {
        if (tile (track, x->trn[0]) == 'S' && *wait < x->size - 1) {
            *wait += 1;
        } else {
            if (*wait > 0) *wait = 0;
            getnext (track, x);
        }
    }
}

point get_origin (graph *track) {
    point p;
    for (p.y = 0; p.y < track->height; ++p.y) {
        for (p.x = 0; p.x < track->width; ++p.x) {
          //printf ("%c",track.grid[p.y][p.x]);
            if (tile (track, p) != ' ') return p;
        }
    }
    return (point){0,0};
}
train *mktrain (graph *track, const char *src, int pos) {
    int size = strlen (src), j = 0;

    train X = {'X', 2, malloc (2 * sizeof (point))};  // search starting point
    X.trn[0] = get_origin (track);

    for (int i = 4; i < 8; i++) {
          point nxt = add (X.trn[0], direct[i]);

          if (is_valid (track, X.trn[0], X.trn[0], i))
              X.trn[1] = nxt;
    }

    while (pos-->0)
        getnext (track, &X);

    train *curr = malloc (sizeof (train));
    curr->size = size;
    curr->trn = malloc (size * sizeof (point));

    if (isupper (src[0])) {                   // set anticlockwise
        curr->id = src[0];
        curr->trn[j++] = X.trn[0];

        for (int i = 0; i < 8; i++) {
            if (is_valid(track, X.trn[1], X.trn[0], i)) {
                curr->trn[j++] = add (X.trn[0], direct[i]);
            }
        }
    } else {                                  // or clockwise
        curr->id = src[size - 1];
        curr->trn[j++] = X.trn[0];
        curr->trn[j++] = X.trn[1];
    }

    while (j < size) {                             // make wagons
        for (int i = 0; i < 8; i++) {
            point prev = curr->trn[j - 2];

            if (is_valid (track, prev, curr->trn[j - 1], i) && j < size) {
                curr->trn[j] = add (curr->trn[j - 1] , direct[i]);
                j++;
            }

        }
    }

    return curr;
}
graph *mk_graph (const char *src) {
    graph *new = malloc (sizeof (graph));
    new->grid = malloc (128 * sizeof (char *));

    for (int i = 0; i < 128; i++) {
        new->grid[i] = malloc (128 * sizeof (char));
        for (int j = 0; j < 128; j++) {
            new->grid[i][j] = ' ';
        }
    }
    int width = 0, size = strlen (src);
    int x = 0, y = 0;

    for (int i = 0; i < size; i++, x++) {

        if (src[i] == '\n') {
            width = MAX (x, width);
            y++, x = -1;
        } else {
            if (src[i] != ' ') {
                new->grid[y][x] = src[i];
            }
        }
    }

    new->height = y, new->width = width;

    return new;
  }

bool collision (const train *a, const train *b) {
    int i, map[2048] = {};
    point p;

    for (i = 0; i < a->size; i++) {
        p = a->trn[i];
        map[p.y * 66 +p.x]++;
    }

    for (i = 0; i < b->size; i++) {
        p = b->trn[i];
        map[p.y * 66 +p.x]++;
    }
    for (i = 0; i < 2048; i++) {
        if (map[i] > 1) return true;
    }
    return false;
}
int train_crash (const char *src, const char *a_train, int a_train_pos, const char *b_train, int b_train_pos, int limit) {
    //printtest(src, a_train, a_train_pos, b_train, b_train_pos, limit);
    graph *track = mk_graph (src);

    train *A = mktrain (track, a_train, a_train_pos);
    train *B = mktrain (track, b_train, b_train_pos);
    int sleep = A->size, sleepb = B->size, cnt = 0;

    if (collision (A, B)) return 0;

    while (limit-->0) {

      display2 (track, A, B);
        advance (track, A, &sleep);
        advance (track, B, &sleepb);
        cnt++;
        if (collision (A, B)) return cnt;
    }
    /*
    */
    return -1;
}

void Test () {

    /*

    const char track1[1024] = {47,45,45,45,45,92,32,32,32,32,32,47,45,45,45,45,92,32,10,124,32,32,32,32,32,92,32,32,32,47,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,47,32,32,32,92,32,32,32,32,32,124,32,10,92,45,45,45,45,47,32,32,32,32,32,92,45,45,45,45,47,10};
    train_crash (track1, "Eeeeeeeeeeeeeeeeeeeeeeeeeeeeeee", 7, "Xxxx", 0, 100);

    const char track2[1024] = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,32,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,10};
    train_crash (track2, "aA", 10, "oooooooooooooooooooooooooO", 70, 200);

    const char track3[1924] = {47,45,45,45,45,92,32,32,32,32,32,47,45,45,45,45,92,32,10,124,32,32,32,32,32,92,32,32,32,47,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,47,32,32,32,92,32,32,32,32,32,124,32,10,92,45,45,45,45,47,32,32,32,32,32,92,45,45,45,45,47,10};
    train_crash (track3, "aaaaaA", 8, "bbbbbB", 20, 100);

    */
const char track6[1024] = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,32,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,10,32,32,32,32,32,32,32,32,10};
//  train_crash (track6, "Xxx", 22, "Dddddd", 84, 2000);
const char track7[1024] = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,43,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
train_crash (track7, "aaaA", 0, "bbbbbbbbbbbbbB", 30, 100);
  /*
    const char track4[1024] = {47,45,45,45,45,45,45,83,45,45,45,45,45,45,45,45,45,45,92,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,10,92,45,45,45,45,45,45,45,45,45,45,83,45,45,45,45,45,45,47,10};
    train_crash (track4, "aaaaaaaaaaaaA", 7, "xxxX", 30, 100);


    const char track5[1024] = {47,45,45,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,45,45,83,45,45,45,45,45,45,45,45,92,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,124,32,32,32,32,32,32,32,32,124,10,32,32,32,32,32,32,32,32,92,45,45,45,45,45,45,45,45,47,10};
    train_crash (track5, "Xxxx", 16, "Eeeeeeeeeeeeeeeeeeeeeeee", 12, 2000);


    */

}

int main () {
  time_t start = clock(), stop;

  const char* track =
      "                                /------------\\\n"
      "/-------------\\                /             |\n"
      "|             |               /              S\n"
      "|             |              /               |\n"
      "|        /----+--------------+------\\        |   \n"
      "\\       /     |              |      |        |     \n"
      " \\      |     \\              |      |        |                    \n"
      " |      |      \\-------------+------+--------+---\\\n"
      " |      |                    |      |        |   |\n"
      " \\------+--------------------+------/        /   |\n"
      "        |                    |              /    | \n"
      "        \\------S-------------+-------------/     |\n"
      "                             |                   |\n"
      "/-------------\\              |                   |\n"
      "|             |              |             /-----+----\\\n"
      "|             |              |             |     |     \\\n"
      "\\-------------+--------------+-----S-------+-----/      \\\n"
      "              |              |             |             \\\n"
      "              |              |             |             |\n"
      "              |              \\-------------+-------------/\n"
      "              |                            |               \n"
      "              \\----------------------------/ \n";
      /*
      Initial track:
                                /------------\
/-------------\                /             |
|             |               /              S
|             |              /               |
|        eeeeeeee------------+------\        |
\       e     |              |      |        |
 \      e     \              |      |        |
 |      e      \-------------+------+--------+---\
 |      e                    |      |        |   |
 \------E--------------------+------/        /   |
        |                    |              /    |
        \------S-------------+-------------/     |
                             |                   |
/-------------\              |                   c
|             |              |             /-----c----\
|             |              |             |     c     \
\-------------+--------------+-----S-------+--Cccc      \
              |              |             |             \
              |              |             |             |
              |              \-------------+-------------/
              |                            |
              \----------------------------/
              */

      //int time = train_crash(track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
      //int time = train_crash (track, "Eeeeeeeeeeeee", 61, "ccccccC", 207, 2000); // 131
      //train_crash (track, "Cccccccccc", 61, "eeeeeeE", 164, 2000);
      //printf ("%i", time);

      const char src[1024] = {47,45,45,45,45,45,92,32,32,32,47,45,45,45,45,45,92,32,32,32,47,45,45,45,45,45,92,32,32,32,47,45,45,45,45,45,92,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,88,32,32,32,32,32,32,32,32,32,88,32,32,32,32,32,32,32,32,32,88,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,92,45,45,45,45,45,47,32,32,32,92,45,45,45,45,45,47,32,32,32,92,45,45,45,45,45,47,32,32,32,92,45,45,45,45,45,47,32,10};
      //train_crash (src, "ddddddddddddddddddD", 29, "Eeeeeee", 8, 2000);
      Test ();
      //temp (src);

      //= "hello example  something"
      //read_commands("1001");
      stop = clock();
      printf ("\nProcess took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
}

/*

bool collision (train a,  train b) {
    auto &[ia, sa, va] = a;
    auto &[ib, sb, vb] = b;

    list<pair<int,int>>::iterator starta = va.begin(), startb = vb.begin();
    map<pair<int,int>, int> mapa, mapb;

    for (auto ita = starta; distance (starta, ita) < sa; ita++) {
        for (auto itb = startb; distance (startb, itb) < sb; itb++) {
            if (*ita == *itb) return true;
        }
    }

    for (auto &p : va) {
        mapa[p]++;
        if (mapa[p] > 1) return true;
    }

    for (auto &p : vb) {
        mapb[p]++;
        if (mapb[p] > 1) return true;
    }

    return false;
}

void Test () {

  string src;
  const std::string example_track =
    "                                /------------\\\n"
    "/-------------\\                /             |\n"
    "|             |               /              S\n"
    "|             |              /               |\n"
    "|        /----+--------------+------\\        |   \n"
    "\\       /     |              |      |        |     \n"
    " \\      |     \\              |      |        |                    \n"
    " |      |      \\-------------+------+--------+---\\\n"
    " |      |                    |      |        |   |\n"
    " \\------+--------------------+------/        /   |\n"
    "        |                    |              /    | \n"
    "        \\------S-------------+-------------/     |\n"
    "                             |                   |\n"
    "/-------------\\              |                   |\n"
    "|             |              |             /-----+----\\\n"
    "|             |              |             |     |     \\\n"
    "\\-------------+--------------+-----S-------+-----/      \\\n"
    "              |              |             |             \\\n"
    "              |              |             |             |\n"
    "              |              \\-------------+-------------/\n"
    "              |                            |               \n"
    "              \\----------------------------/ \n";

    train_crash(example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
  /*
  ///////////////////////////////random//////////////////////////////
  src = {32,32,32,32,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,47,45,92,32,47,45,92,32,32,10,32,32,32,47,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,92,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,124,32,124,32,124,32,32,10,32,32,47,47,32,32,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,92,92,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,47,32,124,32,47,32,32,10,32,32,124,124,32,32,124,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,92,92,92,32,32,32,32,32,32,32,32,32,32,32,32,124,47,32,32,124,47,32,32,32,10,32,32,124,124,32,32,124,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,92,92,92,92,32,32,32,32,32,32,32,32,32,32,32,124,124,32,32,124,124,32,32,32,10,32,32,92,92,32,32,124,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,92,92,92,32,32,32,32,32,32,32,32,32,32,124,124,32,32,124,124,32,32,32,10,32,32,32,92,92,45,47,47,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,124,124,32,92,45,45,45,45,45,45,45,45,45,47,92,45,45,47,124,32,32,32,10,47,45,92,32,92,45,47,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,92,45,47,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,32,32,10,124,32,32,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,32,32,10,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,10};
   train_crash (src,"Xxx",253,"eeeeeeeeeeeeeeeE",135,2000);


   EXPECTED = 1
ACTUAL = 0


/*
   src = {32,32,32,32,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,47,45,92,32,47,45,92,32,32,10,32,32,32,47,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,92,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,124,32,124,32,124,32,32,10,32,32,47,47,32,32,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,92,92,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,47,32,124,32,47,32,32,10,32,32,124,124,32,32,124,47,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,92,92,92,92,32,32,32,32,32,32,32,32,32,32,32,32,124,47,32,32,124,47,32,32,32,10,32,32,124,124,32,32,124,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,92,92,92,92,32,32,32,32,32,32,32,32,32,32,32,124,124,32,32,124,124,32,32,32,10,32,32,92,92,32,32,124,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,92,92,92,32,32,32,32,32,32,32,32,32,32,124,124,32,32,124,124,32,32,32,10,32,32,32,92,92,45,47,47,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,124,124,32,92,45,45,45,45,45,45,45,45,45,47,92,45,45,47,124,32,32,32,10,47,45,92,32,92,45,47,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,92,45,47,124,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,32,32,10,124,32,32,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,124,32,32,32,10,92,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,47,32,32,32,10};
    train_crash (src,"xxxxxxxxX",4,"Dddddddddddddddd",5,2000);


  src = {47,45,45,45,45,92,32,32,32,32,32,47,45,45,45,45,92,32,10,124,32,32,32,32,32,92,32,32,32,47,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,92,32,47,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,32,83,32,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,32,47,32,92,32,32,32,32,32,32,124,32,10,124,32,32,32,32,32,47,32,32,32,92,32,32,32,32,32,124,32,10,92,45,45,45,45,47,32,32,32,32,32,92,45,45,45,45,47,10};
  cout <<  train_crash (src,"Eeeeeeee",27,"Xxxx",7,100); // Expected: equal to 0  Actual: 21


}
*/

point getstart (graph *track, int pos) {
    point origin = get_origin (track);
    train curr = {'X', 2, malloc (2 * sizeof (point))};

    curr.trn[0] = origin;
    /*

    for (int i = 4; i < 8; i++) {
          point nxt = add (origin, direct[i]);

          if (is_valid (track, origin, origin, i))
              curr.trn[1] = nxt;
    }
    */
    while (pos-->0) {
        getnext (track, &curr);
        //display1(track, &curr);
    }

    return curr.trn[0];
  }
bool collision2 (const train *a, const train *b) {

    for (int i = 0; i < a->size; i++) {
        for (int j = 0; j < b->size; j++) {
            if (equals (a->trn[i], b->trn[j])) return true;
        }
    }

    return false;
  }
train *mktrain2 (graph *track, const char *src, int pos) {

    int size = strlen (src), j = 0;
    point lim;
    const point origin = get_origin (track);
    train *curr = malloc (sizeof (train));
    curr->trn = malloc (size * sizeof (point));

    if (isupper(src[0])) {
        curr->id = src[0];
        lim.x = 0, lim.y = 3;
    } else {
        curr->id = src[size - 1];
        lim.x = 4, lim.y = 8;
    }

    curr->size = size;
    curr->trn[j++] = origin;

    for (int i = lim.x; i < lim.y; i++) {            //set clockwise or counterclockwise
        point nxt = add (origin, direct[i]);

        if (is_inside (track, nxt) && track->grid[nxt.y][nxt.x] != ' ')
            curr->trn[j++] = nxt;
    }
    /*
    while (j < size) {                             // make wagons
        for (int i = 0; i < 8; i++) {
            point prev = curr->trn[j - 2];
            //printf ("%i %i\n", curr->trn[j - 1].x, curr->trn[j - 1].y);
            if (is_valid (track, prev, curr->trn[j - 1], i) && j < size) {
                curr->trn[j] = add (curr->trn[j - 1] , direct[i]);
                j++;
            }

        }
    }
    */
    if (isupper(src[0])) {
        point dest = getstart (track, pos);
          while (pos-->0) {
        // while (!equals (curr->trn[0], dest)) {           // go to start point
            getnext (track, curr);
        }
    } else {
        while (pos-->0) {
            getnext (track, curr);
        }
    }
    /*
    */
    return curr;
  }
