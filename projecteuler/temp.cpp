#include <iostream>

using namespace std;

bool eval (const string &num) {
    char dig = '1'; 

    for (int k = 0; k < 14; k += 2) {
        if (num[k] != dig++) {
            return false;
        }
    }

    return true;
}
int main () {

    int64_t high = 1389103919;
    int64_t low = 1100005720;
    // 1_2_3_4_5_6_7_8_9_0
    for (int64_t i = low; i < high; i += 10) {

        string sq = to_string(i*i);
        char dig = '1';
        if (eval(sq) == true) {
            cout << i << " => ";

            for (int j = 0; j < sq.size(); j++) {
                if (j % 2 == 0) {
                    cout << sq[j];
                } else {
                    cout << "_";
                }
            }
            cout << "\n";
        }
    }
}
