#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <cmath>
#include <chrono>

using namespace std;

static inline uint64_t divby3 (uint64_t x) { // fast division by 3
  return (0xAAAAAAABULL * x) >> 33;
}
void mark (int64_t prime[], int x) { prime[x >> 6] |= (1 << ((x >> 1) & 31)); }

string format (int64_t x) { // format an number with separator
    string num = to_string (x), os;
    int size = num.size(), index = 0;

    while (size-->0) {
        os += num[index++];
        if (size % 3 == 0) os += ' ';
    }
    return os;
}
string showsize (uint64_t size) { // human-readable size
    const vector<string> sizes = {"TiB", "GiB", "MiB", "KiB", "B" };
    const int64_t KB = 1024;
    const int64_t MB = KB * 1024;
    const int64_t GB = MB * 1024 ;
    const int64_t TB = GB * 1024;
    uint64_t  maxv = TB;
    stringstream os;

    for (int i = 0; i < 5; i++, maxv /= 1024) {
        if (size < maxv) continue;
        os << fixed << setprecision(2) << size / static_cast<float> (maxv) << " " << sizes[i];
        return os.str();
    }

    return "";
}

vector<int64_t> primes4 (int64_t lim) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~31.71 MB / execution time ~0.80ms

    int64_t half = (lim / 2) + 1;
    vector<int64_t> vs {2,3};
    bool *sieve = new bool[half]();

    for (int64_t i = 5, step = 2; i <= lim; i += step, step = 6 - step) {
        if (sieve[i / 3] == false) {
            vs.push_back(i);
            for (int64_t j = i * i, v = step; j <= lim; j += v * i, v = 6 - v) {
                sieve[j / 3] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
list<int64_t> primes5 (int64_t limit) { // SOE with wheel factorization and bitmask operation ex limit == 1e8 : memory usage ~3.97 MB / ~0.97zA
    int64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    int64_t *sieve = new int64_t[hal + 1]();
    list<int64_t> vs {2,3};

    for (int64_t i = 5, t = 2 ; i <= limit; i += t, t = 6 - t) { // wheel factorization : 2,4A
        int64_t p = 0xAAAAAAABULL * i >> 33;           // fast division by 3
        int64_t mask = sieve[p >> 6] >> (p &63) &1ULL; // x >> 6 => fast division by 64 / x &63 => fast modulus 64

        if (mask == 0) {
            vs.push_back(i);
            for (int64_t j = i * i, v = t; j <= limit; j += v * i, v = 6 - v) {
                int64_t p2 = 0xAAAAAAABULL * j >> 33;
                sieve[p2 >> 6] |= 1ULL << (p2 &63);
            }
        }
    }

    delete[] sieve;
    return vs;
}
list<int64_t> primes7 (int64_t limit) { // SOE with 6 * i -+ 1 wheel ex limit == 1e8 : memory usage ~5.96 MB / execution time ~1.67 ms

    const int64_t sign[2] = {-1, 1};
    const int64_t half = limit >> 7;
    int64_t *sieve = new int64_t[half + 1]();
    list<int64_t> vs {2,3};
    cout << limit << " => MEM : " << showsize (half * sizeof(int64_t)) << "\n";

    for (int64_t i = 1; i <= limit; i++) {
        int64_t pre = 6 * i;
        for (int j = 0; j < 2; j++) {
            int64_t num = pre + sign[j];
            if (num > limit) return vs;

            if ((sieve[num >> 7] >> ((num >> 1) &63) &1ULL) == false) {
                vs.push_back(num);
                for (int64_t k = num * num; k <= limit; k += 2 * num) {
                    sieve[k >> 7] |= 1ULL << ((k >> 1) &63);
                }
            } 
        }
    }

    return vs;
}
vector<int64_t> primes8 (int64_t limit) { // segmented SOE ex : limit == 1e8 : memory usage 41,77 KB / execution time ~ 1.10 ms

    const int64_t L1D_CACHE_SIZE = 32768; // Set your CPU's L1 data cache size in bytes
    const int64_t sqr = static_cast<int64_t> (std::sqrt(limit));
    const int64_t mem = std::max (sqr, L1D_CACHE_SIZE);

    int64_t i = 5, n = 5;
    int64_t t = 2, u = 2;
    //cout << showsize (sizeof(bool) * (mem + sqr)) << "\n";
    bool *cache = new bool[mem];
    bool *sieve = new bool[sqr + 1]();
    std::vector<int64_t> primes, mul;
    std::vector<int64_t> vs {2,3};

    for (int64_t low = 0; low <= limit; low += mem) {
        int64_t high = std::min(low + mem - 1, limit);
        std::fill_n (cache, mem , true);
        // generate sieving primes using simple SOE
        for (; i * i <= high; i += t, t = 6 - t) {
            if (sieve[i] == false) {
                primes.push_back(i);
                mul.push_back(i * i - low);
                for (int64_t j = i * i, v = t; j <= sqr; j += i * v, v = 6 - v) {
                    sieve[j] = true;
                }
            }
        }

        // cache the current segment
        for (std::size_t p = 0; p < primes.size(); p++) {
            int64_t j = mul[p];
            for (int64_t k = primes[p] * 2; j < mem; j += k) {
                cache[j] = false;
            }
            mul[p] = j - mem;
        }

        for (; n <= high; n += u, u = 6 - u) {
            if (cache[n - low]) {
                vs.push_back (n);
            }
        }
    }
    //cout << primes.size() << " " << mul.size() << "\n";
    delete[] cache, delete[] sieve;
    return vs;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    vector<int64_t> seq = {
        2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 31,  37,  41,  43,  47,  53,  59,  61,  67,  71, 73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
        661, 673, 677, 683, 691, 701, 709, 719
    };
    vector<int64_t> wheel = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

    //const int64_t cs = 0xAAAAAAABULL;
    // 31622;

    int k = 1;
    for (int i = 0; i < 9; i++) {
    }
    //cout << (k << 2) << ' ';



    uint64_t limit = 1e2; // prime2 317.89MB, prime5 39.73MB

    // auto seq1 = primes5 (limit);
    // cout << format (seq1.size());
     cout << showsize(sizeof(bool) * 31622 / 2) << " " << showsize(sizeof(int64_t) * 31622 / (2 * 64));



    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count()  << " ms" << std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
vector<int64_t> primes1 (int64_t num) { // simple sieve
    int64_t half = (num >> 1) + 1;
    std::vector<int64_t> vs {2};
    bool *sieve = new bool [half]();

    for (int64_t p = 3; p  <= num ; p += 2) {
        if (sieve[p >> 1] == false) {
            vs.push_back(p);
            for (int64_t i = p * p; i <= num; i += 2 * p) {
                sieve[i >> 1] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
vector<int64_t> primes2 (int64_t lim) {
    int64_t size = (lim - 2) >> 1;
    int64_t lnx = 2 * size + 3;
    bool *sieve = new bool[size]();
    vector<int64_t> vs {2};

    for (int64_t i = 0; i < size; i++) {
        if (sieve[i] == false) {
            vs.push_back(2 * i + 3);

            for (int64_t j = 4L * i * i + 12L * i + 9; j <= lnx; j += 4 * i + 6) {
                sieve[(j - 3L) >> 1] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
vector<int64_t> primes3 (int64_t lim) {
    int64_t limit_i = (lim - 2) >> 1, limit_j = 2 * limit_i + 3;
    bool *sieve = new bool[((lim - 2) >> 5) + 1];
    vector<int64_t> vs {2};

    for (int64_t i = 0; i < limit_i; ++i) {
        if ((sieve[i >> 4] & 1 << (i & 0xF)) == 0) {
            vs.push_back (2 * i + 3);
            for (int64_t j = 4L * i * i + 12L * i + 9; j <= limit_j; j += 4 * i + 6) {
                sieve[((j - 3L) >> 5)] |= 1 << ((j - 3L) >> 1 & 0xF);
            }
        }
    }

    delete[] sieve;
    return vs;
}
