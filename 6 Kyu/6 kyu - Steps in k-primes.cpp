#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

typedef vector<pair<long,long>> test_t;

class Assert {
      public :
          static void That (test_t actual, test_t expression) {
              if (actual != expression) {
                  cout << "actual : ";
                  for (auto it : actual)
                      cout << "<" << it.first << "," << it.second << ">";

                  cout << " expected : ";
                  for (auto it : expression)
                      cout << "<" << it.first << "," << it.second << ">";
              }
              cout << endl;

          }
};
test_t Equals (test_t src) { return src;}

long long *sieve3 (long long num) {

    bool *sieve = new bool[num / 2 + 1];
    long long *prime = new long long[num / 2];
    unsigned size = 2;

    fill_n (sieve, num / 2 + 1, true);
    prime[1] = 2;

    for (long long p = 3; p * p <= num ; p += 2)
        if (sieve[p / 2] == true)
            for (long long i = p * p; i <= num; i += 2 * p)
                 sieve[i / 2] = false;

    for (long long i = 3; i <= num; i += 2)
         if (sieve[i / 2] == true)
            prime[size++] = i;

    prime[0] = size;

    return prime;
}
long long count_k(long long num, const long long *sieve) {
    int i = 0, count = 0;
    int div;

    while (num > 1) {
        div = sieve[i];
        while (num % div == 0) {
            num /= div;
            count++;
        }
        i++;
    }

    return count;
}
vector<long long> kPrimes(int k, long long start, long long end) {
    long long num;
    vector<long long> stack;
    const long long *primes = sieve3 (end);

    for (num = start; num <= end; ++num)
        if (count_k (num, primes) == k)
            stack.push_back(num);

    return stack;
}

namespace KStep {
    vector<pair<long,long>> kprimesStep(int k, int step, long long start, long long end) {
      long long size = 0;
      vector<pair<long,long>> result;
      vector<long long> root = kPrimes(k, start, end);

      for (auto fst : root)
          for (auto scd : root)
              if (scd - fst == step)
                  result.push_back({fst,scd});

      //for (auto it : result)
      //    cout << "<" << it.first << "," << it.second << ">";

      return result;
    }
}


void dotest(int k, int step, long long m, long long n, std::vector<std::pair <long, long>> sol) {
    Assert::That(KStep::kprimesStep(k, step, m, n), Equals(sol));
}
void Test () {
    std::vector<std::pair <long, long>> sol = {};
    dotest(10, 8, 2425364, 2425700, sol);

    sol = {{2627408, 2627416}, {2627440, 2627448}, {2627496, 2627504}};
    dotest(6, 8, 2627371, 2627581, sol);
}

int main (){
    auto start = std::chrono::high_resolution_clock::now();

    // kprimes_step(2, 2, 0, 50) will return [[4, 6], [33, 35]]
    //KStep::kprimesStep(2, 2, 0, 50);
    KStep::kprimesStep(6, 8, 2627371, 2627581);

    Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

}
