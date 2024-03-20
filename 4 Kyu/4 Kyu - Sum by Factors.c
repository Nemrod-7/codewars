#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define NUM *(list + size)
#define BUFFSIZE 1024
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

unsigned *SieveOfEratosthenes (uint64_t limit) { // SoE whith advanced wheel factorization and bitmask => ex limit == 1e8 : memory usage 3,97 MB / execution time ~0.60 ms
    const uint64_t wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    uint64_t size = 5;
    uint64_t *sieve = calloc((hal + 1), sizeof(uint64_t));
    unsigned *prime = malloc (limit * sizeof (unsigned));

    prime[1] = 2, prime[2] = 3, prime[3] = 5, prime[4] = 7;

    for (uint64_t i = 11, t = 0; i <= limit ; i += wheel[t], t = t == 47 ? 0 : t + 1) {
        uint64_t p = 0xAAAAAAABULL * i >> 33;           // fast division by 3
        uint64_t mask = sieve[p >> 6] >> (p &63) &1ULL; // x >> 6 => fast division by 64 / x &63 => fast modulus 64

        if (mask == 0) {
            prime[size++] = i;

            for (uint64_t j = i * i, v = t; j <= limit; j += i * wheel[v], v = v == 47 ? 0 : v + 1) {
                uint64_t p2 = 0xAAAAAAABULL * j >> 33;
                sieve[p2 >> 6] |= 1ULL << (p2 &63);
            }
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

    bool divided = false;
    int sum = 0, max = maxnum (list,l) + 1;
    char *output = malloc (BUFFSIZE * sizeof(char)), *out = output;
    const unsigned *primes = SieveOfEratosthenes (max);
    *output = '\0';

    for (int i = 1; i < primes[0]; i++) {
        for (int size = 0; size < l; size++) {
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
