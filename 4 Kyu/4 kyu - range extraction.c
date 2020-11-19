#include <stdlib.h>

#define BUFFSIZE 1024
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))


char *range_extraction(const int *args, size_t n) {
    int min = args[n - 1], max = args[0], cnt = 0;
    char *output = malloc (BUFFSIZE * sizeof (char)), *out = output;
    *output = '\0';
    //printf ("size :::%i:::\n",n);
    for (int i = 0; i < n; ++i) {
        min = MIN (min, args[i]), max = MAX (max, args[i]);
        cnt++;
        if (args[i] + 1 != args[i + 1]) {

            if (cnt < 2)       out += sprintf (out,"%i",min);
            else if (cnt == 2) out += sprintf (out,"%i,%i",min,max);
            else               out += sprintf (out,"%i-%i",min,max);

            if (i + 1 < n)     out += sprintf (out,",");

            min = args[i + 1], cnt = 0;
        }
            //out += sprintf (out,"%i-%i,",min,max);
    }
    //printf ("%i ",args[i]);

    return output;
}
