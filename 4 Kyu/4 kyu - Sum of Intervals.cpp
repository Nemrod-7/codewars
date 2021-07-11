#include <vector>
#include <limits>
#include <numeric>

int sum_intervals(std::vector<std::pair<int, int>> intervals) {
    int minv = std::numeric_limits<int>::max(), maxv = 0;

    for (auto &it : intervals) {
        maxv = std::max (maxv, it.second);
        minv = std::min (minv, it.first);
    }

    std::vector<int> range (maxv - minv);

    for (auto &it : intervals)
        for (int i = it.first; i < it.second; i++)
            range[i - minv] = true;

    return std::accumulate (range.begin(), range.end(), 0);
}
