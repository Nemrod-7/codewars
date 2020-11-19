typedef unsigned long int ull;

ull fact (int num) {
    return num ? num * fact (num - 1) : 1;
}

double going (int n) {
    double res = 1, fact = 1;
    int k = n + 1;

    while (k-->2)
        res += fact /= k;

    return floor (res * 1000000)/1000000;
}
