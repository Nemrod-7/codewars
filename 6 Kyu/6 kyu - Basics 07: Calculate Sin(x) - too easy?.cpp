#include <iostream>

#include <iomanip>
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



int main () {

    auto start = std::chrono::steady_clock::now();

    Test ();
    //cout << M_PI;
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
}

uint64_t factorial (int n) {
    return n ? n * factorial (n - 1) : 1;
}
double power (double base, int exp) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) return power (base, exp / 2) * power (base, exp / 2);
    return base * power (base, exp / 2) * power (base, exp / 2);;
}
double sin(double deg) {
    const double pi = 3.14159265358979323846, radian = deg * pi / 180, epsilon = 1e-8;
    double sine = 0;
    int inf = 10;
    //sin(x) = sum from k = 0 to infinity {(-1)^k . { x^{2k + 1}} over {fact (2k + 1)}  }
    for (int k = 0; k < inf; ++k) {
        int y = 2 * k + 1;
        sine += power (-1, k) * power (radian, y) / factorial(y);
        //cout << fixed<<setprecision(8)<< pow (radian, y) << endl;
    }

    return sine;
}

void assertEqual(double actual,double expected) {

    if (actual != expected) {
        cout <<fixed<<setprecision(20)<< "expected : " << expected;
        cout <<fixed<<setprecision(20)<< " got : " << actual << endl;
    }
}
void Test() {
  {
          const double expected = 0.86602540378443864676372317075294;
          double actual = sin(60);

          assertEqual(actual, expected);
  }

  {
          const double expected = 0.70710678118654752440084436210485;
          double actual = sin(45);

          assertEqual(actual, expected);
      }
   {
          const double expected = -0.98480775301220805936674302458952;
          double actual = sin(-80);

          assertEqual(actual, expected);
      }

   {
          const double expected = 0.77217937246662690089870067353431;
          double actual = sin(50.55);

          assertEqual(actual, expected);
      }

}
