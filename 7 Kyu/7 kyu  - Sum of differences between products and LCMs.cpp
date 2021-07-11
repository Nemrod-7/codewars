#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

uint64_t sum_differences_between_products_and_LCMs (const vector<pair<uint64_t, uint64_t>>& pairs) {
  uint64_t sum = 0;

  for (auto &[a,b] : pairs)
      sum += (a * b - lcm (a, b));

  return sum;
}

int main () {

    pair<int,int> p {2,5};

    auto [a,b] = p;

    cout << a;
}
