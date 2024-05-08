#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

bool is_prime (unsigned p) {
    if (p < 2) return false;
    if (0 == p % 2) return 2 == p;
    if (0 == p % 3) return 3 == p;

    unsigned d = 5;

    while (d * d <= p) {
        if (0 == p % d) return false;
        d += 2;
        if (0 == p % d) return false;
        d += 4;
    }
    return true;
}
size_t period(const std::vector<int> &v) { // detect cycle in a generic sequence

    for (size_t i = 1; i < v.size(); i++) {
        size_t j = 0;

        while (j < v.size() - i && v[j] == v[j + i]) ++j;

        if (j + i == v.size()) {
            return i;
        }
    }

    return 0;
}
vector<int> fibonacci (int n) {
    vector<int> seq (n);
    seq[0] = 0, seq[1] = 1;

    for (int i = 2; i < n; i++) {
        seq[i] = seq[i-2] + seq[i-1];
    }

    return seq;
}
int pisano (int m) {

    int a1 = 1, an = 1, a0 = 0;
    int index = 1;

    while (index++) {
        an = (a1 + a0) % m;

        if (index != 2 && an == 1 && a1 == 1 && a0 == 0) {
            break;
        }

        a0 = a1, a1 = an;
    }

    return index - 2;
}


int main () {

    const vector<int> pi = { 1, 3, 8, 6, 20, 24, 16, 12, 24, 60, 10, 24, 28, 48, 40, 24, 36, 24, 18, 60, 16, 30, 48, 24, 100, 84, 72, 48, 14, 120, 30, 48, 40, 36, 80, 24, 76, 18, 56, 60, 40, 48, 88, 30, 120, 48, 32, 24, 112, 300, 72, 84, 108, 72, 20, 48, 72, 42, 58, 120, 60, 30, 48, 96, 140, 120, 136};

    int n = 30;

    int a1 = 1, an = 1, a0 = 0;
    //ofstream ofs ("pisano", ios::app );


    for (int k = 2; k < 9; k++) {

        pisano(k);
        cout << endl;
    }


    //ofs.close();
}
