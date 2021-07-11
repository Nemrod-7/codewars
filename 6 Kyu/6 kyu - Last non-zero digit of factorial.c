int last_digit (long n) {
    const int dig1[5] = {1,1,2,6,4}, dig2[4] = {6,2,4,8};
    return (n < 5) ? dig1[n] : (dig2[(n / 5) % 4] * last_digit (n / 5) * last_digit (n % 5)) % 10;
}
