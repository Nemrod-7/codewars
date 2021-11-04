#include <map>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <cstring>


template<std::size_t S> int solve (std::array<int, S> a0) {
    int cnt = S;
    std::map<int,int> hist;

    for (size_t i = 0; i < S; i++) {
        hist.clear();
        hist[a0[i]] = 1;

        for (size_t j = i + 1; j < S; j++) {

            bool flag = true;
            hist[a0[j]]++;

            for (auto it : hist) {
                if (it.second % 2 == 0)
                      flag = false;
            }

            if (flag) cnt++;
        }
    }

    return cnt;
}
