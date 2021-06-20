#include <iostream>
#include <iomanip>

using namespace std;

uint64_t a007376 (uint64_t n) {

  uint64_t dig = 1, val  = 1, lim  = 9;

  while (n > val * lim) {
      n -= val * lim;
      val++;
      lim *= 10;
      dig *= 10;
  }

  dig += (n - 1) / val;
  string s = to_string (dig);
  return s[(n - 1) % val] - '0';
}
uint64_t prefix (uint64_t n) {
    long long num = n * (n + 1) * 0.5;

    for (uint64_t p = 10; p < n; p *= 10)
        num += (n - (p - 1)) * (n - (p - 2)) / 2;

    return num;
  }
int solve (long long n) {

    uint64_t num = n, low = 0, high = 1e9, mid;
    n--;

    while (low < high){
       mid = (low + high + 1) / 2 ;
       if (prefix (mid) > n)
          high = mid - 1;
       else
          low = mid;
    }

    return a007376 (num - prefix (low));
}

int main () {

  //Test();

  return 0;
}

void Test () {
  /*
    Assert::That(solve(1), Equals(1));
    Assert::That(solve(2),Equals(1));
    Assert::That(solve(3),Equals(2));
    Assert::That(solve(100),Equals(1));
    Assert::That(solve(2100), Equals(2));
    Assert::That(solve(31000),Equals(2));
    Assert::That(solve(55),Equals(1));
    Assert::That(solve(123456),Equals(6));
    Assert::That(solve(123456789),Equals(3));
    Assert::That(solve(999999999999999999), Equals(4));
    Assert::That(solve(1000000000000000000),Equals(1));
    Assert::That(solve(999999999999999993),Equals(7));
    */
}
