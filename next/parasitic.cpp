#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

string parasitic (int dig, int base) {
    string num = to_string(dig);

    for (int i = 0; i < 23; i++) {
        int ans[54] = {0};
        int size = num.size() + 1;

        reverse(num.begin(), num.end());

        for (int j = 0; j < num.size(); j++) {
            ans[j] += dig * (num[j] - '0');
            ans[j + 1] = ans[j + 1] + ans[j] / base;
            ans[j] %= base;
        }

        num.clear();
        while (ans[size] == 0) size--;
        if (ans[size] < dig) size--;

        for (int j = size; j >= 0; j--) {
            num += ans[j] + '0';
        }

        num += (dig + '0');

        // cout << num << '\n';
        if (ans[size] == 0) {
            // return "1" + num;
        }
    }

    return "1" + num;
}

int main () {

    int mul = 3;
    int base = 10;

    for (int i = 2; i < 10; i++) {
      string res = parasitic(i, 10);

      cout  << i << " :: [" << res << "]\n";
    }



}
