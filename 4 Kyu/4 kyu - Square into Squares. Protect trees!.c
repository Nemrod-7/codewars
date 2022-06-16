
char* decompose(long long n) {
long long buff[32], n2 = 0, num;
    char *os = malloc (32 * sizeof (char)), *out = os;
    int size = 0;
    buff[size++] = n;

    do {
        num = buff[--size];
        n2 += num * num;

        while (num-->1) {
            if (num * num <= n2) {
                buff[size++] = num;
                n2 -= num * num;
            }
        }

    } while (n2 != 0 && size != 0);

    out += sprintf (out, "[");
    while (size--> 0) {
        out += sprintf (out, "%li", buff[size]);
        if (size > 0)
            out += sprintf (out, ",", buff[size]);
    }
    out += sprintf (out, "]");

    return os;
}
