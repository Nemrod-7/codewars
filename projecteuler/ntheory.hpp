#include <string>
#include <vector>
#include <cmath>
#include <cstdint>

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }
int lcm (int a, int b) { return a * gcd (a,b) / b; }

bool issquare (int64_t num) {
    int64_t sq = sqrt(num);
    return sq * sq == num;
}
bool is_prime (int64_t num) {

    if (num < 2) return false;
    if (num < 4) return true;
    if (num % 2 == 0 || num % 3 == 0 ) return false;

    for (int64_t i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
std::vector<uint32_t> sieve (int64_t limit) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~31.71 MB / execution time ~0.80ms
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    uint64_t *sieve = new uint64_t[hal + 1]();
    std::vector<uint32_t> vs {2,3};

    for (uint64_t i = 5, t = 2 ; i * i <= limit; i += t, t = 6 - t) { // wheel factorization : 2,4A
        uint64_t p = 0xAAAAAAABULL * i >> 33;           // fast division by 3
        uint64_t mask = sieve[p >> 6] >> (p &63) &1ULL; // x >> 6 => fast division by 64 / x &63 => fast modulus 64

        if (mask == 0) {
            for (uint64_t j = i * i, v = t; j <= limit; j += v * i, v = 6 - v) {
                uint64_t p2 = 0xAAAAAAABULL * j >> 33;
                sieve[p2 >> 6] |= 1ULL << (p2 &63);
            }
        }
    }

    for (uint32_t i = 5, t = 2; i <= limit; i += t, t = 6 - t) {
        uint32_t p = 0xAAAAAAABULL * i >> 33;

        if ((sieve[p >> 6] >> (p &63) &1ULL) == false) {
            vs.push_back(i);
        }
    }

    delete[] sieve;
    return vs;
}
int64_t tau (int64_t n) { // count number of divisors
    int64_t total = 1;

    for (; (n & 1) == 0; n >>= 1) // Deal with powers of 2 first
        ++total;

    for (int64_t p = 3; p * p <= n; p += 2) { // Odd prime factors up to the square root
        int count = 1;
        for (; n % p == 0; n /= p)
            ++count;
        total *= count;
    }

    if (n > 1) total *= 2; // If n > 1 then it's prime
    return total;
}
int64_t sigma (int64_t num) { // sum of proper divisors

    int64_t n = num, sum = 1;
    int64_t p = 2;

    while (p * p <= n && n > 1) {
        if (n % p == 0) {
            int64_t j = p * p;
            n /= p;

            while (n % p == 0) {
                j *= p;
                n /= p;
            }

            sum = sum * (j - 1) / (p - 1);
        }
        p += (p == 2) ? 1 : 2;
    }

    if (n > 1) sum *= (n + 1);

    return sum - num;
}
int64_t phi (int64_t num) { // totient funtion

    int64_t res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0)
            num /= 2;

        res -= res / 2;
    }

    for (int64_t pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0)
                num /= pr;

            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}
int64_t phi2 (int64_t num, std::vector<int64_t> &prime) { // totient funtion

    int64_t res = num;
    int64_t *p = prime.data();

    for (int64_t i = 0; p[i] * p[i] <= num; i++) {
        if (num % p[i] == 0) {
            while (num % p[i] == 0)
                num /= p[i];

            res -= res / p[i];
        }
    }

    return (num > 1) ? res - res / num : res;
}
std::vector<int64_t> phi3 (int64_t lim) { // sieve of totient
    std::vector<int64_t> sieve (lim + 1);

    for (int64_t i = 0; i <= lim; i++)
        sieve[i] = i;

    for (int64_t i = 2; i <= lim; i++) {
        if (sieve[i] == i) {
            for (int64_t j = i; j <= lim; j += i)
                sieve[j] -= sieve[j] / i;
        }
    }
    return sieve;
}

int64_t fibonacci (int64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci (n - 1) + fibonacci (n - 2);
}
std::string collatz (int64_t n) {

    std::string os;

    while (n != 1) {
        os += std::to_string (n) + " -> ";
        n = (n % 2 == 0) ? n * 0.5 : 3 * n + 1;
    }
    return os + "1";
}
void collatz2 (int64_t a1) {
    const char alpha[3] = {'D','U','d'};
    string seq;
    cout << a1 << " => ";
    while (a1 > 1) {
        int mod = a1 % 3;

        switch (mod) {
            case 0 : a1 = a1 / 3; break;
            case 1 : a1 = (4 * a1 + 2) / 3; break;
            case 2 : a1 = (2 * a1 - 1) / 3; break;
            default: break;
        }
        seq += alpha[mod];
        cout << alpha[mod];
    }
}

bool isPentagonal (int64_t N) {
    double n = (1 + sqrt(24*N + 1))/6;
    return (n - (int64_t) n) == 0;
}
bool check_goldbach (int64_t num, const std::vector<int64_t> &prime) {

    for (int64_t i = 0; i < prime.size() && prime[i] < num; i++) {
        for (int64_t k = 1; k * k < num; k++) {
            if (prime[i] + 2 * (k * k) == num) {
                //cout << prime[i] << " + 2 x " << k << "²";
                return true;
            }
        }
    }

    return false;
}
void farey (int n) {

    typedef struct { int d, n; } frac;
    frac f1 = {0, 1}, f2 = {1, n}, tmp;
    int k;
    printf("%d/%d ", 1, n);
    while (f2.n > 1) {
        k = (n + f1.n) / f2.n;
        tmp = f1;
        f1 = f2;
        f2 = (frac) { f2.d * k - tmp.d, f2.n * k - tmp.n };
        std::cout << f2.d << "/" << f2.n << " ";
    }
}
int64_t cntdiv (int64_t num) {
    int np = 0;

    while ((num &1) == 0) {
        num >>= 1;
        np++;
        if (np > 2) return np;
    }

    for (int64_t p = 3; p * p <= num; p += 2) {
        while (num % p == 0) {
            num /= p;
            np++;
            if (np > 2) return np;
        }
    }
    if (num > 1) np++;
    return np;
}

int64_t reverse (int64_t num) {

    int64_t rev = 0;

    do {
        rev = rev * 10 + num % 10;
    } while (num /= 10);

    return rev;
}
bool palindrome (int64_t num) {
    if (num % 10 == 0) return false;
    return reverse (num) == num;
}

int64_t radical (int64_t n) {
    // A007947 -> r(n) -> Π p|n
    if (is_prime (n)) return n;

    int64_t res = (n % 2 == 0) ? 2 : 1;
    while (n % 2 == 0) n /= 2;

    for (int64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            res *= i;
            while (n % i == 0) n /= i;
        }
    }

    if (n != 1) res *= n;
    return res;
}
double resilience (int64_t d) {
    return phi (d) / static_cast<double> (d-1);
}
