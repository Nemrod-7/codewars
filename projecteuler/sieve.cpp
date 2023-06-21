#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;

static inline uint64_t divby3 (uint64_t x) { return (0xAAAAAAABULL * x) >> 33; }
string format (int64_t x) {
    string num = to_string (x), os;
    int size = num.size(), index = 0;
    while (size-->0) {
        os += num[index++];
        if (size % 3 == 0) os += ' ';
    }
    return os;
}


void mark (int64_t prime[], int x) {
    prime[x >> 6] |= (1 << ((x >> 1) & 31));
}
vector<int64_t> primes2 (int64_t lim) { // soe with wheel factorization => ex limit == 1e8 : memory usage ~32 MB / execution time ~0.80ms
    
    int64_t half = (lim / 3) + 1;
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
vector<int64_t> primes5 (int64_t limit) { // soe with wheel factorization and bitmask operation ex limit == 1e8 : memory usage ~4 MB / ~0.97
    int64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    int64_t *sieve = new int64_t[hal + 1]();
    vector<int64_t> vs {2,3};

    for (int64_t i = 5, t = 2 ; i <= limit; i += t, t = 6 - t) {
        int64_t p = 0xAAAAAAABULL * i >> 33; //  fast division by 3
        int64_t mask = sieve[p >> 6] >> (p &63) &1ULL;

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
vector<int64_t> primes6 (int64_t n) { // simple segmennt sieve
    int64_t range = floor(sqrt(n))+1;
    vector<int64_t> prime = primes2(range);
    vector<int64_t> vs = prime;
    bool sieve[range+1];

    // Divide the range [0..n-1] in different segments We have chosen segment size as sqrt(n).
    int64_t low = range, high = 2 * range;

    while (low < n) {
        if (high >= n)
            high = n;
        // To mark primes in current range. A value in mark[i] will finally be false if 'i-low' is Not a prime, else true.
        fill_n (sieve, range+1, true);
        // Use the found primes by the reference sieve to find primes in current range
        for (int64_t i = 0; i < prime.size(); i++) {
            int p = prime[i];
            // Find the minimum number in [low..high] that is a multiple of prime[i] (divisible by prime[i]) For example, if low is 31 and prime[i] is 3, we start with 33.
            int64_t loLim = floor (low / p) * p;
            
            if (loLim < low) {
                loLim += p;
            }
            
            /* Mark multiples of prime[i] in [low..high]: We are marking j - low for j, i.e. each number
               in range [low, high] is mapped to [0, high-low] so if range is [50, 100] marking 50 corresponds
               to marking 0, marking 51 corresponds to 1 and so on. In this way we need to allocate space only for range */

            for (int64_t j = loLim; j < high; j += p)
                sieve[j - low] = false;
        }
       
        // for (int64_t i = low; i < high; i++) {
       //     if (sieve[i - low] == true) {
       //         vs.push_back(i);
       //     }
       // }

        low = low + range;
        high = high + range;
    }

    return vs;
}

vector<int64_t> primes7 (int64_t lim) { // soe with wheel factorization => ex limit == 1e8 : memory usage ~32 MB / execution time ~0.80ms
    
    int64_t half = (lim / 3) + 1;
    vector<int64_t> vs {2,3};
    bool *sieve = new bool[half]();
    int64_t i = 5, step = 2;

    for (; i * i <= lim; i += step, step = 6 - step) {
        if (sieve[i / 3] == false) {
            vs.push_back(i);
            for (int64_t j = i * i, v = step; j <= lim; j += v * i, v = 6 - v) {
                sieve[j / 3] = true;
            }
        }
    }

    for (; i <= lim; i += step, step = 6 - step) {
        if (sieve[i / 3] == false) {
            vs.push_back(i);
        }
    }

    delete[] sieve;
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


    uint64_t limit = 1e8;
    //cout << (sizeof (bool) * limit / 3) / 1024 << "\n" << (sizeof (int64_t) * limit / 192) / 1024; 
    const int64_t cs = 0xAAAAAAABULL;

    for (int i = 65; i < 104; i++) {
    //    cout << ((cs * i) >> 33) << " " << i / 3 << "\n";
    }
    //cout << format (0xAAAAAAABULL) << " " << format (cs);
        auto seq1 = primes7 (limit);
        cout << seq1.size();



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
vector<int64_t> primes3 (int64_t lim) { 
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
vector<int64_t> primes4 (int64_t lim) {
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


