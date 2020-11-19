#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct _geom {
  double x;
  double y;
} Rect;

long long rectangle_rotation(int height, int width) {

    Rect half = {(height / sqrt (2)) * 0.5, (width / sqrt (2)) * 0.5};
    Rect fst = {floor (half.x) * 2 + 1, floor (half.y) * 2 + 1};
    Rect sec;

    sec.x = ((half.x - floor (half.x)) < 0.5) ? fst.x - 1 : fst.x + 1;
    sec.y = ((half.y - floor (half.y)) < 0.5) ? fst.y - 1 : fst.y + 1;

    return sec.x * sec.y + fst.x * fst.y;
}
