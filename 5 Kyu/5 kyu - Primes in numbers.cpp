#include <iostream>
#include <cmath>
#include <sstream>

class PrimeDecomp {
    private :
        static std::string format (int p, int val) {
            std::ostringstream os;
            os << '(' << p;
            if (val > 1) os << "**" << val;

            return os.str() + ')';
        }
    public :
        static std::string factors(int num) {

            std::ostringstream os;

            for (int p = 2; p * p <= num; p++)
                if (num % p == 0)
                    for (int k = 1; k * k <= num; k++) {

                        if (num % p != 0) {
                            os << format (p, k - 1); break;
                        }
                        num /= p;
                    }

            if (num > 2) os << '(' << num << ')';

            return os.str();
        }
};

int main () {

}
