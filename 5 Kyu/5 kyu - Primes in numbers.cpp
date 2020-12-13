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
        static std::string factors(int lst) {

            std::ostringstream os;

            for (int p = 2; p < sqrt (lst); p++)
                if (lst % p == 0)
                    for (int k = 1; k <= sqrt (lst); k++) {

                        if (lst % p != 0) {
                            os << format (p, k - 1); break;
                        }
                        lst /= p;
                    }

            if (lst > 2) os << '(' << lst << ')';

            return os.str();
        }
};
