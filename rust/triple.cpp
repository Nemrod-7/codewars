#include <iostream>

using namespace std;

int gcd (int a, int b) {

    while (b != 0) {
        int tmp = a % b;
        a = b;
        b = tmp % b;
    }

    return a;
}

int gcd1 (int a, int b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

void triplet (int limit) {
    int n = 1, m = 2;
    int a, b, c;

    while ((m * m + 1) < limit) {
        for (n = m % 2 - 1; n < m; n += 2) {
            if (gcd(m,n) == 1) {
                a = m * m - n * n;
                b = 2 * m * n;
                c = m * m + n * n;
                cout << a << " " << b << " " << c << "\n" ;
            }
        }

        m += 1;
    }

}

int main () {

    int limit = 50;

    int n = 1, m = 2;
    int a, b, c;

    triplet(50);

    /*
       while ((m * m + 1) < limit) {
       if (n >= m) n = m % 2, m = m + 1;
       c = m * m + n * n;

       if (c >= limit) {
       n = m;
       } else {
       if (gcd(m,n) == 1) {
       a = m * m - n * n;
       b = 2 * m * n;
       cout << a << " " << b << " " << c << "\n" ;
       }
       }
       n += 2;
       }
       */
}
