#include <inttypes.h>

void uint32_to_ip(uint32_t ip, char *output) {

      int index = 4;
      char *out = output;

      while (index-->0) {
          int num = ((ip >> (index * 8)) % 256);
          out += sprintf (out,"%i",num);

          if (index != 0)
              out += sprintf (out,".");
      }
}
