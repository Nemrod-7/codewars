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
uint64_t int_sqrt (uint64_t s) {
	if (s <= 1) return s;
	uint64_t x0 = s / 2, x1 = (x0 + s / x0) / 2;
	while (x1 < x0) {
		x0 = x1;
		x1 = (x0 + s / x0) / 2;
	}
	return x0;
}

string format (uint64_t x) { // format an number with separator
    string num = to_string (x), os;
    int size = num.size(), index = 0;

    while (size-->0) {
        os += num[index++];
        if (size % 3 == 0) os += ',';
    }
    os.pop_back();
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
vector<uint64_t> primes2 (uint64_t limit) { // in progress
    uint64_t size = (limit - 2) >> 1;
    uint64_t lnx = 2 * size + 3;
    bool *sieve = new bool[size]();
    vector<uint64_t> vs {2};
    int64_t cnt = 1;
    cout << showsize(sizeof(bool) * size) << " => ";

    for (uint64_t i = 0; i < size; i++) {
        if (sieve[i] == false) {
            //cout << 2 * i + 3 << " ";
            cnt++;
            for (uint64_t j = 4L * i * i + 12L * i + 9; j <= lnx; j += 4 * i + 6) {
                sieve[(j - 3L) >> 1] = true;
            }
        }
    }
cout << format (cnt) << "\n";
    delete[] sieve;
    return vs;
}
vector<uint64_t> primes4 (uint64_t limit) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~31.71 MB / execution time ~0.80ms

    uint64_t half = (limit / 2) + 1;
    vector<uint64_t> vs {2,3};
    bool *sieve = new bool[half]();

    for (uint64_t i = 5, step = 2; i <= limit; i += step, step = 6 - step) {
        if (sieve[i / 3] == false) {
            vs.push_back(i);
            for (uint64_t j = i * i, v = step; j <= limit; j += v * i, v = 6 - v) {
                sieve[j / 3] = true;
            }
        }
    }

    delete[] sieve;
    return vs;
}
vector<uint32_t> primes5 (uint64_t limit) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~3.97 MB / execution time ~0.80ms
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    uint64_t *sieve = new uint64_t[hal + 1]();
    vector<uint32_t> vs {2,3};

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
vector<uint64_t> primes6 (uint64_t limit) { // segmented SOE ex : limit == 1e8 : memory usage 41,77 KB / execution time ~ 1.10 ms

    const uint64_t L1D_CACHE_SIZE = 1 << 15;  //  Set your CPU's L1 data cache size in bytes : 32768
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
vector<uint64_t> primes7 (uint64_t limit) { // SoE whith advanced wheel factorization and bitmask => ex limit == 1e8 : memory usage 3,97 MB / execution time ~0.60 ms
    const uint64_t wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    // const uint64_t end = int_sqrt (limit);
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
vector<uint64_t> primes8 (uint64_t limit) { // segmented SOE with advance wheel factorization ex : limit == 1e8 : memory usage 41,77 KB / execution time ~ 0.82 ms

    const uint64_t wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};
    const uint64_t L1D_CACHE_SIZE = 32768; // Set your CPU's L1 data cache size in bytes
    const uint64_t sqr = static_cast<uint64_t> (std::sqrt(limit));
    const uint64_t mem = std::max (sqr, L1D_CACHE_SIZE);

    uint64_t i = 11, n = 11;
    uint64_t t = 0, u = 0;
    int64_t cnt = 4;
    bool *cache = new bool[mem];
    bool *sieve = new bool[sqr + 1]();
    std::vector<uint64_t> primes, mul;
    std::vector<uint64_t> vs {2,3};

    cout << showsize (sizeof(bool) * (mem + sqr)) << " ";
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
            }
        }
    }

    cout << format (cnt) << "\n";
    //cout << primes.size() << " " << mul.size() << "\n";
    delete[] cache, delete[] sieve;
    return vs;
}

void square (int limit) {
  const uint64_t mod = 1000000007;


  uint64_t half = (limit >> 1) + 1;
  std::vector<uint64_t> vs;
  bool *sieve = new bool [limit + 1]();
  sieve[0] = sieve[1] = true;

  for (uint64_t p = 2; p * p <= limit ; p++) {
      if (sieve[p] == false) {
          for (uint64_t i = p * p; i <= limit; i +=  p) {
              sieve[i] = true;
          }
      }
  }
  uint64_t sum = 0;
  vector<uint64_t> sqr;

  for (int i = 1; i <= limit; i++) {
      if (sieve[i] == false) {
          sum += 1;
          sqr.push_back(i*i);

      } else {
          uint64_t mxd = 1;

          for (int j = 1; j * j <= i; j++) {
              if (i % (j * j) == 0) {
                  mxd = j;
              }
          }

          // for (int j = 0; j < sqr.size() && sqr[j] <= i; j++) {
          //     if (i % sqr[j] == 0) {
                  // mxd = sqr[j];
          //     }
          // }

          cout << i << " :: " << mxd << "\n";
          sum += mxd;
      }
  }

  cout << "S: " << sum;

  delete[] sieve;
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

    const double π = 3.1415926535897932384626;
    uint64_t limit = 1e9; // 1e14
    uint64_t sum = 0;
    vector<uint64_t> sqr;
    // Problem 745 : A008833 		Largest square dividing n

    square(100);

    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count()  << " ms" << std::endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////



vector<uint64_t> primes3 (uint64_t lim) { // proto
    const uint64_t limit_i = (lim - 2) >> 1;
    const uint64_t limit_j = 2 * limit_i + 3;
    const uint64_t mem = ((lim - 2) >> 5) + 1;
    // reserve ceil(1.25506 * n / log(n));
    char *sieve = new char[mem]();
    vector<uint64_t> vs {2};
    int64_t cnt = 1;
    cout << showsize(sizeof(char) * mem) << " => ";

    for (uint64_t i = 0; i < limit_i; ++i) {
        if ((sieve[i >> 4] & 1 << (i & 0xF)) == 0) {
            cnt++;
            //cout << 2 * i + 3 << " ";
            for (uint64_t j = 4L * i * i + 12L * i + 9; j <= limit_j; j += 4 * i + 6) {
                sieve[((j - 3L) >> 5)] |= 1 << ((j - 3L) >> 1 & 0xF);
            }
        }
    }

    cout << format(cnt) << "\n";
    delete[] sieve;
    return vs;
}
