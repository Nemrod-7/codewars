#include <vector>
#include <algorithm>

using namespace std;

int lcm (int a, int b) { return a * b / __gcd(a, b); }

int sum_differences_between_products_and_LCMs(const vector<vector<int>> &pairs) {
    int sum = 0, prod ;
    for (auto it : pairs) {
        prod = it[0] * it[1];
        sum += (prod - lcm (it[0], it[1]));
    }
    return sum;
}
