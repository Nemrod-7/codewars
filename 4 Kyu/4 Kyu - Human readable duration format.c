#include <stdlib.h>
#include <stdbool.h>
char *formatDuration (int duration) {
    //printf ("%i\n",duration);
   unsigned long nseconds[5] = {31536000,86400,3600,60,1};
    char time[5][7] = {"year","day","hour","minute","second"};
    int i, period[5] = {0}, count = 0;

    char *result = malloc (64 * sizeof(char)), *tmp = result;

    *result = '\0';

    for (i = 0; i < 5; ++i) {
        if (duration >= nseconds[i])
            period[i] = duration / nseconds[i];

      if (period[i] != 0)
          count++;

      duration -= (period[i] * nseconds[i]);
    }

    for (i = 0; i < 5; ++i) {

        if (period[i] != NULL) {
            count--;
            tmp += sprintf (tmp,"%i %s", period[i],time[i]);

            if (period[i] > 1)
                tmp += sprintf (tmp,"s");

            if (count > 1)
                tmp += sprintf (tmp,", ");

            if (count == 1)
                tmp += sprintf (tmp," and ");
        }
    }

    if (result[0] == NULL)
        tmp += sprintf (tmp,"now");

    return result;
}
