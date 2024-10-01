#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
char *trench_assault(const char* battlefield) {
    char *result = malloc(12);

    return result;
}

void Tests() {
    // check_solution("psbpww\n" "------", "LLLLLL");
    // check_solution("dzm    qm\n" "---|dq|--", "RRR|RR|RR");
    // check_solution("pbbzmq\n" "------", "LLLLLR");
    // check_solution("     s\n" "mmmm|-", "RRRR|L");
    // check_solution("  zb\n" "m|--", "R|RL");
    // check_solution("sbbb  \n" "----|m", "LLLL|R");
    // check_solution("m     \n" "-|pzzz", "R|LLLR");
    // check_solution("pwss     s"   "\n" "----|qdd|-", "LLLL|LLR|L");
}

int main () {

    // int hash[20] = {[0 ... 19] = 2};
    const char *src = "pwss     s"   "\n" "----|qdd|-";
    const int size = (strchr(src,'\n') - src) + 1;
    char *res = malloc(size * sizeof(char)), *ptr = res;
    int pts, cnt = 0;

    for (int i = 0; i < size; i++) {
        char up = src[i], lo = src[size+i];
        pts = count(up) + count(lo);

        if (i > 0 && src[i-1] == '|' || src[size+i-1] == '|') {
            pts *= 2;
        }
        cnt += pts;

        if (up == '|' || lo == '|') {
            *ptr++ = '|';
        } else if (cnt < 0) {
            *ptr++ = 'L';
        } else if (cnt > 0){
            *ptr++ = 'R';
        }
        // printf("%c %c\n", up, lo);
    }

    *(ptr-1) = '\0';

    printf("%s\n", res);
    // printf("%s %i\n", field, (search - field));



     printf("\nexit\n");
}
