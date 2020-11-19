#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <chrono>

using namespace std;
using ull = unsigned long long;

/////////////////////////////////Assert/////////////////////////////////////////
typedef int test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

ull exp_sum (unsigned int num) {
  if (num == 0) return 1;
  unsigned int n, k;
  ull p[500][500] = {0};

  for (n = 1; n <= num; n++)
      for (k = n; k > 0; k--) {
          if (k > n) p[k][n] = 0;
          else if (k == n) p[k][n] = 1;
          else p[k][n] = p[k][n - k] + p[k + 1][n];
      }

    return p[1][num];
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

}

void Test() {
    Assert::That(exp_sum(2), Equals(2));
    Assert::That(exp_sum(3), Equals(3));
    Assert::That(exp_sum(4), Equals(5));
    Assert::That(exp_sum(1), Equals(1));
    Assert::That(exp_sum(5), Equals(7));
    Assert::That(exp_sum(10), Equals(42));
}
