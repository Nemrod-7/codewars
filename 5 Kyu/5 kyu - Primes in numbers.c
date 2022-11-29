#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* factors(int num) {

    char *out = malloc (64 * sizeof(char)), *ptr = out;

    for (int p = 2; p < sqrt (num); p++)
        if (num % p == 0)
            for (int k = 1; k <= sqrt (num); k++) {
                if (num % p != 0) {
                    if (k - 1 > 1 ) ptr += sprintf (ptr, "(%i**%i)",p,k - 1);
                    else ptr += sprintf (ptr, "(%i)",p );
                    break;
                }
                num /= p;
            }

    if (num > 2) ptr += sprintf (ptr, "(%i)",num);

    return out;
} 

int main () {
		
		int n = 50;
		char *res = factors (n); 
		printf ("%d => %s\n", n , res);

}
