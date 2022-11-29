#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef pair <long long, long long> test_t;
class Assert {
    public :
        static void That (test_t actual, test_t express) {
            if (actual != express) {
                cout << "actual : <" << actual.first << "," << actual.second << "> ";
                cout << "expected : <" << express.first << "," << express.second << "> " << endl;
            }

        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

class GapInPrimes {
    public:
        static std::pair <long long, long long> gap(int g, long long m, long long n);
};

bool is_prime (uint32_t num) {
    if (num < 2) return false;
    if (num <=3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    ///if (num < 25) return true;
    const uint32_t end = sqrt (num);

    for (uint32_t i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
uint32_t *SieveOfEratosthenes (uint32_t num) {
    bool *primes = new bool[num + 1];
    uint32_t *sieve = new uint32_t[num + 1];

    const uint32_t end = sqrt (num);
    uint32_t p, i, size = 1;

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
pair <long long, long long> GapInPrimes::gap(int step, long long start, long long end) {

    //int *primes = SieveOfEratosthenes (end);
    pair <long long, long long> index, out;

    for (long long actual = start; actual <= end; ++actual) {

        if (is_prime(actual)) {
            index.first = index.second;
            index.second = actual;
        }

        if (index.second - index.first == step)
            return index;
    }

    return {};
}
vector<uint64_t> sieve1 (uint64_t num) {

    const uint64_t end = sqrt (num);
    bool *sieve = new bool[num + 1];
    vector<uint64_t> prime;

    fill_n (sieve, num, true);

    for (uint64_t p = 2; p <= end ; p++)
        if (sieve[p] == true)
            for (uint64_t i = p * p; i <= num; i += p)
                 sieve[i] = false;

    for (uint64_t i = 2; i <= num; i++)
         if (sieve[i] == true)
            prime.push_back (i);

    return prime;
}
vector<uint64_t> sieve2 (uint64_t num) {

    const uint64_t end = sqrt (num);
    bool *sieve = new bool[num + 1];
    vector<uint64_t> prime {2};

    fill_n (sieve, num + 1, true);

    for (uint64_t p = 3; p <= end ; p += 2) {
        if (sieve[p] == true) {
            for (uint64_t i = p * p; i <= num; i += p) {
                 sieve[i] = false;
            }
        }
    }

    for (uint64_t i = 3; i <= num; i += 2) {
         if (sieve[i] == true) {
            prime.push_back (i);
         }
    }

    return prime;
}
vector<uint64_t> sieve3 (uint64_t num) { //x2 / half space

    const uint64_t end = sqrt (num);
    bool *sieve = new bool[num / 2 + 1];
    vector<uint64_t> prime {2};

    fill_n (sieve, num / 2 + 1, true);

    for (uint64_t p = 3; p <= end ; p += 2)
        if (sieve[p / 2] == true)
            for (uint64_t i = p * p; i <= num; i += 2 * p)
                 sieve[i / 2] = false;

    for (uint64_t i = 3; i <= num; i += 2)
        if (sieve[i / 2] == true)
            prime.push_back (i);

    return prime;
}
int main () {

    auto start = std::chrono::steady_clock::now();

    auto p = GapInPrimes::gap(11,30000,100000);

    int num = 100000;

    bool *sieve = new bool[num + 1];
    fill_n (sieve, num + 1, true);

    for (uint64_t p = 2; p * p <= num ; p++)
        if (sieve[p] == true)
            for (uint64_t i = p * p; i <= num; i += p)
                 sieve[i] = false;

    for (uint64_t p = 30000; p <= 30025 ; p++) {
        if (sieve[p] == true) {

            cout << p << " ";
        }
    }


    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;
}

void testequal(std::pair <long long, long long> ans, std::pair <long long, long long> sol) {
    Assert::That(ans, Equals(sol));
}
void dotest(int g, long long m, long long n, std::pair <long long, long long> expected) {
    testequal(GapInPrimes::gap(g, m, n), expected);
}
void Test() {
    dotest(2,100,110, {101, 103});
    dotest(4,100,110, {103, 107});
    dotest(6,100,110, {0, 0});
    dotest(8,300,400, {359, 367});
    dotest(10,300,400, {337, 347});
    dotest(2,5,5, {0, 0});
    dotest(2,5,7, {5, 7});
    dotest(4,130,200, {163, 167});
}
