#include <stdio.h>
#include <stdlib.h>

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

int main () {

		int n = 8;

		for (int j = 2; j < n; j++) {
				for (int i = 1; i < j; i++) {
						if (gcd (i,j) == 1) {

						} else {
							printf ("%i/%i ", i, j);

						}
				}
		}

}
