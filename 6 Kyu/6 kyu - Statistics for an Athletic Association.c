#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* strdup (const char *);

int cmp (const void * a, const void * b) { return ( *(int*)a - *(int*)b ); }
char *format (int time) {
    char *os = malloc (10 * sizeof (char));
    sprintf (os, "%02i|%02i|%02i", (time / 3600) % 60, (time / 60) % 60 ,time % 60);
    return os;
}
char *stat (char *src) {
  
    int h, m, s, mid, med;
    int hist[64], size = 0, cnt = 0;
    char *str = strdup (src), *buff = strtok (str, ",");
    char *os = malloc (64 * sizeof (char)), *ptr = os;

    while (buff) {

        sscanf (buff, "%u|%u|%u", &h, &m, &s);
        int time = h * 3600 + m * 60 + s;
        hist[size++] = time;
        cnt += time;
        buff = strtok (NULL, ",");
    }
    qsort (hist, size, sizeof (int), cmp);

    mid = size / 2, med = hist[mid];

    if (!(size % 2)) med = (med + hist[mid - 1]) / 2;

    ptr += sprintf (ptr, "Range: %s", format (hist[size - 1] - hist[0]));
    ptr += sprintf (ptr, " Average: %s", format (cnt / size));
    ptr += sprintf (ptr, " Median: %s", format (med));
    
    return os;
}
