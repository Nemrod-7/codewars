#include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>

char *passwd_hash(const char *passwd) {

  unsigned char buffer[16];
  char *hash = malloc (64 * sizeof (char)), *tmp;

  tmp = hash;

  MD5 (passwd,strlen (passwd),buffer);

  for (int i = 0; i < 16; i++) {
   tmp += sprintf (tmp, "%02x", buffer[i]);
  }

  return hash;
}
