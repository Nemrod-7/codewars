int* seven(long long m) {

    long long x = 0, y = 0;
    int *res = malloc (2 * sizeof(int)), count = 0;

    while (m > 99) {
        y = (m % 10);
        x = m / 10 - 2 * y;
        m = x;
        count++;
    }

    res[0] = m;
    res[1] = count;

    return res;
}
