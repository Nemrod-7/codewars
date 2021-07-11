#include <iostream>
#include <iomanip>
#include <vector>
#include <set>

using namespace std;

int sequ (int n) {

    vector<int> seq;

    for (long i = n; i <= 20; ++i) {
        int num = n, mul = 1;

        while (num) {
            int n2 = num % 10;
            if (n2 != 0) mul *= n2;
            num /= 10;
        }
        n += mul;

        seq.push_back(n);
    }

    return 0;
}

int main () {
    int n = 1;
    //sequ (1);
    set<int> seq;

    //seq(6);
    for (long i = n; i <= 20; ++i) {

        //cout <<setw(2) << i << " " << fact(i) << endl;
    }

}
