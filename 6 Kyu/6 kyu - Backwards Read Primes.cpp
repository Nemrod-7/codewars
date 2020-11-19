#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;
class BackWardsPrime {
    public:
      static std::string backwardsPrime(long long start, long long end);
};


typedef long long ll;
//std::vector<uint64_t> SieveOfEratosthenes (uint64_t num) ;
bool is_prime (vector<uint64_t> base, uint64_t num) ;
long long backnum (long long num);

void Test();

int main () {
    auto start = std::chrono::high_resolution_clock::now();
    //vector<uint64_t> base = SieveOfEratosthenes (1095403 * 10);
    Test();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Process took " << elapsed.count()  << " ms" << std::endl;

    return 0;
}

uint64_t *SieveOfEratosthenes (uint64_t num) {

    bool *primes = new bool[num+1];
    uint64_t *sieve_v = new uint64_t[num];
    const uint64_t end = sqrt (num);
    int p, i, next = 0;
    //std::fill_n (primes, num , false);
    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                primes[i] = true;


    for (i = 2; i <= num; i++)
        if (primes[i] == false)
            sieve_v[next++] = i;
            //sieve.push_back(i);

    return sieve_v;
}
bool is_prime_v2 (vector<uint64_t> &base, uint64_t num) {

    if (num <= 3) return true;
    else if (num % 2 == 0 || num % 3 == 0) return false;

    else
        for (uint64_t &prime : base)
            if (num == prime)
                return true;

    return false;
}
bool is_prime_v1 (uint64_t num) {
    const uint64_t end = sqrt (num);
    uint64_t i;
    if (num <= 3) return true;
    else if (num % 2 == 0 || num % 3 == 0) return false;

    else {
        for (i = 5; i <= end; i += 6)
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
    }

    return true;
}
long long backnum (long long num) {
    string temp = to_string (num);
    reverse (temp.begin(), temp.end());
    char *end;
    long long int number;
    return strtoll(temp.c_str(), &end, 10);
}

string BackWardsPrime::backwardsPrime(long long start, long long end) {

    //uint64_t * base = SieveOfEratosthenes (end * 10);
    ostringstream os;
    int cnt = 0;
    for (long long num = start; num <= end ; ++num)

        if (is_prime_v1(num)) {
            long long back = backnum (num);
            if (back != num)
                if (is_prime_v1(back) ) {
                    if (cnt++ > 0)
                        os << " ";
                    os << num;
                    }
            }

    return os.str();
}

void Test () {


      cout << BackWardsPrime::backwardsPrime(1068658,1070158) << endl;
cout << BackWardsPrime::backwardsPrime(966232,967732) << endl;
cout << BackWardsPrime::backwardsPrime(299402,300902) << endl;
cout << BackWardsPrime::backwardsPrime(412661,414161) << endl;
cout << BackWardsPrime::backwardsPrime(226338,227838) << endl;
cout << BackWardsPrime::backwardsPrime(921271,922771) << endl;
cout << BackWardsPrime::backwardsPrime(38860,40360) << endl;
    cout << BackWardsPrime::backwardsPrime(390801,392301) << endl;


    cout << BackWardsPrime::backwardsPrime(698008,699508) << endl;
    cout << BackWardsPrime::backwardsPrime(148770,150270) << endl;

cout << BackWardsPrime::backwardsPrime(71847,73347) << endl;
cout << BackWardsPrime::backwardsPrime(970739,972239) << endl;


}
