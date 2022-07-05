#include <stdio.h>
#include <stdlib.h>


long long gcd (long long a, long long b) { return a==0 ? b : gcd (b % a , a); }
_Bool search (int *seq, int size, int val) {

    for (int i = 0; i < size; i++) {
        if (seq[i] == val) return 1;
    }
    return 0;
}
int *g (long long n) {

    static long long k = 2;
    static int seq[300000] = {1,7};

    for (; k < 2 * n; k++) {
        seq[k] = seq[k - 1] + gcd (k, seq[k - 1]);
        seq[k - 1] = seq[k] - seq[k - 1];
    }

    return seq;
}
_Bool isunique (int *seq, int size, int val) {

    for (int i = 0; i < size; i++) {
        if (seq[i] == val) {
            return 0;
        }
    }

    return 1;
}

long long countOnes (long long n) {

    int *seq = g (n + 1);
    long long sum = 0;

    for (int i = 0; i < n; i++) {
        if (seq[i] == 1) sum++;
    }

    return sum;
}
long long maxPn (long long n) {

    int s1 = n, *seq = g (s1);
    int s2 = 0, seq2[30000];
    int i = 0, maxv = 0;

    while (s2 != n) {
        int val = seq[i];

        if (val != 1) {
            //printf ("%d ", val);
            if (val >= maxv) {
                maxv = val;
            }

            if (isunique (seq2, s2, val)) {
                seq2[s2++] = val;
            }
        }

        i++;
    }

    return maxv;
}
long long anOverAverage (long long n) { return 3; }

void dotest1(long long n, long long expr) {
    long long act = countOnes(n);
    if(act != expr)
        printf("Expected %lld, got %lld\n", expr, act);
}
void dotest2(long long n, long long expr) {
    long long act = maxPn(n);
    if(act != expr)
        printf("Expected %lld, got %lld\n", expr, act);
}
void dotest3(long long n, int expr) {
    int act = anOverAverage(n);
    if(act != expr)
        printf("Expected %d, got %d\n", expr, act);
}

void Test() {
    printf("countOnes\n");

    dotest1(1, 1);
    dotest1(10, 8);
    dotest1(100, 90);

    printf("maxPn\n");
    dotest2(1, 5);
    dotest2(5, 47);
    dotest2(7, 101);

    printf("anOverAverage\n");
    dotest3(5, 3);
}
int main () {

    Test();

}
