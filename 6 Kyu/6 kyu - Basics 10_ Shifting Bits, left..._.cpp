
int nextHigher(int num) {
    if (num == 0) return 1;
    if (num == -1) return 0;
    int shift = num > 0 ? 2 : -1, bit = abs(num);

    while (bit >>= 1) shift <<= 1;

    if (shift == num) return shift >>= 1;

    return shift;
}
