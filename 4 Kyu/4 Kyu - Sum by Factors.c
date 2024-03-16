#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define NUM *(list + size)
#define BUFFSIZE 1024
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

unsigned *SieveOfEratosthenes (unsigned num) {
    unsigned hal = (num >> 1) + 1;
    bool sieve[hal];
    uint64_t sv2[num >> 6];

    unsigned *prime = malloc (num * sizeof (int));
    register int p, i, size = 2;

    prime[1] = 2;
    memset (sieve, false, hal);

    for (p = 3; p * p <= num; p += 2) {
        if (sieve[p >> 1] == false) {
            for (i = p * p; i <= num; i += 2 * p) {
                sieve[i >> 1] = true;

                // sv2[i >> 6] |= 1ULL << (i &63);
            }
        }
    }

    for (i = 3; i <= num; i += 2) {
         if (sieve[i >> 1] == false) {
            prime[size++] = i;
         }
    }

    prime[0] = size;
    return prime;
}
unsigned maxnum (const int* list,int size) {

    unsigned max = 0;
    while (size-->0)
        max = MAX (max, abs (NUM));

    return max;
}
char *sumOfDivided (const int *list, int l) {

    int sum = 0, max = maxnum (list,l) + 1;
    char *output = malloc (BUFFSIZE * sizeof(char)), *out = output;
    unsigned *primes = SieveOfEratosthenes (max);
    bool divided = false;
    *output = '\0';

    for (int i = 1; i < primes[0]; ++i) {
        for (int size = 0; size < l; ++size) {
            if (abs (NUM) % primes[i] == 0) {
                sum += NUM, divided = true;
            }
        }

        if (divided == true) {
            out += sprintf (out,"(%i %i)", primes[i], sum);
        }

        sum = 0, divided = false;
    }
    return output;
}

static void printArray (size_t length, const int array[length]) {
    printf("{");
    for (size_t i = 0; i < length; i++)
        printf("%d%s", array[i], (i == (length - 1)) ? "" : ", ");
    printf("}");
}
static void dotest(size_t count, const int numbers[count], const char *expected) {
    char *actual = sumOfDivided(numbers, count);

    if (strcmp(actual, expected)) {

        printf("%s %s\n", actual, expected);
    }

    free(actual);
}
#define fixed_test(numbers, expected) dotest(ARR_LEN(numbers), numbers, expected)
#define ARR_LEN(array) (sizeof(array) / sizeof *(array))

void test () {
  fixed_test(((int[]){12, 15}), "(2 12)(3 27)(5 15)");
  fixed_test(((int[]){15,21,24,30,45}), "(2 54)(3 135)(5 90)(7 21)");
  fixed_test(((int[])
      {107, 158, 204, 100, 118, 123, 126, 110, 116, 100}),
      "(2 1032)(3 453)(5 310)(7 126)(11 110)(17 204)(29 116)(41 123)(59 118)(79 158)(107 107)"
  );
  fixed_test(((int[])
      {-29804, -4209, -28265, -72769, -31744}),
      "(2 -61548)(3 -4209)(5 -28265)(23 -4209)(31 -31744)(53 -72769)(61 -4209)(1373 -72769)(5653 -28265)(7451 -29804)"
  );
  fixed_test(((int[]){17, -17, 51, -51}), "(3 0)(17 0)");
  fixed_test(((int[]){173471}), "(41 173471)(4231 173471)");

// dotest(0, NULL, "");

}

int main () {

    test();

    printf("\nend\n");
}
