#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void show(const char *src) {
  // printf("size : %i\n", size);
    const int size = (strchr(src,'\n') - src);

    printf("\n[");
    for (int i = 0; i < size; i++) {
        printf("%c", src[i]);
    }
    printf("]\n[");
    for (int i = 0; i < size; i++) {
        printf("%c", src[i + size + 1]);
    }
    printf("]\n");
}
void showpoint(const char *src) {

    int count(char);
    const int size = (strchr(src,'\n') - src);
    printf("\n[");
    for (int i = 0; i < size; i++) {
        printf("%2i ", count(src[i]));
    }
    printf("]\n[");
    for (int i = 0; i < size; i++) {
        printf("%2i ", count(src[size+i+1]));
    }
    printf("]\n\n");
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

    int prv, cnt = 0, upper = 0, lower = 0;
    _Bool trench = 0;

    //showpoint(src);
    for (int i = 0; i < size; i++) {
        int gnd = count(src[i]), sub = count(src[size+i+1]);

        if (trench == 0) {
            lower = 0;
        }

        if (src[size+i+1] == '|') {
            trench ^= 1;
        }

        if (trench == 1 && src[size+i] == '|') {
            sub *= 2;
        }

        if (gnd == 0) {
            lower += upper;
            upper = 0;
        }


        prv = cnt;
        upper += gnd, lower += sub;
        cnt = upper + lower;

        if (cnt == 0) {
            cnt = prv;
        }

        if (src[size+i+1] == '|') {
            *ptr++ = '|';
        } else if (cnt < 0) {
            *ptr++ = 'L';
        } else if (cnt > 0){
            *ptr++ = 'R';
        }
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

    do_test("pwss     s\n" "----|qdd|-", "LLLL|LLR|L");
    do_test("psbpww\n" "------", "LLLLLL");
    do_test("dzm    qm\n" "---|dq|--", "RRR|RR|RR");
    do_test("sbbb  \n" "----|m", "LLLL|R");
    do_test("pbbzmq\n" "------", "LLLLLR");   


    do_test("  zb\n" "m|--", "R|RL");
    do_test("     s\n" "mmmm|-", "RRRR|L");
    do_test("m     \n" "-|pzzz", "R|LLLR");
    do_test("  zb\n" "m|--", "R|RL");
}

int main () {

    // int hash[20] = {[0 ... 19] = 2};

    int a = -5, b = 6;

    Tests();


    printf("\nexit\n");
}
