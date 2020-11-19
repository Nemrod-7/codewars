#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;

/////////////////////////////////Assert/////////////////////////////////////////

typedef int test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expected) {
            if (actual != expected)
                cout << "actual : " << actual << " expected : " << expected << endl;
        }
};
test_t Equals (test_t src) { return src;}
void Test();
////////////////////////////////////////////////////////////////////////////////

class KPrimes {
    public:
        static std::vector<long long> countKprimes(int k, long long start, long long end);
        static int puzzle(int s);
};

long long *SieveOfEratosthenes (long long num) {
    bool *primes = new bool[num + 1];
    long long *sieve = new long long[num +1]; //vector<long long> sieve(num);
    const unsigned end = sqrt (num);
    unsigned p, i, size = 1;

    fill_n (primes, num, 0);

    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;

    for (i = 2; i <= num; i++)
         if (primes[i] == false)
            sieve[size++] = i;

    sieve[0] = size;
    //delete primes;
    return sieve;
}
bool is_prime(int num) {

    const unsigned end = sqrt (num);
    unsigned i;
    if (num <= 3) return num > 1;
    else if (num % 2 == 0 || num % 3 == 0) return false;

    else {
        for (i = 5; i <= end; i += 6)
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
    }

    return true;
}
long long count_k(long long num, const long long *sieve) {
    int i = 0, count = 0;
    int div;
    long long size = sieve[0];

    while (num > 1) {
        for (i = 1; i < size; ++i)
            if (num % sieve[i] == 0) {
                num /= sieve[i];
                break;
            }

        count++;
    }

    return count;
}

vector<long long> KPrimes::countKprimes(int k, long long start, long long end) {
    long long num , curr, count;
    vector<long long> k_primes;

    for (num = start; num <= end; ++num) {
        count = 0;
  			curr = num;

  			for (int i = 2; i <= curr / i; i++)
  				  while (curr % i == 0) {
  					     count++;
  					     curr /= i;
  				  }

  			if (curr > 1)
  				  count++;

  			if (count == k)
            k_primes.push_back(num);
		}

    return k_primes;
}
int KPrimes::puzzle(int s) {

   int num = 0, cnt = 0;
   vector<long long> onek = KPrimes::countKprimes(1, 0, s);
   vector<long long> threek = KPrimes::countKprimes(3, 0, s);
   vector<long long> sevenk = KPrimes::countKprimes(7, 0, s);

     for (auto &one : onek)
         for (auto &three : threek)
             for (auto &seven : sevenk)
                 if (seven + three + one == s)
                     cnt++;
    return cnt;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  // countKprimes(5, 500, 600) --> [500, 520, 552, 567, 588, 592, 594]

  KPrimes::countKprimes(8,10000000,10000200);

  Test();
  //for (auto it : sieve)
  //    cout << it << " ";
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;


  return 0;
}
void testequal(std::vector<long long> ans, std::vector<long long> sol) {
    //Assert::That(ans, Equals(sol));
}
void testequalInt(int ans, int sol) {
    Assert::That(ans, Equals(sol));
}
void dotest(int k, long long start, long long nd, std::vector<long long> expected) {
    testequal(KPrimes::countKprimes(k, start, nd), expected);
}
void Test() {
        std::vector<long long> sol = {4,6,9,10,14,15,21,22,25,26,33,34,35,38,39,46,49,51,55,57,58,62,65,69,74,77,82,85,86,87,91,93,94,95};
        //dotest(2, 0, 100, sol);
        //sol =  {8, 12, 18, 20, 27, 28, 30, 42, 44, 45, 50, 52, 63, 66, 68, 70, 75, 76, 78, 92, 98, 99};
        //dotest(3, 0, 100, sol);

        testequalInt(KPrimes::puzzle(138), 1);
        testequalInt(KPrimes::puzzle(143), 2);
        testequalInt(KPrimes::puzzle(250), 7);
KPrimes::countKprimes(2,0,100);
KPrimes::countKprimes(3,0,100);
KPrimes::countKprimes(5,1000,1100);
KPrimes::countKprimes(5,500,600);
KPrimes::countKprimes(7,1000,1500);
KPrimes::countKprimes(7,10000,10100);
KPrimes::countKprimes(7,100000,100100);
KPrimes::countKprimes(12,100000,100100);
KPrimes::countKprimes(1,2,30);
KPrimes::countKprimes(8,10000000,10000200);
KPrimes::puzzle(138);
KPrimes::countKprimes(7,0,138);
KPrimes::countKprimes(3,0,138);
KPrimes::countKprimes(1,0,138);
KPrimes::puzzle(143);
KPrimes::countKprimes(7,0,143);
KPrimes::countKprimes(3,0,143);
KPrimes::countKprimes(1,0,143);
KPrimes::puzzle(250);
KPrimes::countKprimes(7,0,250);
KPrimes::countKprimes(3,0,250);
KPrimes::countKprimes(1,0,250);
KPrimes::puzzle(2500);
KPrimes::countKprimes(7,0,2500);
KPrimes::countKprimes(3,0,2500);
KPrimes::countKprimes(1,0,2500);
KPrimes::puzzle(2600);
KPrimes::countKprimes(7,0,2600);
KPrimes::countKprimes(3,0,2600);
KPrimes::countKprimes(8,5381330,5383080);
KPrimes::countKprimes(7,5722535,5724490);
KPrimes::countKprimes(8,2698220,2699701);
KPrimes::countKprimes(6,6801228,6802637);
KPrimes::countKprimes(7,2720454,2721961);
KPrimes::countKprimes(7,5524555,5526024);
KPrimes::countKprimes(8,5050029,5051958);
KPrimes::countKprimes(8,2455414,2457057);
KPrimes::countKprimes(6,7605724,7607569);
KPrimes::countKprimes(6,4522635,4523668);
KPrimes::countKprimes(7,3148415,3149508);
KPrimes::countKprimes(5,2328347,2330286);
/*

*/
}
