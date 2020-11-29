#include <stdio.h>


/***************************************************************
* Feel free to change the function signatures (I did) *
* Only the "run" functions are tested                         *
***************************************************************/

#include <stddef.h>

typedef double fitness_t (const char *, ...);

extern fitness_t fitness;

char* generate (size_t length) {
}

void select (int size, char* population[size], double fitnesses[size]) {
}

char* mutate (size_t n, char* s, double p) {
}

void crossover (size_t n, char* s1, char* s2) {
}

char* runN (fitness_t f, int length, double p_c, double p_m, int iterations) {
  // TODO: Implement the run method
}

char* run (fitness_t f, int length, double p_c, double p_m) {
  // TODO: Implement the run method
}

int main () {

    return 0;
}
