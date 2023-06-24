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
void mark (uint64_t prime[], int x) { prime[x >> 6] |= (1 << ((x >> 1) & 31)); }

string format (uint64_t x) { // format an number with separator
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
    const uint64_t KB = 1024;
    const uint64_t MB = KB * 1024;
    const uint64_t GB = MB * 1024 ;
    const uint64_t TB = GB * 1024;
    uint64_t  maxv = TB;
    stringstream os;

    for (int i = 0; i < 5; i++, maxv /= 1024) {
        if (size < maxv) continue;
        os << fixed << setprecision(2) << size / static_cast<float> (maxv) << " " << sizes[i];
        return os.str();
    }

    return "";
}

vector<uint64_t> primes6 (uint64_t limit) { // segmented SOE ex : limit == 1e8 : memory usage 41,77 KB / execution time ~ 1.10 ms

    const uint64_t L1D_CACHE_SIZE = 32768; // Set your CPU's L1 data cache size in bytes
    const uint64_t sqr = static_cast<uint64_t> (std::sqrt(limit));
    const uint64_t mem = std::max (sqr, L1D_CACHE_SIZE);

    uint64_t i = 5, n = 5;
    uint64_t t = 2, u = 2;
    int64_t cnt = 0;
    cout << showsize (sizeof(bool) * (mem + sqr)) << " ";
    bool *cache = new bool[mem];
    bool *sieve = new bool[sqr + 1]();
    std::vector<uint64_t> primes, mul;
    std::vector<uint64_t> vs {2,3};

    for (uint64_t low = 0; low <= limit; low += mem) {
        uint64_t high = std::min(low + mem - 1, limit);
        std::fill_n (cache, mem , true);
        // generate sieving primes using simple SOE
        for (; i * i <= high; i += t, t = 6 - t) {
            if (sieve[i] == false) {
                primes.push_back(i);
                mul.push_back(i * i - low);
                for (uint64_t j = i * i, v = t; j <= sqr; j += i * v, v = 6 - v) {
                    sieve[j] = true;
                }
            }
        }

        // cache the current segment
        for (std::size_t p = 0; p < primes.size(); p++) {
            uint64_t j = mul[p];
            for (uint64_t k = primes[p] * 2; j < mem; j += k) {
                cache[j] = false;
            }
            mul[p] = j - mem;
        }

        for (; n <= high; n += u, u = 6 - u) {
            if (cache[n - low]) {
                cnt++;
                //vs.push_back (n);
            }
        }
    }

    cout << format (cnt) << "\n";
    //cout << primes.size() << " " << mul.size() << "\n";
    delete[] cache, delete[] sieve;
    return vs;
}
vector<uint64_t> primes7 (uint64_t limit) { // SoE whith advanced wheel factorization and bitmask => ex limit == 1e8 : memory usage 3,97 MB / execution time ~0.6 ms 
    const uint64_t wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    
    uint64_t *sieve = new uint64_t[hal + 1]();
    std::vector<uint64_t> vs {2,3,5,7};
    cout << setw(10) << limit << " ";
    cout << showsize(sizeof(uint64_t) * hal) << " => ";
    
    for (uint64_t i = 11, t = 0; i * i <= limit ; i += wheel[t], t = t == 47 ? 0 : t + 1) {
        uint64_t p = 0xAAAAAAABULL * i >> 33;           // fast division by 3
        uint64_t mask = sieve[p >> 6] >> (p &63) &1ULL; // x >> 6 => fast division by 64 / x &63 => fast modulus 64

        if (mask == 0) {
            for (uint64_t j = i * i, v = t; j <= limit; j += i * wheel[v], v = v == 47 ? 0 : v + 1) {
                uint64_t p2 = 0xAAAAAAABULL * j >> 33;
                sieve[p2 >> 6] |= 1ULL << (p2 &63);
            }
        }
    }

    for (uint64_t i = 11, t = 0; i <= limit ; i += wheel[t], t = t == 47 ? 0 : t + 1) {
        uint32_t p = 0xAAAAAAABULL * i >> 33;
        if ((sieve[p >> 6] >> (p &63) &1ULL) == false) {
            vs.push_back(i);
        }
    }

    cout << vs.size() << "\n";
    delete[] sieve;
    return vs;
}


vector<uint64_t> primes8 (uint64_t limit) { // segmented SOE ex : limit == 1e8 : memory usage 41,77 KB / execution time ~ 0.82 ms

    const uint64_t wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};
    const uint64_t L1D_CACHE_SIZE = 32768; // Set your CPU's L1 data cache size in bytes
    const uint64_t sqr = static_cast<uint64_t> (std::sqrt(limit));
    const uint64_t mem = std::max (sqr, L1D_CACHE_SIZE);

    uint64_t i = 11, n = 11;
    uint64_t t = 0, u = 0;
    int64_t cnt = 4;
    cout << showsize (sizeof(bool) * (mem + sqr)) << " ";
    bool *cache = new bool[mem];
    bool *sieve = new bool[sqr + 1]();
    std::vector<uint64_t> primes, mul;
    std::vector<uint64_t> vs {2,3};

    for (uint64_t low = 0; low <= limit; low += mem) {
        uint64_t high = std::min(low + mem - 1, limit);
        std::fill_n (cache, mem , true);
        // generate sieving primes using simple SOE
        for (; i * i <= high ; i += wheel[t], t = t == 47 ? 0 : t + 1) {
            if (sieve[i] == false) {
                primes.push_back(i);
                mul.push_back(i * i - low);
                for (uint64_t j = i * i, v = t; j <= sqr; j += i * wheel[v], v = v == 47 ? 0 : v + 1) {
                    sieve[j] = true;
                }
            }
        }

        // cache the current segment
        for (std::size_t p = 0; p < primes.size(); p++) {
            uint64_t j = mul[p];
            for (uint64_t k = primes[p] * 2; j < mem; j += k) {
                cache[j] = false;
            }
            mul[p] = j - mem;
        }

        for (; n <= high; n += wheel[u], u = u == 47 ? 0 : u + 1) {
            if (cache[n - low]) {
                cnt++;
                //vs.push_back (n);
            }
        }
    }

    cout << format (cnt) << "\n";
    //cout << primes.size() << " " << mul.size() << "\n";
    delete[] cache, delete[] sieve;
    return vs;
}
int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    vector<uint64_t> seq = {
        2,   3,   5,   7,  11,  13,  17,  19,  23,  29, 31,  37,  41,  43,  47,  53,  59,  61,  67,  71, 73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
        661, 673, 677, 683, 691, 701, 709, 719
    };

    uint64_t wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

    uint32_t limit = 1000000000; // prime2 317.89MB, prime5 39.73MB
    const uint64_t cs = 0xAAAAAAAB;
    // cout << pr.size();

    //cout << format (100000001);

    primes6(1e8);
    for (int i = 10; i < 1000000; i *= 10) {
    }

    //
    //    int32_t i32 = numeric_limits<int32_t>::max();
    //    uint32_t u32 = numeric_limits<uint32_t>::max();
    //    int64_t i64 = numeric_limits<int64_t>::max();
    //    uint64_t u64 = numeric_limits<uint64_t>::max();
    //
    //    cout << i32 << " " << sqrt(i32) << "\n";
    //    cout << u32 << " " << sqrt(u32) << "\n";
    //
    //    cout << i64 << " " << (int64_t) sqrt(i64) << "\n";
    //    cout << u64 << " " << (uint64_t) sqrt(u64) << "\n";
    //
    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count()  << " ms" << std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
vector<uint64_t> primes1 (uint64_t num) { // simple sieve
    uint64_t half = (num >> 1) + 1;
    std::vector<uint64_t> vs {2};
    bool *sieve = new bool [half]();

    for (uint64_t p = 3; p  <= num ; p += 2) {
        if (sieve[p >> 1] == false) {
            vs.push_back(p);
            for (uint64_t i = p * p; i <= num; i += 2 * p) {
                sieve[i >> 1] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
vector<uint64_t> primes2 (uint64_t lim) {
    uint64_t size = (lim - 2) >> 1;
    uint64_t lnx = 2 * size + 3;
    bool *sieve = new bool[size]();
    vector<uint64_t> vs {2};

    for (uint64_t i = 0; i < size; i++) {
        if (sieve[i] == false) {
            vs.push_back(2 * i + 3);

            for (uint64_t j = 4L * i * i + 12L * i + 9; j <= lnx; j += 4 * i + 6) {
                sieve[(j - 3L) >> 1] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
vector<uint64_t> primes3 (uint64_t lim) {
    uint64_t limit_i = (lim - 2) >> 1, limit_j = 2 * limit_i + 3;
    bool *sieve = new bool[((lim - 2) >> 5) + 1];
    vector<uint64_t> vs {2};

    for (uint64_t i = 0; i < limit_i; ++i) {
        if ((sieve[i >> 4] & 1 << (i & 0xF)) == 0) {
            vs.push_back (2 * i + 3);
            for (uint64_t j = 4L * i * i + 12L * i + 9; j <= limit_j; j += 4 * i + 6) {
                sieve[((j - 3L) >> 5)] |= 1 << ((j - 3L) >> 1 & 0xF);
            }
        }
    }

    delete[] sieve;
    return vs;
}
vector<uint64_t> primes4 (uint64_t lim) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~31.71 MB / execution time ~0.80ms

    uint64_t half = (lim / 2) + 1;
    vector<uint64_t> vs {2,3};
    bool *sieve = new bool[half]();

    for (uint64_t i = 5, step = 2; i <= lim; i += step, step = 6 - step) {
        if (sieve[i / 3] == false) {
            vs.push_back(i);
            for (uint64_t j = i * i, v = step; j <= lim; j += v * i, v = 6 - v) {
                sieve[j / 3] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
list<uint32_t> primes5 (int64_t limit) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~3.97 MB / execution time ~0.80ms
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    uint64_t *sieve = new uint64_t[hal + 1]();
    list<uint32_t> vs {2,3};

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
