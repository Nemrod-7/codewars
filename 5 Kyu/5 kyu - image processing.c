#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
//#include "codewars_img.h"

#define BUFFSIZE 1024
#define CELL (*(*(data + i) + j))

typedef unsigned char Byte;
typedef struct _pixel {
    unsigned char R, G, B;
} Pixel;

typedef struct _image {
    size_t width, height;
    Pixel** data;
} Image;
typedef struct _point {
    float x, y;
} Point;

void Test ();

float kernel_identity[1][1] = {{1}};
float kernel_blur[3][3] = {
  {1.0/9,1.0/9,1.0/9},
  {1.0/9,1.0/9,1.0/9},
  {1.0/9,1.0/9,1.0/9}
};
float kernel_sharpen[3][3] = {
  { 0,-1, 0},
  {-1, 5,-1},
  { 0,-1, 0}
};
float kernel_edgeDetect[3][3] = {
  {-1,-1,-1},
  {-1, 8,-1},
  {-1,-1,-1}
};
float kernel_laplacianFilter[9][9] = {
  {    0,     0, 0.075,  0.05,  0.05,  0.05, 0.075,     0,     0},
  {    0,  0.05, 0.075, 0.125, 0.125, 0.125, 0.075,  0.05,     0},
  {0.075, 0.075, 0.125, 0.075,     0, 0.075, 0.125, 0.075, 0.075},
  { 0.05, 0.125, 0.075,  -0.3,-0.575,  -0.3, 0.075, 0.125,  0.05},
  { 0.05, 0.125,     0,-0.575,  -0.9,-0.575,     0, 0.125,  0.05},
  { 0.05, 0.125, 0.075,  -0.3,-0.575,  -0.3, 0.075, 0.125,  0.05},
  {0.075, 0.075, 0.125, 0.075,     0, 0.075, 0.125, 0.075, 0.075},
  {    0,  0.05, 0.075, 0.125, 0.125, 0.125, 0.075,  0.05,     0},
  {    0,     0, 0.075,  0.05,  0.05,  0.05, 0.075,     0,     0}
};

unsigned char *processImage(const unsigned char *imageData, int height, int width, void* weights, int n);
Image *init (const Byte *imageData, int width, int height);
Pixel filter (Image *input, int x, int y, void* weights, int n);

void display (Image *input);

int main () {

    time_t start = clock(), stop;

    //processImage (codewars_rgb,codewars_height,codewars_width,kernel_laplacianFilter,9);
    Test ();

    stop = clock();
    printf ("Process took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);
    return 0;
}

int limit (int pos, int size) {
    if (pos < 0) return 0;
    if (pos >= size) return size - 1;
    return pos;
}
Pixel filter (Image *input, int x, int y, void* weights, int size) {

    const int center = size * 0.5;
    const int startx = x - center, endx = x + center, starty = y - center, endy = y + center;
    Pixel conv = {0}, cell, **data = input->data;
    float (*kernel)[size] = weights;
    double R = 0, G = 0, B = 0, k;

    for (int i = startx; i <= endx; ++i) {
        for (int j = starty; j <= endy; ++j) {

            k = *(*(kernel + (i - startx)) + (j - starty));
            cell = (*(*(data + limit (i, input->width)) + limit (j, input->height)));

            R += (k * cell.R);
            G += (k * cell.G);
            B += (k * cell.B);
        }
    }
    conv.R = limit (round (R), 256);
    conv.G = limit (round (G), 256);
    conv.B = limit (round (B), 256);

    return conv;
}
Image *init (const Byte *img, int width, int height) {
    int next = 0;
    Image *new = malloc (sizeof (Image));
    new->data = malloc (width * sizeof (Pixel**));

    new->width = width, new->height = height;

    for (int i = 0; i < width; ++i) {
        new->data[i] = malloc (height * sizeof (Pixel));
        for (int j = 0; j < height; ++j) {
            new->data[i][j].R = img[next++];
            new->data[i][j].G = img[next++];
            new->data[i][j].B = img[next++];
        }
    }

    return new;
}
unsigned char *processImage
    (const unsigned char *imageData, int h, int w, void* weights, int n) {

    int next = 0;
    Image *img = init (imageData, h, w);
    /*
    FILE *fp = fopen ("test.pnm", "w");
    fprintf (fp, "P6\n%d %d\n255\n", img->width, img->height);
    fflush (fp);
    */

    Byte *out = malloc (3 * h * w * sizeof (char));
    for (int x = 0; x < h; ++x)
    //fclose (fp);
    return out;
}

void display (Image *input) {

    Pixel **data = input->data;
    int next = 0;
    for (int i = 0; i < input->width; ++i)
        for (int j = 0; j < input->height; ++j) {

            if (next == 50)
                break;
            printf ("%i R[%3i] G[%3i] B[%3i]\n",
                next++,
                (*(*(data + i) + j)).R,
                CELL.G,
                input->data[i][j].B);

        }
        printf ("\n");
}

int abs (int a) { return a < 0 ? -a : a; }
int RGBImageDataSimilar (const unsigned char *actual, const unsigned char *expected, int h, int w) {
  for (int i = 0, s = 3*w*h; i < s; ++i)
    if (abs (actual[i] - expected[i]) > 3)
      return 0;
  return 1;
}

void putImageData
    (const unsigned char *imageData, int h, int w, int displayHeight, int displayWidth) {
    printf ("\n");
    for (int i = 0; i < 3*h*w; ++i) {
        //if (i % 3*w == 0)
        //    printf ("\n");

        printf ("[%3i]",imageData[i]);

    }
    printf ("\n");
}
void imageTest
    (unsigned char *imageData, int h, int w, void* weights, int n, const unsigned char *expected) {

    unsigned char *processed = processImage (imageData, h, w, weights, n);
    const int TILE_SIZE = 36;
    unsigned char *original = memcpy (malloc (3*w*h), imageData, 3*w*h);
    bool ok = true;

    for (int i = 0; i < 3 * h * w; ++i)
        if (processed[i] != expected[i])
            printf ("%4i :::%3i %3i:::\n",i, processed[i], expected[i]);
          //  ok = false;
        //
      /*
    if (ok == false) {
        putImageData (original, h, w, TILE_SIZE, TILE_SIZE);
        printf (" -> ");
        putImageData (processed, h, w, TILE_SIZE, TILE_SIZE);
        printf (", expected: ");
        putImageData (expected, h, w, TILE_SIZE, TILE_SIZE);
    }
    */
}

void Test () {
    /*
    */
    imageTest ((unsigned char[3]){127,127,127},
              1, 1,
              kernel_identity, 1,
              (unsigned char[3]){127,127,127});

    imageTest ((unsigned char[3]){127,255,0},
              1, 1,
              kernel_blur, 3,
              (unsigned char[3]){127,255,0});

    imageTest ((unsigned char[12]){0,0,0,0,0,0,0,0,0,255,255,255},
              2, 2,
              (float[3][3]){{0.2,0,0},{0,0.2,0.2},{0,0.2,0.2}}, 3,
              (unsigned char[12]){51,51,51,102,102,102,102,102,102,204,204,204});
    /*
    imageTest (codewars_rgb,codewars_height,codewars_width,
              kernel_laplacianFilter,9,
              codewars_laplacianFiltered);
    */
}
