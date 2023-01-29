#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ull;

ull factorial (ull num) { return num ? num * factorial (num - 1) : 1; }
char *reverse (char *str) {
		if (!str || strlen(str) < 2) return str;

		int end = strlen (str) - 1, start = 0;

		while (start < end) {
				str[start] ^= str[end];
				str[end] ^= str[start];
				str[start++]^= str[end--];
		}

		return str;
}
char *dec2FactString (ull num) {

		int fact = 1;
		char *res = malloc (32 * sizeof(char)), *ptr = res;

		while (num) {
				ptr += sprintf (ptr, "%i", num % fact);
				num /= fact++;
		}
		reverse(res);
		return res;
}

ull factString2Dec (const char *fact) {

		const size_t size = strlen (fact);
		int dig,  num = 0;

		for (int i = 1; i < size; i++) {
				dig = fact[(size - 1) - i] - '0';
				num += dig * factorial(i);
		}

		return num;
}
void dotest (ull dec, const char *fact) {
		ull res = factString2Dec (fact);

}
int main () {

		/*
		do_test(2982, "4041000");
		do_test(463, "341010");
		do_test(36288000, "A0000000000");
		do_test(3628800054, "76A0000021000");
		do_test(1273928000, "27A0533231100");
		do_test(34303, "65341010");
		do_test(555555, "1461330110");
		do_test(7890123456, "13573044440000");
		do_test(371993326789901217ull, "311E55B5544150121110");
		*/

		const char *fact = "341010";
		const size_t size = strlen (fact);
		int dig,  num = 0;

		for (int i = 1; i < size; i++) {
				dig = fact[(size - 1) - i] - '0';
				num += dig * factorial(i);
		}
		printf ("=> %i", num);

		printf ("end\n");
		//463 = 3×5! + 4×4! + 1×3! + 0×2! + 1×1! + 0×0!
}
