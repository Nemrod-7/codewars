#include <stdlib.h>
#include <stdbool.h>

#define M_PI 3.14159265358979323846
#define ABS(x) (((x) < 0) ? -(x) : (x))

char* iterPi(double epsilon) {

    double num = 4, denum = 3, pi = 4;
    short int sign = 0;
    unsigned long int index = 1;
    bool superior = true;

    char *result = malloc (64 * sizeof (char));

    while (superior) {

        sign = (index % 2) ? (-1) : (1);
        pi += sign * (num / denum);
        denum += 2;
        index++;

        if (ABS (M_PI - pi) < epsilon)
          superior = false;

    }
    //printf ("epsilon %.10f\n",epsilon);
    sprintf (result,"[%i, %.10f]",index,pi);
    //printf ("epsilon %.9f\n",(1 - epsilon) * M_PI);
    return result;
}
