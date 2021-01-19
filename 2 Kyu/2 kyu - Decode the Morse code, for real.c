#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

const double infinity = 9999;

enum {zero, dot, dash, space};

typedef struct _point {
    double x, y;
} Point;

typedef struct _geom {
    size_t size;
    Point *data;
} Geom;

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
#define ABS(x) (x <= 0 ? -(x) : x)
#define BUFFSIZE 1024


const char *morse_code (const char *);

double distance (Point p, Point q) {
    double x = p.x - q.x, y = p.y - q.y;
    return sqrt (x * x + y * y);
}
Point nearest_point (Geom map, Point p) {
    double min = infinity;
    Point *set = map.data, near;

    for (size_t i = 0; i < map.size; ++i) {
        double dist = distance (set[i], p);
        if (dist < min) {
            near = set[i];
            min = dist;
        }
    }

    return near;
}
Point getmean (Geom cluster) {

    Point mean;
    double sumx = 0, sumy = 0;

    for (size_t i = 0; i < cluster.size; ++i) {
        sumx += cluster.data[i].x;
        sumy += cluster.data[i].y;
    }
    mean.x = sumx / cluster.size;
    mean.y = sumy / cluster.size;

    return mean;
}
bool is_inside (Point a, Point b) { return a.x == b.x && a.y == b.y; }
Geom k_means (Geom map, Geom centr) {
    size_t i, j;
    Point near, *p = map.data, *c = centr.data;
    // int index = 2;

    for (i = 0; i < centr.size; ++i) {

        Geom cluster = {.size = 0, .data = malloc (map.size * sizeof (Point))};

        for (j = 0; j < map.size; ++j) {
            near = nearest_point(centr, p[j]);

            if (is_inside (c[i], near))
                cluster.data[cluster.size++] = p[j];

        }
        centr.data[i] = getmean (cluster);
        //printf ("[%5.2f |%5.2f]\n",centr.data[i].x, centr.data[i].y);
    }
    //near.x = centr.data[0].x, near.y = centr.data[1].x;
    //printf ("[%5.2f |%5.2f]\n",centr.data[0].x, centr.data[1].x);
    return centr;
}
Geom filter (const char *src) {

    size_t size, min = 999, mid, max = 0, val[24] = {};
    char ref;

    while (*src) {
        ref = *src;
        size = 0;

        while (*src == ref) {
            size++, src++;
        }

        min = MIN(size, min);
        max = MAX(size, max);
        val[size]++;
    }

    size = max - min;
    mid = (max - min) / 2;

    Geom map = {.size = size, .data = malloc (size * sizeof (Point))};
    Geom centr = {.size = 3, .data = malloc (3 * sizeof (Point))};

    centr.data[0].x = min, centr.data[0].y = val[min];
    centr.data[1].x = mid, centr.data[1].y = val[mid];
    centr.data[2].x = max, centr.data[2].y = val[max];

    int nxt = 0;

    for (size_t i = min; i <= max; ++i) {
        //printf ("%i ", val[i]);
        if (val[i] > 0) {
            map.data[nxt].x = i;
            map.data[nxt].y = val[i];
            nxt++;
        }
    }
    map.size = nxt;
/*
    for (int i = 0; i < map.size; ++i) {
        printf ("%.0f [%.0f]\n", map.data[i].x, map.data[i].y);
    }

    printf ("\n");
    */
    return k_means (map, centr);
}

int unit (Geom K, double bit) {
    if (bit == 0) return 0;
    double a = ABS (K.data[0].x - bit), b = ABS (K.data[1].x - bit), c = ABS (K.data[2].x - bit);
    /*
    printf ("bit : %.0f ", bit);
    printf ("dot : %.2f dash : %.2f space : %.2f\n", a, b, c);
    */
    if (a < b && a < c) return dot;
    if (a > b && b < c) return dash;
    if (a > b && b > c) return space;

    return 0;
}

char *decodeBitsAdvanced (const char *bits) {
    printf ("[%s]\n",bits);
  
    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
  /*
    char bit;
    Geom u_size = filter (bits);
    double size;

    while (*bits) {
        bit = *bits;
        size = 0;

        while (*bits == bit) {
            size++;
            bits++;
        }

        int length = unit (u_size, size);

        if (bit == '1' && length == 1)
            out += sprintf (out, "."); // printf ("-");
        if (bit == '1' && length > 1)
            out += sprintf (out, "-"); // printf (".");
        if (bit == '0' && length == 2)
            out += sprintf (out, " "); // printf (" ");
        if (bit == '0' && length == 3)
            out += sprintf (out, "  "); // printf (" ");
    }
    //printf ("%s\n", output);
  */
    return output;
}

char* decodeMorse (const char *code) {

    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
  
    char *source = strdup (code), *token = strtok (source, " ");

    while (token) {

        if (*(token - 1) == ' ' && out != output)
            out += sprintf (out," ");

        out += sprintf (out,"%s", morse_code (token));

        token = strtok (NULL," ");
    }
  
    return output;
}
