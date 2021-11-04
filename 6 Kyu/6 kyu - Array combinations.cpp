#include <vector>
#include <set>

int solve(const std::vector<std::vector<int>> &data) {

    int k = 1;
    std::vector<std::set<int>> S (data.size());

    for (size_t i = 0; i < data.size(); i++)
        for (auto const &dig : data[i])
            S[i].insert (dig);

    for (auto const &it : S)
        k *= it.size();

    return k;
}
