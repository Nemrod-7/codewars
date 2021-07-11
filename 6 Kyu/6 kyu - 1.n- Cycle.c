int cycle (int n) {
    if (n % 2 == 0 || n % 5 == 0) return -1;

    int cnt = 0, val = 1;

    do {
        val = val * 10 % n;
        cnt++;
    } while (val != 1);

    return cnt;
}
