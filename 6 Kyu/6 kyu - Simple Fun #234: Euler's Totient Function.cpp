#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;
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

vector<int> SieveOfEratosthenes2 (int num) {
    const int end = sqrt (num);
    bool *primes = new bool[num + 1];
    vector<int> sieve;//(num + 1);
    int p, i, size = 0;

    fill_n (primes, num, 0);

    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;

    for (i = 2; i <= num; i++)
        if (primes[i] == false)
            sieve.push_back(i);

    return sieve;
}

int *SieveOfEratosthenes (int num) {
    int end = sqrt (num);
    bool *primes = new bool[num + 1];// = {0};
    int *sieve = new int[num*0.5+1];
    int p, i, size = 1;

    fill_n (primes, num, 0);

    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;

    for (i = 2; i <= num; i++)
         if (primes[i] == false)
            sieve[size++] = i;

    sieve[0] = size;
    return sieve;
}
int eulersTotientFunction (int n){
    int res = n;
    const int end = sqrt (n);
    //vector<int> primes = SieveOfEratosthenes(end);
    int *primes = SieveOfEratosthenes(end);
    for (int i = 1; i < primes[0]; ++i)
        if (n % primes[i] == 0) {
            while (n % primes[i] == 0)
                n /= primes[i];

            res -= res / primes[i];
        }

    return (n > 1) ? res - res / n : res;
}


int main () {
      auto start = chrono::steady_clock::now();

      int count = 0;

      while (chrono::steady_clock::now() - start < chrono::milliseconds(2000))
          eulersTotientFunction (count++);


      //Test();

      cout << "computed : " << count << endl;
      auto end = chrono::steady_clock::now();
      chrono::duration<double> elapsed = end - start;
      cout << "Process took " << elapsed.count()  << " ms" << endl;
}
void Test () {

    Assert::That(eulersTotientFunction(5), Equals(4));
    Assert::That(eulersTotientFunction(49), Equals(42));
    Assert::That(eulersTotientFunction(1), Equals(1));
    Assert::That(eulersTotientFunction(9), Equals(6));
    Assert::That(eulersTotientFunction(10), Equals(4));
}
