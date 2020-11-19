int sumTriangularNumbers(int n) {

int res = 0;

    for (int i = 0; i <= n; ++i)
        res += i * (i + 1) / 2;

    return (n > 0) ? res : 0;
}
