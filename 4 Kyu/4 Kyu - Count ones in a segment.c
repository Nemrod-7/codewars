long long countOnes ( int left, int right ) {

  long long int countBits (long long int n);
	return countBits (right) - countBits (left - 1);
}

long long int countBits (long long int n) {

    if (n <= 0) return 0;
    if (n == 1) return 1;

    const unsigned int x = floor(log(n)/log(2));
    long long int zx = pow (2,x), fst = (x * pow (2,x-1));

    return fst + countBits (n - zx) + (n - zx + 1);
}
