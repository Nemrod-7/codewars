#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <openssl/crypto.h>
#include <openssl/md5.h>


char *passwd_hash (const char *passwd) {

  uint8_t buffer[MD5_DIGEST_LENGTH];
  char *hash = malloc (33 * sizeof (char)), *ptr = hash;

  MD5 (passwd, strlen (passwd), buffer);
  //printf ("%s ",buffer);
  for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
      ptr += sprintf (ptr, "%02x", buffer[i]);

  return hash;
}

void crack (char *hash, char *expected) {
    int pin;
    uint8_t *hex = OPENSSL_hexstr2buf (hash, &pin);
    uint8_t buffer[MD5_DIGEST_LENGTH];
    uint8_t *passwd = malloc (5 * sizeof (char));

    pin = 0;

    while (memcmp (hex, buffer, 16)) {
        sprintf (passwd, "%05i", pin++);
        MD5 (passwd, 5, buffer);
    }
    strcpy (expected, passwd);
}


int main () {

    char expected[6] = {0};
     crack("827ccb0eea8a706c4c34a16891f84e7b", expected);
     printf ("pin : %s\n", expected);
     crack("86aa400b65433b608a9db30070ec60cd", expected);
     printf ("pin : %s\n", expected);
     /*
*/


}
////////////////////////////////////arkive////////////////////////////////////

_Bool compare (const char *a, const char *b) {

  for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
      if (a[i] != b[i])
          return 0;

  return 1;
}
char *pad (char *src, int len) {
    const int lim = 5;
    const char c = src[len];
    for (int i = len; i < lim; i++)
        src[i] = c;
}
char *hextos (char *hex) {

        char *buffer = malloc (MD5_DIGEST_LENGTH * sizeof (char));
        int idx = 0;

        for (int i = 0; i < 32; i += 2)
        sscanf (hex + i, "%2x", &buffer[idx++]);

        buffer[idx] = '\0';
        return buffer;
      }
