#include <stdlib.h>
#include <stdbool.h>

#define MASK ((x + y) % 2)

int getbyte (bool *bits, int pos, int size) {
  int byte = 0, end = pos + size;

  for (int i = pos; i < end; i++) {
    byte = byte * 2 + bits[i];
  }

  return byte;
}
bool is_unrow (int x, int y) {
  if (x == 6 || y == 6) return false;
  if (x > 12 && y < 9) return false;
  if (x < 8 && (y < 9 || y > 12)) return false;

  return true;
}
bool *decode (int qrc[21][21]) {

  bool *output = malloc (1024 * sizeof(bool)), *out = output;

  int startx = 21, endx = startx - 1;
  int i, x = startx, y;
  bool up = true, bit;

  while (x > 0) {
    for (i = 0; i < 21; i++) {
      y = up == true ? 20 - i : i;

      for (x = startx; x-->= endx;) {

        if (is_unrow (x, y)) {
          bit = qrc[y][x];
          if (MASK == 0) bit ^= 1;
          *out++ = bit;
        }
      }
    }

    up ^= 1;
    startx = endx - 1, endx = startx - 1;
  }

  return output;
}
const char *scanner(int qrcode[21][21]) {

    bool *bits = decode (qrcode);
    int i = 12, nxt = 0;
    const int size = getbyte (bits, 4, 8), end = size * 8 + i;
    char *out = malloc (20 * sizeof (char));

    for (; i < end; i += 8)
        out[nxt++] = getbyte (bits, i, 8);

    out[nxt] = '\0';
    //printf ("%s\n", out);
    return out;
}

int main () {


}
