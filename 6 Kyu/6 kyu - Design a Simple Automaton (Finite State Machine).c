#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool read_commands(const char* commands) {
    char c;
    int state = 1;

    while (c = *commands++)
        switch (state) {
            case 1 : if (c == '1') state = 2; break;
            case 2 : if (c == '0') state = 3; break;
            case 3 : state = 2; break;
        }

    return state == 2 ? true : false;
}
