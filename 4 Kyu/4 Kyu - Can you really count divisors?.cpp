#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;

/////////////////////////////////Assert/////////////////////////////////////////

typedef int test_t;
class Assert {
    public :
        static void That (int actual, int expected) {
            if (actual != expected)
                cout << "actual : " << actual << " expected : " << expected << endl;
        }
};
int Equals (int src) { return src;}
void Test();


long countDivisors(const long &num) {
    long sum = 0, N = floor (sqrt (num)), k = N + 1;

    while (k-->1)
        sum += num / k - k;

    return sum * 2 + N;
}
int main () {
    auto start = std::chrono::high_resolution_clock::now();

    long num = 999999999999998; // 34693207724723862
    //long *primes = SieveOfEratosthenes (num);
    //long *primes2 = sieveofatkins (num);
    int n = 5;
    int sod = n / log (n);
    cout << sod << endl;
    //Test();
    //cout << countDivisors(10) <<endl;
    //sieveofatkins (5000000);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
}

void Test () {
    Assert::That(countDivisors(1), Equals(1));
    Assert::That(countDivisors(2), Equals(3));
    Assert::That(countDivisors(3), Equals(5));
    Assert::That(countDivisors(4), Equals(8));
    Assert::That(countDivisors(5), Equals(10));
}
