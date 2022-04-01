#include <sstream>
#include <vector>
#include <tuple>
#include <limits>
#include <algorithm>

using namespace std;

namespace Closest {
    int getweight (long long num) {
        int sum = 0;
        do { sum += num % 10; } while (num /= 10);
        return sum;
    }
    vector<tuple<int, unsigned int, long long>> closest (const string &src) {
      
        unsigned int index = 0;
        long long num;
        stringstream iss (src);
        vector<tuple<int, unsigned int, long long>> base, res (2);
                
        while (iss >> num)
            base.push_back ({getweight (num), index++, num});

        if (base.size() == 0) return res;
        sort (base.begin(), base.end());
        
        int minv = numeric_limits<int>::max();
        
        for (size_t i = 0; i < base.size() - 1; i++) {
            int dist = abs (get<0>(base[i]) - get<0>(base[i + 1]));

            if (dist < minv) {
                res[0] = base[i], res[1] = base[i + 1];
                minv = dist;
            }      
        }

        return res;
    }
}
