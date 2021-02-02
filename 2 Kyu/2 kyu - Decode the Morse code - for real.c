#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h> // -lm
////////////////////////////////////////////////////////////////////////////////
void Test ();
int assert_count = 1;

void assertEquals (char *actual, char *expected) {

  if (strcmp (actual, expected) != 0) {
      printf ("test >> %i actual : [%s] expected : [%s]\n",assert_count, actual, expected);
  }
  assert_count++;
}
////////////////////////////////////////////////////////////////////////////////
const char *morse[55] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", ".-.-.-", "--..--", "..--..", ".----.", "-.-.--", "-..-.", "-.--.", "-.--.-", ".-...", "---...", "-.-.-.", "-...-", ".-.-.", "-....-", "..--.-", ".-..-.", "...-..-", ".--.-.", "...---..."};
const char *ascii[55] = {"A",  "B",    "C",    "D",   "E", "F",    "G",   "H",    "I",  "J",    "K",   "L",    "M",  "N",  "O",   "P",    "Q",    "R",   "S",   "T", "U",   "V",    "W",   "X",    "Y",    "Z",    "0",     "1",     "2",     "3",     "4",     "5",     "6",     "7",     "8",     "9",     ".",      ",",      "?",      "'",      "!",      "/",     "(",     ")",      "&",     ":",      ";",      "=",     "+",     "-",      "_",      "\"",     "$",       "@",      "SOS"};

const float infinity = 9999;

enum {zero, dot, dash, space};

typedef struct _point {
    float x, y;
} Point;

typedef struct _geom {
    size_t size;
    Point *data;
} Geom;

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
#define ABS(x) (x <= 0 ? -(x) : x)
#define BUFFSIZE 1024

/*
//#define FOUND 0
"Dot" – is 1 time unit long.
"Dash" – is 3 time units long.
Pause between dots and dashes in a character – is 1 time unit long.
Pause between characters inside a word – is 3 time units long.
Pause between words – is 7 time units long.
*/

const char *morse_code (const char *code) {

    for (int index = 0; index < 55; index++)
        if (strcmp (code, morse[index]) == 0)
            return ascii[index];

    return NULL;
  }

float distance (const Point p, const Point q) {
    float x = p.x - q.x, y = p.y - q.y;
    return sqrt (x * x + y * y);
}
Point nearest_point (const Geom map, const Point p) {
    float min = infinity;
    Point *set = map.data, near;

    for (int i = 0; i < map.size; ++i) {
        float dist = distance (set[i], p);
        if (dist < min) {
            near = set[i];
            min = dist;
        }
    }

    return near;
}
Point getmean (const Geom cluster) {

    Point mean;
    float sumx = 0, sumy = 0;

    for (int i = 0; i < cluster.size; ++i) {
        sumx += cluster.data[i].x;
        sumy += cluster.data[i].y;
    }
    mean.x = sumx / cluster.size;
    mean.y = sumy / cluster.size;

    return mean;
}
bool is_inside (const Point a,const Point b) { return a.x == b.x && a.y == b.y; }
Geom k_means (const Geom map, Geom ctid) {
    int i, j, index = 2;

    Point near, *p = map.data, *c = ctid.data;
    Geom nxtc = {.size = 3, .data = malloc (3 * sizeof (Point))};

        for (i = 0; i < ctid.size; ++i) {
            Geom cluster = {.size = 0, .data = malloc (map.size * sizeof (Point))};
            //printf ("[%.0f,%.0f]-> ",  c[i].x,  c[i].y);
            /*
            for (int i = 0; i < ctid.size; ++i) {
                printf ("%.2f [%.2f]\n", ctid.data[i].x, ctid.data[i].y);
            }
            */

            for (j = 0; j < map.size; ++j) {
                near = nearest_point (ctid, p[j]);
                //printf ("[%.0f,%.0f]",  near.x,  near.y);
                if (is_inside (c[i], near))
                    cluster.data[cluster.size++] = p[j];

            }
            nxtc.data[i] = getmean (cluster);
            //printf("\n\n");
        }
        for (i = 0; i < ctid.size; ++i)
             ctid.data[i] = nxtc.data[i];

    return ctid;
}

Geom mk_grph (const int tmp[64]) {
    Geom new = {.size = 64, .data = malloc (64 * sizeof (Point))};
    int next = 0;
    for (int i = 0; i < 64; ++i) {
        if (tmp[i] != 0) {
            new.data[next].x = (float)i;
            new.data[next++].y = tmp[i];
        }
    }
    new.size = next;
    return new;
}
Geom mk_seeds (const Geom grph, const int mode) {
    float size = grph.size / (float)7;
    Geom new = {.size = 0, .data = malloc (3 * sizeof (Point))};
    if (size == 0) return new;
    int i = 0;

    new.data[0] = grph.data[i++];
    if (mode > 2) new.data[i++].x = round (grph.data[0].x * 3 * size);
    new.data[i++] = grph.data[grph.size - 1];


    new.size = i;
    return new;
}

Geom filter (const char *src) {

    int size, mapo[64] = {0}, mapz[64] = {0};
    char ref;

    for (int i = 0; i < 64; ++i) {
        mapo[i] = 0;
        mapz[i] = 0;
    }


    while (*src) {
        ref = *src;
        size = 0;

        while (*src == ref) {
            size++, src++;
        }

        if (ref == '1') mapo[size]++;
        if (ref == '0') mapz[size]++;
    }

    Geom one = mk_grph (mapo);
    Geom zero = mk_grph (mapz);

    Geom clusto = k_means (one, mk_seeds (one, 2));
    Geom clustz = k_means (zero, mk_seeds (zero, 3));

    Geom unit = {.size = 3, .data = malloc (3 * sizeof (Point))};

    /*

    for (int i = 0; i < clusto.size; ++i)
    printf ("%.02f %.02f\n", clusto.data[i].x, clusto.data[i].y);
    for (int i = 0; i < zero.size; ++i)
        printf ("%.2f %.2f\n", zero.data[i].x, zero.data[i].y);

    */
    unit.data[0].x = MIN (clusto.data[0].x, clustz.data[0].x);
    unit.data[1].x = MAX (clusto.data[1].x, clustz.data[1].x);
    unit.data[2].x = clustz.data[2].x;

    return unit;
}
char *clean (const char *src) {
    size_t size = strlen (src);
    char *bits = malloc (size * sizeof (char)), *end = &bits[size - 1];
    strcpy (bits, src);

    while (*bits == '0') bits++;
    while (*end == '0') end--;

    *(end + 1) = '\0';
    return bits;
  }

char* decodeMorse (const char *code) {

    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
    char *source = strdup (code), *token = strtok (source, " ");
    // int index = 0;

    while (token) {

        if (*(token - 1) == ' ' && out != output)
            out += sprintf (out," ");

        out += sprintf (out,"%s", morse_code (token));

        token = strtok (NULL," ");
    }

    return output;
}
char *decodeBitsAdvanced (const char *src) {

    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
    char *bits = clean (src), bit;

    Geom u_size = filter (bits);
    float dot = u_size.data[0].x, dash = u_size.data[1].x, space = u_size.data[2].x;
    float size;
    //printf ("%.2f %.2f %.2f\n", dot, dash, space);
    while (*bits) {
        bit = *bits;
        size = 0;

        while (*bits == bit) {
            size++;
            bits++;
        }

        if (bit == '1') {
            if (ABS (size - dot) <= ABS (size - dash))
                out += sprintf (out, "."); // printf ("-");
            else
                out += sprintf (out, "-"); // printf (".");
        }

        if (bit == '0') {
            if (ABS (size - dot) > ABS (size - dash))
                out += sprintf (out, " "); // printf (" ");

            if (ABS (size - space) < ABS (size - dash))
                out += sprintf (out, " "); // printf (" ");
        }
    }
    //printf ("%s ",output);
    return output;
}

int main () {
    char *bits = "0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000";

    char *morse = decodeBitsAdvanced(bits);
    char *decoded = decodeMorse (morse);

    printf ("%s",decoded);
    Test ();
}

void Test () {
    char *bits, *decrypted;
    bits = "0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000";
    decrypted = decodeMorse (decodeBitsAdvanced(bits));

    printf ("%s\n", decrypted);

    bits = "";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "0";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "0000000000";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "01110";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "1100110011001100000011000000111111001100111111001111110000000000000011001111110011111100111111000000110011001111110000001111110011001100000011";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "111";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));

    bits = "1";
    printf ("[%s]\n", decodeMorse (decodeBitsAdvanced (bits)));
    /*
    /*
    */
    // testExampleFromDescription() {
    /*
        assertEquals(decodeMorse(decodeBitsAdvanced("0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000")), "HEY JUDE");

    // testShortMessages() {
        assertEquals(decodeMorse(decodeBitsAdvanced("")), "");
        assertEquals(decodeMorse(decodeBitsAdvanced("0")), "");
        assertEquals(decodeMorse(decodeBitsAdvanced("000000000000000000000000000000000000000000")), "");
        assertEquals(decodeMorse(decodeBitsAdvanced("1")), "E");
        assertEquals(decodeMorse(decodeBitsAdvanced("101")), "I");
        assertEquals(decodeMorse(decodeBitsAdvanced("1001")), "EE");  // error
        assertEquals(decodeMorse(decodeBitsAdvanced("10001")), "EE"); // error
        assertEquals(decodeMorse(decodeBitsAdvanced("100001")), "EE");// error
        assertEquals(decodeMorse(decodeBitsAdvanced("10000001")), "E E");
        assertEquals(decodeMorse(decodeBitsAdvanced("100000001")), "E E");
        assertEquals(decodeMorse(decodeBitsAdvanced("1000000001")), "E E");
        assertEquals(decodeMorse(decodeBitsAdvanced("10000000001")), "E E");
        assertEquals(decodeMorse(decodeBitsAdvanced("10111")), "A");
        assertEquals(decodeMorse(decodeBitsAdvanced("1110111")), "M");
        assertEquals(decodeMorse(decodeBitsAdvanced("111000111")), "I");

    // testMultipleBitsPerDot() {
        assertEquals(decodeMorse(decodeBitsAdvanced("111")), "E");
        assertEquals(decodeMorse(decodeBitsAdvanced("1111111")), "E");
        assertEquals(decodeMorse(decodeBitsAdvanced("110011")), "I");
        assertEquals(decodeMorse(decodeBitsAdvanced("111110000011111")), "I");
        assertEquals(decodeMorse(decodeBitsAdvanced("11111100111111")), "M");

    // testExtraZeros() {
        assertEquals(decodeMorse(decodeBitsAdvanced("01110")), "E");
        assertEquals(decodeMorse(decodeBitsAdvanced("000000011100000")), "E");

    // testEmptyMessage() {
        assertEquals(decodeMorse(decodeBitsAdvanced("")), "");
        assertEquals(decodeMorse(decodeBitsAdvanced("0")), "");
        assertEquals(decodeMorse(decodeBitsAdvanced("0000000000")), "");

    // testLongMessage() {
        assertEquals(decodeMorse(decodeBitsAdvanced("1100110011001100000011000000111111001100111111001111110000000000000011001111110011111100111111000000110011001111110000001111110011001100000011")), "HEY JUDE");
        assertEquals(decodeMorse(decodeBitsAdvanced("00000000000111111100000011010001110111000000001110000000000000000001111111011111100001101111100000111100111100011111100000001011100000011111110010001111100110000011111100101111100000000000000111111100001111010110000011000111110010000011111110001111110011111110000010001111110001111111100000001111111101110000000000000010110000111111110111100000111110111110011111110000000011111001011011111000000000000111011111011111011111000000010001001111100000111110111111110000001110011111100011111010000001100001001000000000000000000111111110011111011111100000010001001000011111000000100000000101111101000000000000011111100000011110100001001100000000001110000000000000001101111101111000100000100001111111110000000001111110011111100011101100000111111000011011111000111111000000000000000001111110000100110000011111101111111011111111100000001111110001111100001000000000000000000000000000000000000000000000000000000000000")), "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
        /*
        assertEquals(decodeMorse(decodeBitsAdvanced("11111000001111111000011111100000111111111111111000011111111111111000000111111111111111100001110000011111100000001111000000000000000011111111111111000000111110000011111111111111100000011110000011111111111111100001111111111111110000000000000000000000000000000000011111111111111110000000000000001111000000111110000011110000000111100000000000000111110000000000000000000000000000000000011111111111111100000111111111111111000011111000001111111111111100000000000000001111111000000111111000000011111111111111000000000000000011110000001111100000000000000011111111111111100000111111000111111111111111000011110000000000000000111111111111110000000111100000111111111111110000000000000000000000000000000000011111111111111100000111111000011111000000111110000000000000001111110000111111111111111100000011110000000000000011111111111111100001111111111111110000111111111111110000000000000001111100000001111111111111110000000111111111111111110000000000000000111111111111111000001111100000000000000000000000000000000000011110001111100000011111111111111110000011100000000000000011111111111111110000011111111111110000001111111111111111000000000000001111111111111110000001111100001111110000001111111111111111000000000000000000000000000000000011110000011111111111111100000011111111111111100001111111111111111000000000000001111110001111000011111111111100000000000000001111111111111100000011111111111111100000000000000110000011111111111111100000111111111111111100000111110000000000000001111110000111110000111111000000000000000000000000000000000011111111111111110001111111111111111000001111111111111111000000000000000111100000111110000111100000111111111111111000000000000000111111000000000000000111000000111111111111111000111100000000000000000000000000000000000011111111111111100000000000000011111110000111100000111111000001111110000000000000001111110000000000000000000000000000000000000111111000111111111111111100000111100000011111110000000000000011110000111111111111111000000000000011111111111111000001111111111111111000001111100001111100000000000000011111111111111000001111110000011111111111111111000011111111111111100000000000000000000000000000000000001111111111111111000001111110000011110000000000000111111111111111000001111111111111111000011111111111111110000000000000001111111111111100000011111111111111100000111100000000000000011111100000111111111111110000001110000011111111111111110000011111000011111111111111")), "SOS! THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.");

    // finalTest() {
        assertTrue("Oops, sorry, wrong!", decodeMorse(decodeBitsAdvanced("00000000000000011111111000000011111111111100000000000111111111000001111111110100000000111111111111011000011111111011111111111000000000000000000011111111110000110001111111111111000111000000000001111111111110000111111111100001100111111111110000000000111111111111011100001110000000000000000001111111111010111111110110000000000000001111111111100001111111111110000100001111111111111100000000000111111111000000011000000111000000000000000000000000000011110001111100000111100000000111111111100111111111100111111111111100000000011110011111011111110000000000000000000000111111111110000000011111000000011111000000001111111111110000000001111100011111111000000000111111111110000011000000000111110000000111000000000011111111111111000111001111111111001111110000000000000000000001111000111111111100001111111111111100100000000001111111100111111110111111110000000011101111111000111000000001001111111000000001111111111000000000111100001111111000000000000011111111100111111110111111111100000000000111111110000001100000000000000000000111111101010000010000001111111100000000011111000111111111000000111111111110011111111001111111110000000011000111111110000111011111111111100001111100001111111100000000000011110011101110001000111111110000000001111000011111110010110001111111111000000000000000000111111111110000000100000000000000000011110111110000001000011101110000000000011111111100000011111111111100111111111111000111111111000001111111100000000000001110111111111111000000110011111111111101110001111111111100000000111100000111100000111111111100000111111111111000000011111111000000000001000000111100000001000001111100111111111110000000000000000000010001111111100000011111111100000000000000100001111111111110111001111111111100000111111100001111111111000000000000000000000000011100000111111111111011110000000010000000011111111100011111111111100001110000111111111111100000000000000111110000011111001111111100000000000011100011100000000000011111000001111111111101000000001110000000000000000000000000000111110010000000000111111111000011111111110000000000111111111111101111111111100000000010000000000000011111111100100001100000000000000111100111100000000001100000001111111111110000000011111111111000000000111100000000000000000000111101111111111111000000000001111000011111000011110000000001100111111100111000000000100111000000000000111110000010000011111000000000000001111111111100000000110111111111100000000000000111111111111100000111000000000111111110001111000000111111110111111000000001111000000000010000111111111000011110001111111110111110000111111111111000000000000000000000000111111111110000000111011111111100011111110000000001111111110000011111111100111111110000000001111111111100111111111110000000000110000000000000000001000011111111110000000001111111110000000000000000000000011111111111111000000111111111000001111111110000000000111111110000010000000011111111000011111001111111100000001110000000011110000000001011111111000011111011111111110011011111111111000000000000000000100011111111111101111111100000000000000001100000000000000000011110010111110000000011111111100000000001111100011111111111101100000000111110000011110000111111111111000000001111111111100001110111111111110111000000000011111111101111100011111111110000000000000000000000000010000111111111100000000001111111110111110000000000000000000000110000011110000000000001111111111100110001111111100000011100000000000111110000000011111111110000011111000001111000110000000011100000000000000111100001111111111100000111000000001111111111000000111111111100110000000001111000001111111100011100001111111110000010011111111110000000000000000000111100000011111000001111000000000111111001110000000011111111000100000000000011111111000011001111111100000000000110111000000000000111111111111000100000000111111111110000001111111111011100000000000000000000000000")).equals("MGY CQD CQD SOS TITANIC POSITION 41.44 N 50.24 W. REQUIRE IMMEDIATE ASSISTANCE. COME AT ONCE. WE STRUCK AN ICEBERG. SINKING"));
    */

}

/*
struct _sites filter (const char *bits, char c[]) {
  char *code = strdup (bits), *token = strtok (code, "0");

  int size, min = 99, max = 0,num, med[24] = {}, next = 0;
  int dot = 99;

  while (token) {
    size = strlen (token);
    //printf ("%i %s\n", size , token);
    med[size]++;
    min = MIN(size, min);
    max = MAX(size, max);
    token = strtok (NULL, "0");
  }
  struct _sites K;
  //printf ("[%i]\n", min);
  int k = 0;
  float clust[5] = {}, div = 0;

  for (int i = 1; i <= max + 1; ++i)
  printf ("[%i]", i);
  printf ("\n");

  for (int i = 1; i <= max + 1; ++i) {
    if (med[i] == 0) {
      K.cluster[k].low = min;
      k++;
      min = 99;
    }
    div++;
    if (med[i])
    min = MIN(min, i);
    //K.cluster[k].low = min;
    K.cluster[k].up = i;
    clust[k] += med[i];
    //printf ("[%i] %i\n", i, min);
    //printf ("[%i]",med[i]);
  }

  printf ("\n");
  for (int i = 0; i < k; ++i) {
    printf ("[%i][%i]\n", K.cluster[i].low, K.cluster[i].up);
    //printf ("[%.2f]", clust[i]);
  }

}
*/
