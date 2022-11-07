#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> factorize (int n) {

    vector<int> factors;

    for (int k = 2; k < n; k++)
        if (n % k == 0)
            factors.push_back(k);

    return factors;
}
int product (const vector<int> &clust) {
    int prod = 1;

    for (auto &num : clust)
        prod *= num;

    return prod;
}

/*
 Product Partition           Score(sc)
[708, 2]                    1420  <---- maximum value
[472, 3]                     950
[354, 2, 2]                 1074
[354, 4]                     716
[236, 3, 2]                  723
[236, 6]                     484
[177, 2, 2, 2]               740    # equals to: (177^1 + 2^3) * 4
[177, 4, 2]                  549
[177, 8]                     370
[118, 3, 2, 2]               500
[118, 4, 3]                  375
[118, 6, 2]                  378
[118, 12]                    260
[59, 3, 2, 2, 2]             350    # equals to: (59^1 + 3^1 + 2^3) * 5
[59, 6, 2, 2]                276
[59, 4, 3, 2]                272
[59, 12, 2]                  219
[59, 8, 3]                   210
[59, 6, 4]                   207
[59, 24]                     166  <---- minimum value
 */
vector<int> factorize2 (int n) {

    vector<int> factors;

    for (int k = n - 1; k > 1; k--)
        if (n % k == 0)
            factors.push_back(k);

    return factors;
}

bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}

vector<int> path; // it will store all current factors

void recurse(int max, int val) {

    if (val == 1) {
        for (int i = 0; i < path.size(); i++)
            cout << path[i] << " ";

        cout << endl;
        return;
    }


    for (int i = max; i > 1; i--) {
        if (val % i == 0) {
            path.push_back(i);
            recurse(i, val / i);
            path.pop_back();
        }
    }
}

void Output(int value) {
    cout << "Result for " << value << ": " << endl;
    recurse(value, value);
}
int main (int argc, char** argv) {
    //  g(n,[2]);

    int n = 1416;
    vector<int> factors = factorize (n);
    vector<vector<int>> partition;

    vector<int> comb;

    Output (1416);

}
