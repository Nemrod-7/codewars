unsigned __int128 factorial(unsigned n) {
    unsigned __int128 prod = 1;
    for (int i = 1; i <= n; ++i)
        prod *= i;
    return prod;
}
