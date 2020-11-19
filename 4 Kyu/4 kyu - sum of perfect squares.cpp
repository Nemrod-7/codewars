#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef uint64_t test_t;
class Assert {
  public :
  static void That (test_t actual, test_t expression) {
    if (actual != expression)
    std::cout<<"actual : "<< actual <<" expected : "<< expression <<std::endl;
  }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
bool is_square (uint64_t n) {
    uint64_t root = sqrt (n);
    return (root * root == n) ? true : false;
}
bool is_odd (int n) { return n % 2 == 0;}
int sum_of_squares (int n) {
    if (n <= 0 || is_square (n)) return 1; // case 1 : If answer is 1 => sum is perfect square
    if (n < 4) return n;

    // a positive integer can be expressed as the sum of three squares if and only if it is not of the form 4^k(8m+7) for some int k and m
    while (n % 4 == 0) n >>= 2; // handling 4^k
    if (n % 8 == 7) return 4; //handling 8m+7

    for (int i = 1; i * i <= n; ++i) // If answer is 2 => one factor has to be (int)square_root
        if (is_square (n - i * i))   // and remaining other number has to be a perfect square
            return 2;

    return 3;
}
int main () {

    auto start = std::chrono::high_resolution_clock::now();

    Test ();
    //cout << sum_of_squares(31);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " <<fixed<< elapsed.count()  << " ms" << std::endl;
}


void Test() {
    Assert::That(sum_of_squares(15), Equals(4));
    Assert::That(sum_of_squares(16), Equals(1));
    Assert::That(sum_of_squares(17), Equals(2));
    Assert::That(sum_of_squares(18), Equals(2));
    Assert::That(sum_of_squares(19), Equals(3));
}
