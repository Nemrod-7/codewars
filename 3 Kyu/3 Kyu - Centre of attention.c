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
///////////////////////////////////////////////////////////////////

typedef struct _point {
    int x;
    int y;
} Point;

typedef struct {
    unsigned *pixels;
    unsigned width, height;
} Image;

typedef struct {
    unsigned *values;
    unsigned size;
} unsigned_array;

const Point direct[4] = {{-1,0},{1,0},{0,-1},{0,1}};
////////////////////////////////func def///////////////////////////
unsigned_array central_pixels(Image image, unsigned colour);
void display (Image ,unsigned );
bool answer_matches(unsigned_array actual, unsigned_array expected);
///////////////////////////////////////////////////////////////////
int main () {
    time_t start = clock(), stop;

    unsigned data[4];

  //  Image img = {.width = 4095, .height = 4095};

    //central_pixels (img,5);

    Test();

    stop = clock();
    printf ("Process took %.06f s \n", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
    return 0;
}

unsigned_array find_max (Image img, int val, unsigned *w) {

    unsigned_array out = {.size = 0};
    out.values = malloc (BUFFSIZE * sizeof(unsigned));

    if (val > 0)
        for (int x = 0; x < img.height; ++x)
            for (int y = 0; y < img.width; ++y) {
                int pos = img.width * x + y;
                if (w[pos] == val)
                    out.values[out.size++] = pos;
            }


    return out;
}
int update (Image img, Point p, unsigned *w) {

    if (p.x == 0 || p.y == 0 || p.x == img.height-1 || p.y == img.width-1)
        return 1;

    int i = 4, val = 9999;
    unsigned color = img.pixels[img.width * p.x + p.y];

    while (i-->0) {
        Point nxtp = {p.x + direct[i].x, p.y + direct[i].y};
        int pos = img.width * nxtp.x + nxtp.y;

        if (img.pixels[pos] != color) return 1;

        if (w[pos] != 0)
            val = MIN (val, w[pos] + 1);
    }

    return val;
}
unsigned_array central_pixels(Image img, unsigned color) {

    unsigned maxval = 0;
    int size = img.height * img.width;
    int x = 0, y = 0;
    unsigned *w = malloc (img.width * img.height * sizeof(int));
    memset (w, 0, size * sizeof(int));

    for (x = 0; x < img.height; ++x)
        for (y = 0; y < img.width; ++y) {
            int pos = img.width * x + y;
            if (img.pixels[pos] == color)
                w[pos] = update (img, (Point){x,y}, w);
        }

    for (x = img.height - 1; x >= 0; x--)
        for (y = img.width - 1; y >= 0; y--) {
            int pos = img.width * x + y;
            if (img.pixels[pos] == color) {
                w[pos] = update (img, (Point){x,y}, w);
                maxval = MAX (maxval, w[pos]);

            }
        }

    return find_max (img, maxval, w);
}
///////////////////////////////tools///////////////////////////////
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
    /*
    // Only one red pixel has the maximum depth of 3:
    unsigned red_ctr[] = { 32 };
    expected.values = red_ctr;   expected.size = 1;

    cr_assert(answer_matches(central_pixels(image, 1), expected));

    // Multiple blue pixels have the maximum depth of 2:
    unsigned blue_ctr[] = { 16,17,18,26,27,28,38 };
    expected.values = blue_ctr;   expected.size = 7;
    cr_assert(answer_matches(central_pixels(image, 2), expected));

    // All the green pixels have depth 1, so they are all "central":
    unsigned green_ctr[] = { 35,45,46,47,56,57,58,59 };
    expected.values = green_ctr;   expected.size = 8;
    cr_assert(answer_matches(central_pixels(image, 3), expected));
    */
    // Similarly, all the purple pixels have depth 1:
    unsigned purple_ctr[] = { 2,3,4,5 };
    expected.values = purple_ctr;   expected.size = 4;
    cr_assert(answer_matches(central_pixels(image, 4), expected));

    //central_pixels(image, 5);
    //cr_assert_eq(central_pixels(image, 5).size, 0);

/////////////////////////////////circular disc test/////////////////////////////
  /*
    unsigned imgdata[] = {60,62,62,61,61,62,61,60,60,62,61,
62,60,62,60,61,61,62,62,62,62,61,62,60,60,60,62,61,62,62,60,62,60,61,61,62,60,60,62,60,61,62,61,62,60,62,61,62,62,61,62,62,60,61,62,61,61,62,61,61,60,61,60,60,60,60,60,61,60,60,62,61,61,61,61,61,60,61,61,60,62,60,62,60,60,60,60,62,62,61,61,62,60,62,60,60,62,61,62,62,62,62,61,61,60,60,60,60,61,61,61,61,60,61,62,60,61,62,62,62,61,61,61,61,61,60,60,61,61,60,61,61,62,60,60,60,60,60,61,60,62,60,62,62,62,61,60,61,62,60,60,60,62,62,60,61,60,60,62,62,61,61,61,61,62,61,61,61,60,62,61,62,60,60,62,60,62,62,61,61,62,62,62,62,62,62,60,62,60,61,62,61,62,60,62,62,60,61,60,61,61,62,60,62,62,60,62,62,62,62,61,62,61,60,62,60,62,62,60,60,61,62,61,62,61,61,62,62,60,60,60,61,62,61,62,62,61,62,61,61,61,62,60,62,62,62,60,62,60,61,60,61,61,61,60,60,60,60,60,61,60,61,61,62,62,60,61,61,62,61,62,61,60,62,61,61,60,62,61,60,60,62,62,62,61,61,60,60,61,60,61,62,61,62,61,61,62,61,62,60,62,62,62,60,62,60,61,60,60,61,60,61,60,62,61,62,61,62,62,60,60,61,62,62,61,61,61,62,60,62,62,62,61,61,61,61,62,60,61,60,61,62,62,62,60,60,62,61,60,61,61,61,60,61,61,62,60,62,61,60,61,61,61,61,60,60,62,62,60,61,62,62,60,62,62,60,60,61,13,13,13,61,61,61,60,60,61,61,60,62,62,62,61,61,60,61,61,60,62,60,62,62,60,60,62,62,62,62,62,62,60,60,61,62,61,62,62,13,13,13,13,13,60,60,60,61,62,60,62,61,61,61,61,61,60,60,61,60,60,60,60,60,60,61,60,60,62,61,61,60,62,62,62,60,62,61,13,13,13,13,13,13,13,60,62,62,60,60,62,61,61,62,60,62,61,60,62,60,62,61,62,62,61,62,62,61,60,62,61,62,61,61,60,60,61,62,13,13,13,13,13,13,13,61,62,60,60,60,62,62,61,61,61,60,60,62,62,62,61,61,61,60,62,60,60,61,60,60,61,61,62,60,61,61,60,61,13,13,13,13,13,13,13,62,61,61,62,61,60,60,60,60,62,60,60,62,62,60,60,61,62,60,62,61,62,62,60,62,62,62,61,60,60,62,62,62,60,13,13,13,13,13,62,61,61,62,62,61,60,60,62,60,60,62,62,60,61,62,62,61,62,61,61,60,61,61,62,62,60,60,62,60,61,61,60,60,61,62,13,13,13,62,60,60,62,62,61,60,62,60,62,62,60,61,62,61,60,62,61,61,62,60,62,62,61,62,60,60,62,61,60,62,61,60,62,60,60,60,62,60,62,62,60,62,61,60,60,62,61,60,61,61,60,61,60,60,60,60,60,60,60,60,62,61,61,61,60,62,60,62,61,62,61,61,61,60,62,62,60,60,62,62,62,61,60,62,61,61,61,61,61,61,60,61,60,62,62,60,62,62,60,60,62,62,62,60,60,62,61,60,60,61,60,60,62,60,60,60,61,61,60,60,60,60,61,61,60,62,62,60,61,60,60,62,60,60,60,60,61,61,62,61,61,62,62,60,60,62,61,62,62,62,60,62,62,61,61,62,60,62,60,60,60,61,60,60,60,60,61,61,60,60,60,61,61,62,61,62,62,60,61,61,62,62,62,62,62,61,60,60,61,61,61,61,60,61,61,60,62,60,62,62,61,60,60,62,60,62,62,60,60,61,62,60,61,62,60,60,61,60,61,62,62,62,61,62,62,60,61,61,61,61,61,60,62,62,60,62,61,60,62,61,62,60,60,60,62,60,62,61,61,61,62,60,61,61,62,60,61,61,62,61,60,60,61,62,62,62,61,61,60,62,60,60,62,60,61,61,61,61,61,62,62,60,60,60,61,60,62,60,61,61,61,61,62,60,61,62,61,60,60,61,62,62,62,62,60,61,61,62,62,62,62,62,62,60,60,60,61,62,62,62,61,60,62,61,62,61,60,60,61,61,61,61,61,61,61,61,62,60,61,62,62,61,62,62,62,60,60,60,60,62,60,61,61,60,62,60,62,60,60,60,62,61,60,61,61,61,62,60,61,62,61,62,61,61,61,60,61,62,60,62,62,60,61,60,62,61,60,61,62,60,62,62,62,62,60,60,62,60,62,61,62,61,60,60,62,62,61,61,62,62,61,62,61,62,62,60,61,60,61,60,61,60,62,60,60,60,62,60,61,61,62,60,62,60,62,62,60,60,60,62,61,62,62,62,60,61,62,61,62,60,60,60,60,60,62,61,61,61,62,62,60,62,60,62,60,60,61,60,61,60,61,62,60,60,61,60,60,61,60,62,61,61,60,62,61,62,62,60,61,61,60,61,61,62,61,61,62,60,62,61,61,60,60,62,61,62,62,61,61,60,61,62,61,62,60,61,61,60,62,62,61,60,61,60,60,62,62,62,62,61,61,61,62,62,60,61,60,60,62,61,62,60,62,60,60,60,62,61,60,62,61,61,60,60,62,60,62,62,62,62,61,61,61,61,61,62,60,62,60,62,60,62,61,11,11,11,60,60,61,60,62,60,60,62,61,60,60,61,60,60,61,62,62,62,61,61,62,62,60,60,62,61,62,61,61,60,60,60,62,62,60,61,60,11,11,11,62,62,61,60,60,62,60,61,62,62,62,62,61,60,62,61,61,60,62,60,61,60,61,60,60,60,60,61,61,62,60,61,62,62,62,61,62,11,11,11,62,61,61,60,61,61,62,60,62,60,62,60,60,60,61,62,60,62,60,62,62,61,60,61,61,60,60,60,60,62,60,60,62,61,60,60,60,61,62,62,61,61,61,62,61,62,61,62,62,62,62,61,61,60,60,60,61,61,61,62,12,12,12,60,60,60,61,62,61,60,61,60,61,62,60,60,62,61,60,61,61,60,60,60,61,61,60,60,62,62,60,60,61,62,61,61,60,60,60,12,12,12,12,12,60,60,60,60,60,62,62,61,62,60,62,61,62,60,62,61,62,62,62,60,62,61,62,61,62,60,61,60,61,61,60,61,60,62,62,12,12,12,12,12,60,61,61,62,61,60,61,60,61,61,61,61,60,60,62,62,61,61,61,62,60,62,61,60,62,60,61,61,60,62,61,60,61,62,60,12,12,12,12,12,61,61,62,62,61,61,62,61,60,60,60,61,61,62,62,60,61,60,62,61,60,60,60,61,60,61,60,61,61,60,62,62,61,61,62,60,12,12,12,60,60,60,62,61,60,61,61,61,61,61,60,62,62,61,60,62,62};

    Image image2 = { imgdata, 40, 38 };
    central_pixels(image2, 11);

    central_pixels(image2, 12);

    central_pixels(image2, 13);

////////////////////////////////random/////////////////////////////

    unsigned img_dta[] = { 7, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 5, 6, 6, 7, 7, 7, 7, 6, 5, 5, 6, 6, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 5, 6, 6, 7, 7, 7, 7, 6, 5, 5, 6, 6, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 5, 6, 6, 7, 7, 7, 7, 6, 5, 5, 6, 6, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 7, 7, 7, 7, 7, 6, 6, 5, 5, 5, 5, 7, 7, 7, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 7, 7, 7, 7, 7, 6, 6, 5, 5, 5, 5, 7, 7, 7, 5, 5, 5, 5, 5, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 7, 7, 7, 7, 7, 6, 6, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7, 6, 6, 6, 7, 7, 7, 7, 5, 7, 7, 7, 7, 7, 7, 5, 7, 7, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 7, 7, 7, 7, 5, 7, 7, 7, 7, 7, 7, 5, 7, 7, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 7, 7, 7, 7, 5, 7, 7, 7, 7, 7, 7, 5, 7, 7, 5, 6, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 5, 7, 7, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 7, 6, 6, 7, 6, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 7, 7, 7, 6, 6, 6, 6, 5, 7, 7, 6, 6, 6, 6, 7, 5, 5, 5, 5, 6, 6, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 7, 7, 7, 6, 6, 6, 7, 5, 7, 7, 6, 6, 6};


    Image df = { img_dta, 29, 29 };

    central_pixels(df, 5);
    */
}
