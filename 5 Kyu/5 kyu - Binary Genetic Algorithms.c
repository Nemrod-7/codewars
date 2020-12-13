#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/***************************************************************
* Feel free to change the function signatures (I did) *
* Only the "run" functions are tested                         *
***************************************************************/

typedef double fitness_t (const char *, ...);
extern fitness_t fitness;

char* generate (size_t length) {
    char *out = malloc (length * sizeof (char));

    return out;
}

void select2 (int size, char* population[size], double fitnesses[size]) {
}

char* mutate (size_t n, char* s, double p) {
    char *out = malloc (n * sizeof (char));

    return out;
}

void crossover (size_t n, char* s1, char* s2) {

}

char* runN (fitness_t f, int length, double p_c, double p_m, int iterations) {
    char *out = malloc (length * sizeof (char));

    return out;
}

char* run (fitness_t f, int length, double p_c, double p_m) {
    char *out = malloc (length * sizeof (char));

    return out;
}

int main () {

    return 0;
}
