typedef unsigned long long ull;
ull perimeter(int n) {

    ull prev = 1, now = 1, next = 1,sum = 0;

    for (int i = 0; i < n + 1; ++i) {
        prev = now;
        now = next;
        next = now + prev;
        sum += prev * 4;

    }

    return sum;
}
