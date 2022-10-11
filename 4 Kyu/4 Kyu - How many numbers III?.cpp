#include <iostream>
#include <vector>
#include <cstdint>
#include <optional>
#include <tuple>
#include <cmath>
#include <algorithm>

using out_t = std::tuple<std::uint32_t, std::uint64_t, std::uint64_t>;

std::optional<out_t> find_all (std::uint32_t dsum, std::uint32_t ndig) {

    if (ndig > dsum) return std::nullopt;

    uint32_t md = ceil (dsum / (float) ndig);
    uint64_t end = 0, start = 0;
    uint32_t sum = 0, cnt = 0;
    std::vector<int> maxv (ndig, md), minv (ndig, 1);

    do {
        sum = 0;
        for (uint32_t i = 0; i < ndig; i++) {
            sum += maxv[i];
            if (sum > dsum && maxv[i] > 1)
                maxv[i]--;
        }
        sort (maxv.begin(), maxv.end());
    } while (sum > dsum);

    //do {
        sum = 0;
        for (uint32_t i = 0; i < ndig; i++) {
            sum += minv[i];
        }
        if (sum < dsum) {
            const uint32_t len = std::min (ndig, dsum - sum);
            for (uint32_t i = 0; i < len; i++) {
                if (minv[i] < 10)
                    minv[i]++;
            }
        }
        sort (minv.begin(), minv.end());
    //} while (sum < dsum);

    for (uint32_t i = 0; i < ndig; i++) {
        start = start * 10 + minv[i];
        end = end * 10 + maxv[i];
    }

    for (uint64_t i = start; i < end; i++) {

        uint64_t num = i;
        uint64_t sum = 0, lst = num % 10;

        while (num) {
            uint64_t dig = num % 10;
            sum += dig;

            if (dig > lst) {
                sum = 0;
                break;
            }

            lst = dig;
            num /= 10;
        }

        if (sum == dsum)
            cnt++;
    }

    std::cout << cnt << " " << start << " " << end;
    return std::optional<out_t> {{cnt, start, end}};
}

void display (std::optional<out_t>  res) {
    if (res != std::nullopt) {
      auto [cnt,minv,maxv] = *res;
      std::cout << cnt << " " << minv << " " << maxv;
    }
}

int main () {

    //find_all (10, 3);
    int dsum = 10, ndig = 3;


    uint32_t md = ceil (dsum / (float) ndig);

    uint32_t sum = 0, cnt = 0;
    std::vector<int> maxv (ndig, md), minv (ndig, 1);

    //do {
        sum = 0;
        for (uint32_t i = 0; i < ndig; i++) {
            sum += minv[i];
        }
        for (uint32_t i = 0; i < ndig; i++) {
            if (sum < dsum) {
              uint32_t poss = 9 - minv[i];
              minv[i] = std::min (poss, dsum - sum);
              sum += std::min (poss, dsum - sum);
            }
            //std::cout << minv[i];
        }

        sort (minv.begin(), minv.end());

        for (uint32_t i = 0; i < ndig; i++) {
            std::cout << minv[i];
        }
    //} while (sum < dsum);

    std::cout << "end";
}
