#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define BUFFSIZE 64
///////////////////////////////////////assert//////////////////////
#include <stdarg.h>

union u_tag{
  int ival;
  float fval;
  char *cval;
};

int assert_count = 1;
void cr_assert(int test,...) ;
void Test();

// 01_Example_In_The_Picture (4 of 4 Assertions)
// 02_Circular_Disc_Test (10 of 10 Assertions)
// 03_Diagonal_Test (31 of 31 Assertions)
// 04_Random_Tests (48 of 48 Assertions)
// 05_Big_Test
///////////////////////////////////////////////////////////////////
typedef struct _point {
    int x;
    int y;
} Point;
typedef struct _vertex {
    int x, y, w;
    int pix;
    int dist;
    bool visit;
} Vertex;
typedef Vertex** Matrix;
typedef struct _graph {
    int height, width;
    Vertex **Vmap;
} Graph;

typedef struct _node {
    Vertex *data;
    struct _node *next;
} Node;
typedef struct {
    Node *front, *back;
} Queue;

typedef struct {
    unsigned *pixels;
    unsigned width, height;
} Image;

typedef struct {
    unsigned *values;
    unsigned size;
} unsigned_array;

const Point direct[4] = {{-1,0},{1,0},{0,-1},{0,1}};

///////////////////////////////////////////////////////////////////

Graph init_graph (Image img, unsigned color) {

    int next = 0, x , y;
    Graph new = {img.height, img.width};
    new.Vmap = (Vertex **) malloc (new.height * sizeof (Vertex));

    for (x = 0; x < new.height; ++x) {
        new.Vmap[x] = (Vertex *) malloc (new.width * sizeof (Vertex));
        for (y = 0; y < new.width ; ++y) {
          //  Vertex V = {.w = 0, .pix = -1,
                        //.pix = img.pixels[next++] == color ? -1 : 0,
          //            };
            new.Vmap[x][y].w = 0;
            new.Vmap[x][y].pix = img.pixels[next++] == color ? -1 : 0;
        }
    }
    return new;
}

Vertex *coord (Graph G, Point p) { return &G.Vmap[p.x][p.y];}
bool is_inside (Graph G, Point p) {
    return (p.x >= 0 && p.y >= 0 && p.x < G.height && p.y < G.width);
}
bool edge (Graph G, Point p) {

    if (p.x == 0 || p.y == 0 || p.x == G.height-1 || p.y == G.width-1)
        return true;

    int i = 4, val = coord (G, p)->pix;
    while (i-->0) {
        Point next = {p.x + direct[i].x, p.y + direct[i].y};

        if (coord (G, next)->pix == 0)
            return true;
    }

    return false;
}
int check (Graph G, Point p) {

    if (p.x == 0 || p.y == 0 || p.x == G.height-1 || p.y == G.width-1)
        return 1;

    int i = 4, val = 9999;

    while (i-->0) {
        Point next = {p.x + direct[i].x, p.y + direct[i].y};
        Vertex *v = coord (G, next);
        if (v->pix == 0) return 1;
        printf("%i %i\n",v->pix,v->w);
        if (v->w != 0)
            val = MIN (val, v->w + 1);
    }

    return val;
}
void update (Graph *G) {
    for (int x = 0; x < G->height; ++x)
        for (int y = 0; y < G->width; ++y)
            if (G->Vmap[x][y].visit == true && G->Vmap[x][y].pix == -1)
                G->Vmap[x][y].pix = 0;
}

unsigned_array find_max (Graph G, int val) {
    unsigned_array out = {.size = 0};
    out.values = malloc (BUFFSIZE * sizeof(unsigned));

    if (val > 0)
    for (int x = 0; x < G.height; ++x) {
        for (int y = 0; y < G.width; ++y) {

            Vertex *curr = coord (G, (Point){x,y});
            if (G.Vmap[x][y].w == val) {
                //printf ("%i ", G.width * x + y);
                out.values[out.size++] = G.width * x + y;
            }

        }
    }

    return out;
}
unsigned_array central_pixels(Image img, unsigned color) {

    Graph G = init_graph (img, color);
    unsigned dist = 1, maxval = 0;
    bool valid = true;
    int start, end;
    int index = 50;

    for (int i = 0; i < G.height; ++i) {
        for (int j = 0; j < G.width; ++j) {
            if (G.Vmap[i][j].pix == -1)
                G.Vmap[i][j].w = check (G, (Point){i,j});
        }
    }
    /*

    while (valid) {

        valid = false;

        for (int i = 0; i < G.height; ++i) {
            for (int j = 0; j < G.width; ++j) {

                if (G.Vmap[i][j].pix == -1 && edge (G, (Point){i,j})) {
                    G.Vmap[i][j].pix = valid = 1;
                    G.Vmap[i][j].w = dist;
                }

            }
        }

        for (int i = 0; i < G.height; ++i) {
            for (int j = 0; j < G.width; ++j) {
                if (G.Vmap[i][j].pix == 1) {
                    G.Vmap[i][j].pix = 0;
                }
            }
        }

        dist++;
    }
    */

    //display_graph (G);

    //printf ("%2i ", out.size);
    return find_max (G,maxval);
}

int main () {
    time_t start = clock(), stop;

    unsigned data[4];


    //Test();

    stop = clock();
    printf ("Process took %.06f s \n", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
    return 0;
}
///////////////////////////////tools///////////////////////////////
void display_graph (Graph G) {
    printf ("\n");
    for (int x = 0; x < G.height; ++x) {
        for (int y = 0; y < G.width; ++y) {
          //int coord = G.width * x + y;
          if (G.Vmap[x][y].w > 0)
              printf ("%2i",G.Vmap[x][y].w);
          else
              printf ("  ");
        }
        printf ("\n");
    }
    printf ("\n");
}
void display (Image img, unsigned colour) {
    printf ("\n");
    for (int x = 0; x < img.height; ++x) {
        for (int y = 0; y < img.width; ++y) {
          int coord = img.width * x + y;
          if (img.pixels[coord] == colour)
              printf ("%2i",img.pixels[coord]);
          else
              printf ("  ");

        }
        printf ("\n");
    }
    printf ("\n");
}
////////////////////////////////tests//////////////////////////////
void cr_assert(int test,...) {

  va_list list;
  va_start (list,test);
  union u_tag data;

  assert_count++;

  if (test == 0) {
    //data.cval = va_arg(list,char *);
      printf ("\n:::test->%i :::\n",assert_count);
      printf ("%s\n",va_arg(list,char *));
    }

    va_end (list);
}
void cr_assert_eq(int test, int cond,...) {

  #define TOKEN '%'
  #define SPACE ' '
  va_list list;
  va_start (list,cond);
  union u_tag data;
  char *c;

  assert_count++;
  if (test != cond) {
      printf ("\n:::test->%i :::\n",assert_count);
      c = va_arg(list,char *);

      do {

        if (*c == TOKEN) {
            c++;

            switch (*c) {
                case 'i':
                  data.ival = va_arg(list,int);
                  printf ("%i",data.ival);
                  break;
                case 'd':
                  data.ival = va_arg(list,int);
                  printf ("%i",data.ival);
                  break;
                case 'l':
                  data.ival = va_arg(list,int);
                  printf ("%l",data.ival);
                  break;
                case 'f':
                  data.fval = va_arg(list,double);
                  printf ("%f",data.fval);
                  break;
                case 's':
                  data.cval = va_arg(list,char *);
                  printf ("%s",data.cval);
                  break;
                case 'z':
                  data.ival = va_arg(list,int);
                  printf ("%i",data.ival);
                  break;
                default:
                  c++;
            }
            while (*c != SPACE)
              c++;
        }

        putchar (*c);

      } while (*c++);

      printf ("\n");
    }

}
int unsigned_compare(const void *a, const void *b) {
 unsigned ua = *(unsigned*)a, ub = *(unsigned*)b;
 return ua < ub ? -1 : ua > ub ? 1 : 0;
}

bool answer_matches(unsigned_array actual, unsigned_array expected) {
    bool fail = false;
    unsigned i;

    if( !actual.values || actual.size != expected.size )
        fail = true;
    else {
        qsort(actual.values,   actual.size,   sizeof(unsigned), unsigned_compare);

        qsort(expected.values, expected.size, sizeof(unsigned), unsigned_compare);

        for(i=0; !fail && i < actual.size; i++)
            if( actual.values[i] != expected.values[i] )
                fail = true;
    }

    if( fail ) {
        fprintf(stderr, "Wrong answer returned: [");
        for(i=0; i < actual.size; i++)
            fprintf(stderr, "%s%d", i!=0 ? ", " : " ", actual.values[i]);
            fprintf(stderr, " ], should be [");
        for(i=0; i < expected.size; i++)
            fprintf(stderr, "%s%d", i!=0 ? ", " : " ", expected.values[i]);
            fprintf(stderr, " ]\n");
    }

    return !fail;
}
void Test () {


    //////////////////////////////sample tests//////////////////////////////////
    // red = 1 // purple = 4 // blue = 2 // green = 3 //
    unsigned image_data[] = {1,1,4,4,4,4,2,2,2,2,
                             1,1,1,1,2,2,2,2,2,2,
                             1,1,1,1,2,2,2,2,2,2,
                             1,1,1,1,1,3,2,2,2,2,
                             1,1,1,1,1,3,3,3,2,2,
                             1,1,1,1,1,1,3,3,3,3};
    Image image = { image_data, 10, 6 };
    unsigned_array expected;

    // Only one red pixel has the maximum depth of 3:
    unsigned red_ctr[] = { 32 };
    expected.values = red_ctr;   expected.size = 1;
    central_pixels(image, 1);
    /*
    cr_assert(answer_matches(central_pixels(image, 1), expected));

    // Multiple blue pixels have the maximum depth of 2:
    unsigned blue_ctr[] = { 16,17,18,26,27,28,38 };
    expected.values = blue_ctr;   expected.size = 7;
    cr_assert(answer_matches(central_pixels(image, 2), expected));

    // All the green pixels have depth 1, so they are all "central":
    unsigned green_ctr[] = { 35,45,46,47,56,57,58,59 };
    expected.values = green_ctr;   expected.size = 8;
    cr_assert(answer_matches(central_pixels(image, 3), expected));

    // Similarly, all the purple pixels have depth 1:
    unsigned purple_ctr[] = { 2,3,4,5 };
    expected.values = purple_ctr;   expected.size = 4;
    //central_pixels(image, 5);
    cr_assert_eq(central_pixels(image, 5).size, 0);
*/
/////////////////////////////////circular disc test/////////////////////////////
/*
    unsigned imgdata[] = {60,62,62,61,61,62,61,60,60,62,61,
62,60,62,60,61,61,62,62,62,62,61,62,60,60,60,62,61,62,62,60,62,60,61,61,62,60,60,62,60,61,62,61,62,60,62,61,62,62,61,62,62,60,61,62,61,61,62,61,61,60,61,60,60,60,60,60,61,60,60,62,61,61,61,61,61,60,61,61,60,62,60,62,60,60,60,60,62,62,61,61,62,60,62,60,60,62,61,62,62,62,62,61,61,60,60,60,60,61,61,61,61,60,61,62,60,61,62,62,62,61,61,61,61,61,60,60,61,61,60,61,61,62,60,60,60,60,60,61,60,62,60,62,62,62,61,60,61,62,60,60,60,62,62,60,61,60,60,62,62,61,61,61,61,62,61,61,61,60,62,61,62,60,60,62,60,62,62,61,61,62,62,62,62,62,62,60,62,60,61,62,61,62,60,62,62,60,61,60,61,61,62,60,62,62,60,62,62,62,62,61,62,61,60,62,60,62,62,60,60,61,62,61,62,61,61,62,62,60,60,60,61,62,61,62,62,61,62,61,61,61,62,60,62,62,62,60,62,60,61,60,61,61,61,60,60,60,60,60,61,60,61,61,62,62,60,61,61,62,61,62,61,60,62,61,61,60,62,61,60,60,62,62,62,61,61,60,60,61,60,61,62,61,62,61,61,62,61,62,60,62,62,62,60,62,60,61,60,60,61,60,61,60,62,61,62,61,62,62,60,60,61,62,62,61,61,61,62,60,62,62,62,61,61,61,61,62,60,61,60,61,62,62,62,60,60,62,61,60,61,61,61,60,61,61,62,60,62,61,60,61,61,61,61,60,60,62,62,60,61,62,62,60,62,62,60,60,61,13,13,13,61,61,61,60,60,61,61,60,62,62,62,61,61,60,61,61,60,62,60,62,62,60,60,62,62,62,62,62,62,60,60,61,62,61,62,62,13,13,13,13,13,60,60,60,61,62,60,62,61,61,61,61,61,60,60,61,60,60,60,60,60,60,61,60,60,62,61,61,60,62,62,62,60,62,61,13,13,13,13,13,13,13,60,62,62,60,60,62,61,61,62,60,62,61,60,62,60,62,61,62,62,61,62,62,61,60,62,61,62,61,61,60,60,61,62,13,13,13,13,13,13,13,61,62,60,60,60,62,62,61,61,61,60,60,62,62,62,61,61,61,60,62,60,60,61,60,60,61,61,62,60,61,61,60,61,13,13,13,13,13,13,13,62,61,61,62,61,60,60,60,60,62,60,60,62,62,60,60,61,62,60,62,61,62,62,60,62,62,62,61,60,60,62,62,62,60,13,13,13,13,13,62,61,61,62,62,61,60,60,62,60,60,62,62,60,61,62,62,61,62,61,61,60,61,61,62,62,60,60,62,60,61,61,60,60,61,62,13,13,13,62,60,60,62,62,61,60,62,60,62,62,60,61,62,61,60,62,61,61,62,60,62,62,61,62,60,60,62,61,60,62,61,60,62,60,60,60,62,60,62,62,60,62,61,60,60,62,61,60,61,61,60,61,60,60,60,60,60,60,60,60,62,61,61,61,60,62,60,62,61,62,61,61,61,60,62,62,60,60,62,62,62,61,60,62,61,61,61,61,61,61,60,61,60,62,62,60,62,62,60,60,62,62,62,60,60,62,61,60,60,61,60,60,62,60,60,60,61,61,60,60,60,60,61,61,60,62,62,60,61,60,60,62,60,60,60,60,61,61,62,61,61,62,62,60,60,62,61,62,62,62,60,62,62,61,61,62,60,62,60,60,60,61,60,60,60,60,61,61,60,60,60,61,61,62,61,62,62,60,61,61,62,62,62,62,62,61,60,60,61,61,61,61,60,61,61,60,62,60,62,62,61,60,60,62,60,62,62,60,60,61,62,60,61,62,60,60,61,60,61,62,62,62,61,62,62,60,61,61,61,61,61,60,62,62,60,62,61,60,62,61,62,60,60,60,62,60,62,61,61,61,62,60,61,61,62,60,61,61,62,61,60,60,61,62,62,62,61,61,60,62,60,60,62,60,61,61,61,61,61,62,62,60,60,60,61,60,62,60,61,61,61,61,62,60,61,62,61,60,60,61,62,62,62,62,60,61,61,62,62,62,62,62,62,60,60,60,61,62,62,62,61,60,62,61,62,61,60,60,61,61,61,61,61,61,61,61,62,60,61,62,62,61,62,62,62,60,60,60,60,62,60,61,61,60,62,60,62,60,60,60,62,61,60,61,61,61,62,60,61,62,61,62,61,61,61,60,61,62,60,62,62,60,61,60,62,61,60,61,62,60,62,62,62,62,60,60,62,60,62,61,62,61,60,60,62,62,61,61,62,62,61,62,61,62,62,60,61,60,61,60,61,60,62,60,60,60,62,60,61,61,62,60,62,60,62,62,60,60,60,62,61,62,62,62,60,61,62,61,62,60,60,60,60,60,62,61,61,61,62,62,60,62,60,62,60,60,61,60,61,60,61,62,60,60,61,60,60,61,60,62,61,61,60,62,61,62,62,60,61,61,60,61,61,62,61,61,62,60,62,61,61,60,60,62,61,62,62,61,61,60,61,62,61,62,60,61,61,60,62,62,61,60,61,60,60,62,62,62,62,61,61,61,62,62,60,61,60,60,62,61,62,60,62,60,60,60,62,61,60,62,61,61,60,60,62,60,62,62,62,62,61,61,61,61,61,62,60,62,60,62,60,62,61,11,11,11,60,60,61,60,62,60,60,62,61,60,60,61,60,60,61,62,62,62,61,61,62,62,60,60,62,61,62,61,61,60,60,60,62,62,60,61,60,11,11,11,62,62,61,60,60,62,60,61,62,62,62,62,61,60,62,61,61,60,62,60,61,60,61,60,60,60,60,61,61,62,60,61,62,62,62,61,62,11,11,11,62,61,61,60,61,61,62,60,62,60,62,60,60,60,61,62,60,62,60,62,62,61,60,61,61,60,60,60,60,62,60,60,62,61,60,60,60,61,62,62,61,61,61,62,61,62,61,62,62,62,62,61,61,60,60,60,61,61,61,62,12,12,12,60,60,60,61,62,61,60,61,60,61,62,60,60,62,61,60,61,61,60,60,60,61,61,60,60,62,62,60,60,61,62,61,61,60,60,60,12,12,12,12,12,60,60,60,60,60,62,62,61,62,60,62,61,62,60,62,61,62,62,62,60,62,61,62,61,62,60,61,60,61,61,60,61,60,62,62,12,12,12,12,12,60,61,61,62,61,60,61,60,61,61,61,61,60,60,62,62,61,61,61,62,60,62,61,60,62,60,61,61,60,62,61,60,61,62,60,12,12,12,12,12,61,61,62,62,61,61,62,61,60,60,60,61,61,62,62,60,61,60,62,61,60,60,60,61,60,61,60,61,61,60,62,62,61,61,62,60,12,12,12,60,60,60,62,61,60,61,61,61,61,61,60,62,62,61,60,62,62};

    Image image2 = { imgdata, 40, 38 };
    central_pixels(image2, 11);

    central_pixels(image2, 12);

    central_pixels(image2, 13);
*/
////////////////////////////////random/////////////////////////////
  /*
    unsigned img_dta[] = { 7, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 5, 6, 6, 7, 7, 7, 7, 6, 5, 5, 6, 6, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 5, 6, 6, 7, 7, 7, 7, 6, 5, 5, 6, 6, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 5, 6, 6, 7, 7, 7, 7, 6, 5, 5, 6, 6, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 7, 7, 7, 7, 7, 6, 6, 5, 5, 5, 5, 7, 7, 7, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 7, 7, 7, 7, 7, 6, 6, 5, 5, 5, 5, 7, 7, 7, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 7, 7, 7, 7, 7, 6, 6, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7, 6, 6, 6, 7, 7, 7, 7, 5, 7, 7, 7, 7, 7, 7, 5, 7, 7, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 7, 7, 7, 7, 5, 7, 7, 7, 7, 7, 7, 5, 7, 7, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 7, 7, 7, 7, 5, 7, 7, 7, 7, 7, 7, 5, 7, 7, 5, 6, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 7, 7, 7, 6, 6, 6, 6, 5, 7, 7, 6, 6, 6, 6, 7, 5, 5, 5, 5, 6, 6, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6};


    Image df = { img_dta, 29, 29 };

    central_pixels(df, 5);
    */
}
