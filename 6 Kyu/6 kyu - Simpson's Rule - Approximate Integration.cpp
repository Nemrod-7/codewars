#include <iostream>
#include <iomanip>
#include <cmath>
class SimpsonIntegration {
    public:
        static double f (double x) { return 1.5 * pow (sin (x), 3); }
        static double simpson (int n) {

            const double h = M_PI / n;
            double sum = 0;

            for (int i = 1; i <= n; i++)
                sum += (i % 2 == 1) ? 4 * f(i * h) : 2 * f(i * h);

            return M_PI / (3 * n) * (f(M_PI) + sum);
        }
};

int main () {


    return 0;
}

void assertFuzzyEquals(double act, double exp) {
  bool inrange = std::abs(act - exp) <= 1e-10;
  if (inrange == false) {
    std::cout << std::setprecision(10);
    std::cout << "At 1e-10: Expected should be " << exp << ", but got " << act << std::endl;
  }
  //Assert::That(true, Equals(inrange));
}
void dotest(int n, double expected) {
  assertFuzzyEquals(SimpsonIntegration::simpson(n), expected);
}
void Test () {
  dotest(290, 1.9999999986);
  dotest(72, 1.9999996367);
  dotest(252, 1.9999999975);
  dotest(40, 1.9999961668);
}
