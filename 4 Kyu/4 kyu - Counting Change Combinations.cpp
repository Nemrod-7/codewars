#include <iostream>
#include <vector>

unsigned long long countChange(const unsigned int N, const std::vector<unsigned int>& coins) {

    std::vector<uint64_t> part(N + 1);
    part[0] = 1;

    for (auto &c : coins)
        for (size_t i = c; i <= N; ++i)
            part[i] += part[i - c];;

    return part[N];
}



int main () {


}
