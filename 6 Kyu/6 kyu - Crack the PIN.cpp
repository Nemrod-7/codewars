#include <iostream>
#include <cstring>
#include <iomanip>

#include <openssl/crypto.h>
#include <openssl/md5.h>

// -lssl -lcrypto
using namespace std;

std::string md5 (const string &passwd) {

      uint8_t hash[MD5_DIGEST_LENGTH];
      ostringstream os;

      MD5 ((uint8_t *)passwd.c_str(), passwd.size(), hash);

      for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
          os << hex << static_cast<unsigned> (hash[i]);

      return os.str();
}
std::string crack(const std::string& hash) {
  long pin;
  uint8_t *hex = OPENSSL_hexstr2buf (hash.c_str(), &pin);
  uint8_t buffer[MD5_DIGEST_LENGTH];
  pin = 100000;

  while (pin-->0) {
      ostringstream os;
      os << setfill('0') << setw (5) << pin;

      MD5 ((unsigned char *)os.str().c_str(), 5, buffer);

      if (memcmp (buffer, hex, 16) == 0)
          return os.str();
  }

  return "";
}

int main () {

     cout << crack("827ccb0eea8a706c4c34a16891f84e7b") << endl;
     cout << crack("86aa400b65433b608a9db30070ec60cd") << endl;

     cout << md5 ("00078");


     //std::cout << os.str();
}
////////////////////////////////////arkive////////////////////////////////////
/*
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
    strcpy (expected, passwd);
  }
*/
