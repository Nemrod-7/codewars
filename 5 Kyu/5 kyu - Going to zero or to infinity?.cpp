#include <cmath>

class Suite {
    public:
      static double going(int n);
};


unsigned long long fact (int num) { return num ? num * fact (num - 1) : 1; }
double Suite::going(int n) {
    double res = 1, fact = 1;
    int k = n + 1;

    while (k-->2)
        res += fact /= k;

    return floor (res * 1000000)/1000000;
}
