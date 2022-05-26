#include <stdio.h>
#include <inttypes.h>

uint32_t getval (const char *x) {

    uint32_t addr[4], val = 0;
    sscanf (x, "%d.%d.%d.%d", &addr[0], &addr[1], &addr[2], &addr[3]);

    for (int i = 0; i < 4; i++) {
        val <<= 8;
        val |= (uint8_t) addr[i];
    }

    return val;
}
uint32_t ips_between (const char *start, const char *end) {
    return getval (end) - getval (start);
}
