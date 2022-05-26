#include <stdint.h>
#include <string.h>

uint64_t base64_to_base10 (const char *input) {
    const char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint64_t num = 0;

    for (int i = 0; input[i]; i++) {
        num = num * 64 + (strchr (base64, input[i]) - base64);
    }

    return num;
}
