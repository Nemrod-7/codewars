#include <stdbool.h>
#define is0(x) (((x) == 'o') || ((x) == 'O'))
#define isX(x) (((x) == 'x') || ((x) == 'X'))

bool xo (const char *str) {

	int cntx = 0, cnto = 0;

	for (;*str != '\0'; ++str) {
		(isX(*str)) ? cntx++ : 0;
		(is0(*str)) ? cnto++ : 0;
	}
	return (cntx == cnto) ? true : false;
}
