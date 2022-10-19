#include <iostream>
#include <vector>
#include <cstdint>
#include <optional>
#include <tuple>

using out_t = std::tuple<std::uint32_t, std::uint64_t, std::uint64_t>;

std::optional<out_t> find_all (uint32_t sum, uint32_t ndig) {

    if (sum < ndig || sum > ndig * 9) return std::nullopt;
    uint32_t cnt = 0;
    uint64_t minv = 999999, maxv = 0;

    using vertex = std::tuple<uint64_t,uint32_t,uint32_t>;
    std::vector<vertex> comb {{0,0,0}};

    while (!comb.empty()) {

        auto [curr, total, len] = comb.back();
        uint32_t alt = len + 1;
        comb.pop_back();

        if (total == sum && len == ndig) {
          minv = std::min (minv, curr);
          maxv = std::max (maxv, curr);
          cnt++;
        } else {
            for (uint32_t i = curr % 10; i < 10; i++) {
                if (i != 0/* && total + i <= sum && len + 1 <= ndig */) {
                    comb.push_back({curr * 10 + i, total + i, alt});
                }
            }
        }
    }
    return std::optional<out_t> {{cnt,minv,maxv}};
}

int main () {

    uint32_t dsum = 10, ndig = 3;

    //Assert::That(find_all(10, 3), Equals(std::optional<out_t>(out_t(8, 118, 334))));

    uint32_t cnt = 0;
    uint64_t minv = -1, maxv = 0;

    using vertex = std::tuple<uint64_t,uint32_t,uint32_t>;
    std::vector<vertex> comb {{0,0,0}};

    while (!comb.empty()) {

        auto [curr, total, len] = comb.back();
        uint32_t alt = len + 1;
        comb.pop_back();

        if (total == dsum && len == ndig) {
          minv = std::min (minv, curr);
          maxv = std::max (maxv, curr);
          cnt++;
        } else if (total < dsum && len < ndig) {
            for (uint32_t i = curr % 10; i < 10; i++) {
                if (i != 0) {
                    comb.push_back({curr * 10 + i, total + i, alt});
                }
            }
        }
    }

    std::cout << cnt << " " << minv << " " << maxv << "\n";
/*
Assert::That(find_all(27, 3), Equals(std::optional<out_t>(out_t(1, 999, 999))));
Assert::That(find_all(84, 4), Equals(std::nullopt));
Assert::That(find_all(35, 6), Equals(std::optional<out_t>(out_t(123, 116999, 566666))));

  */
    std::cout << "\nend";
}
