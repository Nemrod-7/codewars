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
list<int64_t> primes5 (int64_t limit) { // SOE with wheel factorization and bitmask operation ex limit == 1e8 : memory usage ~3.97 MB / ~0.97
    int64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    int64_t *sieve = new int64_t[hal + 1]();
    list<int64_t> vs {2,3};

    for (int64_t i = 5, t = 2 ; i <= limit; i += t, t = 6 - t) { // wheel factorization : 2,4
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
vector<int64_t> primes6 (int64_t n) { // simple segment sieve
    int64_t range = floor(sqrt(n))+1;
    vector<int64_t> prime = primes4(range);
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

void segmented_sieve(int64_t limit) {

		const int64_t L1D_CACHE_SIZE = 32768; // Set your CPU's L1 data cache size (in bytes) here
		int64_t sqrt = (int64_t) std::sqrt(limit);
		int64_t segment_size = std::max(sqrt, L1D_CACHE_SIZE);
		int64_t count = (limit < 2) ? 0 : 1;

		// we sieve primes >= 3
		int64_t i = 3, n = 3, s = 3;

		std::vector<char> sieve(segment_size);
		std::vector<char> is_prime(sqrt + 1, true);
		std::vector<int64_t> primes;
		std::vector<int64_t> multiples;

		for (int64_t low = 0; low <= limit; low += segment_size) {
				// current segment = [low, high]
				int64_t high = low + segment_size - 1;

				high = std::min(high, limit);
				std::fill(sieve.begin(), sieve.end(), true);

				// generate sieving primes using simple sieve of Eratosthenes
				for (; i * i <= high; i += 2)
						if (is_prime[i])
								for (int64_t j = i * i; j <= sqrt; j += i)
										is_prime[j] = false;

				// initialize sieving primes for segmented sieve
				for (; s * s <= high; s += 2) {
						if (is_prime[s]) {
								primes.push_back(s);
								multiples.push_back(s * s - low);
						}
				}

				// sieve the current segment
				for (std::size_t i = 0; i < primes.size(); i++) {
						int64_t j = multiples[i];
						for (int64_t k = primes[i] * 2; j < segment_size; j += k)
								sieve[j] = false;

						multiples[i] = j - segment_size;
				}

				for (; n <= high; n += 2)
						if (sieve[n - low]) {
								std::cout << n << " ";
						}
		}

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

		uint64_t limit = 1e8; // prime2 317.89MB, prime5 39.73MB
		const int64_t cs = 0xAAAAAAABULL;
		// cout << ceil (1.25506 * 100 / (log(100))) << " ";
		// cout << (sizeof (bool) * limit / 3) / 1024 << "\n" << (sizeof (int64_t) * limit / 192) / 1024;
		// 8.57ms
		//cout << format (0xAAAAAAABULL) << " " << format (cs);

		for (int i = 1; i < seq.size(); i++) {
				int sep = seq[i] - seq[i-1];
				if (sep == 2) cout << "\n";
				cout << sep << " ";
		}


		// auto seq1 = primes5 (100);
		//
		// for (auto p : seq1) {
		//     // cout << p << " ";
		// }
		// cout << format (seq1.size());



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
