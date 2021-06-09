#include <iostream>
#include <cmath>

#include <iomanip>

using namespace std;
/*
ex : 8

1 23 456 789 10
1 12 123 1234


*/
uint64_t prefix (uint64_t n) {
    uint64_t num = n * (n + 1) * 0.5;
    if (n >= 10) num += (n - 9) * (n - 8) * 0.5;
    if (n >= 100) num += (n - 99) * (n - 98) * 0.5;
    return num;
}
uint64_t gen (uint64_t n) { // generate smarandache concanated sequence
    const uint64_t n2 = sqrt (8 * n - 7);
    return n - 0.5 * floor ((n2 + 1) * 0.5) * floor ((n2 - 1) * 0.5);
}
int a007376 (int n) {

    uint64_t dig = 1, val  = 1, lim  = 9;

    while (n > val * lim) {
        n -= val * lim;
        val++;
        lim = lim * 10;
        dig = dig * 10;
    }

    dig += (n - 1) / val;
    string s = to_string (dig);
    return s[(n - 1) % val] - '0';
}

int main () {
    int index = 0, n = 8, num = 8, pre = 0;

    while (pre - num < 0) {
        num -= pre;
        pre = prefix (index++);
        //index++;
    }
    cout << n - pre;
    /*
    for (int i = 1; i < 10; i++) {
        cout << setw(2) << prefix (i) << " : ";
        for (int j = 1; j < i; j++) {
            cout << j;
        }
        cout << endl;
    }
    */
}
