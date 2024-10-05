#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void show(const char *src) {
  // printf("size : %i\n", size);
    const int size = (strchr(src,'\n') - src);

    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%c", src[i]);
    }
    printf("]\n[");
    for (int i = 0; i < size; i++) {
        printf("%c", src[i + size + 1]);
    }
    printf("]\n");
}

int count (char ch) {
    switch(ch) {
        case 's': return - 1; break;
        case 'z': return + 1; break;
        case 'b': return - 2; break;
        case 'd': return + 2; break;
        case 'p': return - 3; break;
        case 'q': return + 3; break;
        case 'w': return - 4; break;
        case 'm': return + 4; break;
    }
    return 0;
}
char *trench_assault(const char *src) {
    const int size = (strchr(src,'\n') - src);
    char *res = malloc(size * sizeof(char)), *ptr = res;
    _Bool trench = 0;
    int up = 0, lo = 0;
    int cnt = 0;
    // printf("%s\n\n", src);


    for (int i = 0; i < size; i++) {
        up += count(src[i]);
        lo += count(src[size+i+1]);

        if (i > 0 && src[size+i] == '|') {
            trench ^= 1;

            if (trench == 1) {
                lo += count(src[size+i+1]);
            }
        }

        // if (trench == 1) {
        //     cnt = up + lo;
        // } else {
        //     cnt = up;
        // }

        if (src[size+i+1] == '|') {
            *ptr++ = '|';
        } else if (cnt < 0) {
            *ptr++ = 'L';
        } else if (cnt > 0){
            *ptr++ = 'R';
        }
        // printf("%i %i => %i\n", lo, up, cnt);
        // printf("[%c]", src[size+i+1]);
    }

    *(ptr) = '\0';
    return res;
}
void do_test (const char *source, const char *expected) {
    const char *actual = trench_assault(source);

    if (strcmp(actual, expected) != 0) {
        show(source);
        printf("error: expected [%s] actual [%s]\n", expected, actual);
    }
}
void Tests() {
    do_test("psbpww\n" "------", "LLLLLL");
    do_test("dzm    qm\n" "---|dq|--", "RRR|RR|RR");
    do_test("pbbzmq\n" "------", "LLLLLR");
    do_test("sbbb  \n" "----|m", "LLLL|R");
    do_test("pwss     s\n" "----|qdd|-", "LLLL|LLR|L");

    do_test("     s\n" "mmmm|-", "RRRR|L");
    // do_test("m     \n" "-|pzzz", "R|LLLR");
    // do_test("  zb\n" "m|--", "R|RL");
}

int main () {

    // int hash[20] = {[0 ... 19] = 2};

    do_test("pwss     s\n" "----|qdd|-", "LLLL|LLR|L");
    // do_test("psbpww\n" "------", "LLLLLL");
    // do_test("dzm    qm\n" "---|dq|--", "RRR|RR|RR");
    // do_test("sbbb  \n" "----|m", "LLLL|R");
    // do_test("pbbzmq\n" "------", "LLLLLR");


    // do_test("  zb\n" "m|--", "R|RL");
    // do_test("     s\n" "mmmm|-", "RRRR|L");
    // do_test("m     \n" "-|pzzz", "R|LLLR");

    // Tests();

     printf("\nexit\n");
}
