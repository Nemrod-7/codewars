#include <stdio.h>

typedef struct { int r, g, b;} rgb;

rgb hex_str_to_rgb (const char *hex) {

    rgb pix;
    sscanf (hex, "#%02x%02x%02x", &pix.r, &pix.g, &pix.b);
    return pix;
}

int main () {


}
