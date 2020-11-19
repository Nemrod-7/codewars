#include <iostream>
#include <cmath>


class DigPow {
    public:
        static int digPow(int n, int p) {

            int sum = 0;

            for (auto dig : std::to_string (n))
                sum += pow (dig - '0', p++);

            return sum % n == 0 ? sum / n : -1;
        }
};

int main () {

    DigPow::digPow(695,2);

    return 0;
}
