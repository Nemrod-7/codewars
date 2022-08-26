#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LEN(array) (sizeof(array) / sizeof *(array))
#define N 128

extern char* strdup (const char *);

void simple_assembler (size_t n, const char *const prog[n], int reg[]) {

    size_t index = 0;
    char *op, *line, *pam;
    /*
    static int cycle;
    cycle++;
    for (size_t i = 0; i < n; i++) {
        printf ("%s\n", prog[i]);
    }
    printf ("\n");

    if (cycle >= 2) return;
    */
    while (index < n) {

        line = strdup (prog[index]);
        op = strtok (line, " ");
        pam = strtok (NULL, " ");

        int key = pam[0];
        int cst = !isalpha (key) ? atoi (pam) : reg[key];

        if (strcmp (op, "mov") == 0) {
            const char *val = strtok (NULL, " ");
            reg[key] = isalpha (val[0]) ? reg[val[0]] : atoi (val);
        }
        if (strcmp (op, "inc") == 0) reg[key]++;
        if (strcmp (op, "dec") == 0) reg[key]--;
        if (strcmp (op, "jnz") == 0 && cst) {
            const char *val = strtok (NULL, " ");
            index += isalpha (val[0]) ? reg[key] : atoi (val);
        } else {
            index++;
        }
    }
}

int main () {

    const char *prog[] = {"mov f -63","mov g -8","mov h -72","mov i 55","mov f 2","jnz f 2","jnz f f","jnz 1 -1","inc h","inc g","dec i","inc f","inc i","mov i 2","jnz i 2","jnz i i","jnz 1 -1","mov i h","mov g -58","inc f","dec g","inc g","dec i","mov f g","mov i 2","jnz i 2","jnz i i","jnz 1 -1","dec f","dec i"};
    int reg[N] = {0};

    simple_assembler (ARR_LEN (prog), prog, reg);
    //printf("%i\n", ARR_LEN (prog));
    printf("\nend\n");
}
