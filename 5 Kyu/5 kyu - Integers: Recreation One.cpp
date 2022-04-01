#include <cmath>
#include <vector>

class SumSquaredDivisors {
    public:
        static std::vector<std::pair<long long, long long>> listSquared (long long m, long long n) {
          
            std::vector<std::pair<long long, long long>> res;
          
            for (long long i = m; i <= n; i++) {
                long long sum = 0;
                for (long long j = 1; j <= i; j++) {
                  
                    if (i % j == 0) sum += (j * j);
                }
                long long s2 = sqrt (sum);

                if (s2 * s2 == sum) res.push_back ({i, sum});      
            }
            
            return res;
        }
};
