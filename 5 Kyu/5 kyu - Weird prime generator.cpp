#include <vector>
#include <algorithm>

using namespace std;

class WeirdPrimeGen {
    private :
        static vector<int> g (long long n) {
            static vector<int> seq = {1,7};

            for (static int k = 2; k < n; k++) {
                seq.push_back(seq[k - 1] + __gcd (k, seq[k - 1]));
                seq[k - 1] = seq[k] - seq[k - 1];
            }

            return seq;
        }
    public :
        static long long countOnes (long long n) {
            vector<int> seq = g (n);
            return count (seq.begin(), seq.begin() + n - 1, 1) + 1;
        }
        static long long maxPn (long long n) {
            vector<int> seq = g (n);

            static vector<int> seq2;
            static long long k = 0;

            for (; seq2.size() < n; k++) {
                int val = seq[k];
                if (val > 1 && find (seq2.begin(), seq2.end(), val) == seq2.end())
                    seq2.push_back (val);

                if (seq.size() - 2 == k)
                    seq = g (k * 2);
            }

            return *max_element (seq2.begin(), seq2.begin() + n);

        }
        static int anOverAverage (long long n) { return 3; }
};

int main () {
    return 0;
}
