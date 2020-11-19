#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1
#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef int test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression) {
              std::cout<<"actual : "<<actual<<" expected : "<<expression;

              std::cout<<std::endl;

            }

        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

uint64_t factorial (unsigned n) {

    uint64_t mult = 1;
    for (unsigned i = 1; i <= n ; ++i)
        mult *= i;

    return mult;
}
uint64_t binomial (unsigned n, unsigned k) {
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;
    //return binomial(n - 1, k - 1) + binomial(n - 1, k);
    //return factorial(n) / (factorial (k) * factorial (n - k));
    uint64_t res = 1;

    k = min (k, n - k);
    for (unsigned i = 1; i <= k; i++)
        res *= (n + 1 - i) / (double)i;

    return res;
}
double bernoulli (unsigned n) {
    if (n == 0) return 1;
    if (n == 1) return 0.5;
    if (n % 2) return 0;
    //return -riemann_zeta (1 - n) * n;
    unsigned k = 0;
    double sum = 0;

    for (; k <= n - 1; k++)
        sum += binomial (n, k) * bernoulli (k) / (n - k + 1);

    return 1 - sum;

}

double zeta (int k) {
    k = abs (k);
    return pow (-1, k) * bernoulli (k + 1) / (k + 1);
}

double series(int k, int nb) {
    double out = 0;

    if (k % 2 == true && k > 2) {
        for (int i = 1; i < nb; ++i)
            out += 1 / pow (i, k);
    }
    else if (k % 2 == false && k >= 2) {
        double bern = -riemann_zeta (1 - k) * k;
        //out = 0.5 * abs (ber) * pow (2 * pi, k) / factorial (k);
        out = 0.5 * abs (bern) * pow (2 * M_PI, k) / factorial (k);
    }
    else {
        out = riemann_zeta (k) ;
        //k = abs (k);
        //out = pow (-1, k) * bernoulli (k + 1) / (k + 1);
    }

    return out;
}

int main () {

    auto start = std::chrono::steady_clock::now();

    Test ();

    //std::cout <<fixed << -riemann_zeta (-27)<< endl;


    auto end = std::chrono::steady_clock::now();
    //std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took "
              << std::chrono::duration<double, milli>(end - start).count()
              << " ms" << std::endl;
}

void assertFuzzy(int n, int nb, double expect) {
    double merr = 1e-6;
    double actual = series(n, nb);
    bool inrange = std::abs (actual - expect) <= merr;
    if (inrange == false) {
        std::cout << "Testing: " << n << ", " << nb << std::endl;
        std::cout <<fixed<< "Expected: " << expect << ", and got " << actual << std::endl;
    }
    Assert::That(inrange, Equals(true));
    std::cout << endl;

}
void Test() {
    assertFuzzy(2, 0, 1.64493406684822);
    assertFuzzy(3, 100000, 1.20205690310973);
    assertFuzzy(4, 0, 1.08232323371113);
    assertFuzzy(-5, 0, -0.003968253968);
    assertFuzzy(-11, 0, 0.0210927960927961);
    assertFuzzy(-9, 0, -0.007575757575757576);
    /*
    */
    assertFuzzy(-27, 0, 974937);
    //assertFuzzy(-29, 0, 20052695);
}
