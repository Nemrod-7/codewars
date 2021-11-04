#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mcastIp2Mac(const char* ip, char* macStr) {
    int it = 0;
    int Byte[4];
    char *copy = strdup(ip), *pch = strtok (copy," ,.-"), *ptr = macStr;

    while (pch != NULL) {
        Byte[it++] = atoi(pch);
        pch = strtok (NULL, " ,.-");
    }

    Byte[1] &= ~(1 << 7);
    ptr += sprintf (ptr, "01:00:5E");

    for (int i = 1; i < 4; i++)
        ptr += sprintf (ptr, ":%02X", Byte[i]);

}
