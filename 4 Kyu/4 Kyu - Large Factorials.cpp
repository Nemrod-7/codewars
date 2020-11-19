#include <iostream>
#include <string>

using namespace std;

#define MAX_DIGIT 1024

string factorial(int n) {

    if (n < 0) return "";

    size_t x, i, tmp;
    size_t carry = 0, ndigits = 1, buffer[MAX_DIGIT] = {1};
    string  os;

    for (x = 2; x <= n; x++) {

        for (i = 0; i < ndigits; ++i) {
            tmp = buffer[i] * x;
            buffer[i] = (tmp + carry) % 10;
            carry = (tmp + carry) / 10;

            if (i == ndigits - 1 && carry > 0)
                  ndigits++;
        }
    }

    while (ndigits-->0)
          os += buffer[ndigits] + '0';

    return os;
}

int main () {

    cout << factorial (10);

}
