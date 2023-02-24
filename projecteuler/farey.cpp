#include <iostream>

typedef struct { int d, n; } frac;

void farey (int n) {

	frac f1 = {0, 1}, f2 = {1, n}, tmp;
	int k;
	printf("%d/%d ", 1, n);
	while (f2.n > 1) {
			k = (n + f1.n) / f2.n;
			tmp = f1;
			f1 = f2;
			f2 = (frac) { f2.d * k - tmp.d, f2.n * k - tmp.n };
			std::cout << f2.d << "/" << f2.n << " ";
	}

	//std::cout << "\n";
}

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

int main () {
	//1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

	// Problem 216
	for (int i = 1; i < 1e4; i++) {
			int fn = 2 * (i * i) - 1;
			std::cout << fn  <<  " ";
	}
	/*
		for (int i = 1; i < 8; i++) {
				farey (i);
				std::cout << "\n";
		}

		*/

		std::cout << "e";
}
