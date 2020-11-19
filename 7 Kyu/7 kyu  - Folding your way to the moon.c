#include <inttypes.h>

int64_t foldTo (double distance) {

  float thick = 0.0001;
  int64_t cnt = 0;
  //printf ("%lf",distance);

  if (distance < 0) {
    cnt = -1;
  } else {
   while (distance > thick) {
      thick = thick * 2;
      cnt++;
    }
  }
return cnt;
}
