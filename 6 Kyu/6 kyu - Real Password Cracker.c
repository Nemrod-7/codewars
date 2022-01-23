#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/crypto.h>
#include <openssl/sha.h>

#include <time.h>

extern char *strdup (const char *);

void showpass (char pass[], int sz) {
    for (int i = 0; i < sz; i++) {
        printf ("%c", pass[i]);
    }
    printf ("\n");
}

const char dict[26] = "abcdefghijklmnopqrstuvwxyz";

uint8_t *search (const int size, const uint8_t *salt) {

    int index = 0;
    const int end = (sizeof (dict) / sizeof (dict[0])) - 1;
    uint8_t hash[SHA_DIGEST_LENGTH], *pass = malloc (size * sizeof(char)) ;

    for (int i = 0; i < size; i++) {
        pass[i] = dict[0];
    }

    while (1) {
        SHA1 (pass, size, hash);

        if (!memcmp (hash, salt, SHA_DIGEST_LENGTH))
            return pass;

        pass[0]++;

        while (pass[index] == dict[end] + 1) {
            if (index == size - 1)
                return NULL;

            pass[index++] = dict[0];
            pass[index]++;
        }
        index = 0;
    }

    return NULL;
}
char *crack_SHA_1 (char *password, const char *ihash) {

    long pin;
    uint8_t *salt = OPENSSL_hexstr2buf (ihash, &pin);

    for (int i = 1; i < 6; i++) {
        uint8_t *curr = search (i, salt);
        if (curr != NULL) {
            password = (char *) curr;
            return password;
        }

    }

    return NULL;
}

void Test (char *pass, const char *ihash) {

      pass = crack_SHA_1 (pass, ihash);
      printf (" -> [%s]\n", pass);
}

int main () {

    time_t start = clock(), stop;

    char *found = malloc (32 * sizeof (char));
    //char *curr = crack_SHA_1(found, "57a59c780467d60ec9d7c60c00238a5fd1093f2e");
    const int end = (sizeof (dict) / sizeof (dict[0])) - 1;


    Test (found, "82ddcade9791f8860341e7ca4c7514e58adcf06b");
    Test (found, "a94a8fe5ccb19ba61c4c0873d391e987982fbbd3");

    Test (found, "3240aa0fe3ca15051680641a59e8d7b61c286b23");
    Test (found, "4dc73741b9473168444fab7e680b439ba69f41ec");
    Test (found, "e6fb06210fafc02fd7479ddbed2d042cc3a5155e");
    Test (found, "8fcd25a39d2037183044a8897e9a5333d727fded");
    Test (found, "b1c1d8736f20db3fb6c1c66bb1455ed43909f0d8");

    Test (found, "2d7a34c9ef8efa2cfdf4b89175f7edec1cd0ddda");
    /*
    */
    stop = clock();
    printf ("\nProcess took %.06f s", (double)(stop - start) * 2 / CLOCKS_PER_SEC);

    return 0;
}

uint8_t *hextos (const char *hex) {

    uint8_t *buffer = malloc (SHA_DIGEST_LENGTH * sizeof (char));
    int it = 0;

    for (int i = 0; i < SHA_DIGEST_LENGTH * 2; i += 2)
        sscanf (hex + i, "%2hhx", &buffer[it++]);

    buffer[it] = '\0';
    return buffer;
}
_Bool compare (const uint8_t *a, const uint8_t *b) {

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        if (a[i] != b[i])
            return 0;

    return 1;
  }
char *crack_SHA_1_proto (char *password, const char *ihash) {
    printf ("[%s]", ihash);
    uint8_t *salt = hextos (ihash);
    char passwd[6];

    uint8_t hash[SHA_DIGEST_LENGTH];

    for (int i = 0; i < 27; i++) {
        for (int j = 0; j < 27; j++) {
            for (int k = 0; k < 27; k++) {
                for (int l = 0; l < 27; l++) {
                    for (int m = 0; m < 27; m++) {

                        passwd[0] = dict[i], passwd[1] = dict[j], passwd[2] = dict[k];
                        passwd[3] = dict[l], passwd[4] = dict[m];

                        SHA1 ((const unsigned char *) passwd, strlen (passwd), hash);

                        if (compare (hash, salt)) {
                            password = strdup (passwd);
                            return password;
                            /*
                            */
                        }
                    }
                }
            }
        }
    }

    return NULL;
  }
