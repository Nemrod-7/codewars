#include <vector>

class RemovedNumbers {
    public:
        static std::vector<std::vector<long long>> removNb (long long n) {

            long long cnt = n * (n + 1) / 2, a = cnt, b, sum;
            std::vector<std::vector<long long>> seq;

            for (a = 2; a < n; a++) {
                b = (cnt - a) / (a + 1), sum = cnt - a - b;

                if (b < n && a * b == sum) 
                    seq.push_back ({a,b});
            }

            return seq;
        }
};
