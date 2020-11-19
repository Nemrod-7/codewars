int nextLower(int num) { // -> to do
    if (num == 0 || num == -1) return num - 1;
    int shift = num > 0 ? 1 : -2, bit = abs (num);

    while (bit >>= 1) shift <<= 1;

    return (shift == num) ? shift >>= 1 : shift;
}
