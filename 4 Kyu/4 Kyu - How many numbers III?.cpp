#include <iostream>
#include <cstdint>
#include <optional>
#include <tuple>

using out_t = std::tuple<std::uint32_t, std::uint64_t, std::uint64_t>;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void show (std::optional<out_t>  res) {
            if (res != std::nullopt) {
              auto [cnt,minv,maxv] = *res;
              std::cout << cnt << " " << minv << " " << maxv;
            }
        }
        static void That (const std::optional<out_t> &a, const std::optional<out_t> &b) {
            if (a != b) {
                std::cout << "actual : ";
                show (a);

                std::cout << " expected : ";
                show (b);

                std::cout << std::endl;
            }
        }
};
std::optional<out_t> Equals (const std::optional<out_t>& entry) { return entry;}

void recurse (uint64_t curr, uint32_t prev, uint32_t sum, uint32_t dig, out_t &hist) {

    auto &[cnt,minv,maxv] = hist;

    if (sum == 0 && dig == 0) {
        if (cnt == 0) minv = curr;
        minv = std::min (minv,curr);
        maxv = std::max (maxv,curr);
        cnt++;
        std::cout << curr << " " << prev << " " << sum << " " << dig << "\n";
    } else if (dig != 0) {
        for (uint32_t i = prev; i < 10 && i <= sum; i++) {
            recurse (10 * curr + i, i, sum - i, dig - 1, hist);
        }
    }
}
std::optional<out_t> find_all (uint32_t sum, uint32_t ndig) {

    if (sum < ndig || sum > ndig * 9) return std::nullopt;
    out_t res = {0,0,0};
    recurse (0, 1, sum, ndig, res);
    //std::cout << get<0>(res) << " " << get<1>(res) << " " << get<2>(res) << "\n";
    return std::optional<out_t> {res};
}



int main () {

    int dsum = 35, ndig = 6;

    Assert::That(find_all(10, 3), Equals(std::optional<out_t>(out_t(8, 118, 334))));



/*
Assert::That(find_all(27, 3), Equals(std::optional<out_t>(out_t(1, 999, 999))));
Assert::That(find_all(84, 4), Equals(std::nullopt));
Assert::That(find_all(35, 6), Equals(std::optional<out_t>(out_t(123, 116999, 566666))));

  */
    std::cout << "\nend";
}

/*

uint64_t vtoi (std::vector<uint32_t> arr) {

    uint64_t num = 0;

    for (size_t i = 0; i < arr.size(); i++) {
        num = num * 10 + arr[i];
    }

    return num;
}
std::optional<out_t> find_all4 (std::uint32_t dsum, std::uint32_t ndig) {

    if (dsum < ndig || dsum > (ndig * 9)) return std::nullopt;

    uint32_t sum = 0;
    std::vector<uint32_t> arr (ndig, 1);

    for (uint32_t i = 0; i < ndig; i++) {
        sum += arr[i];
    }

    for (uint32_t i = 0; i < ndig; i++) {
        uint32_t low = 9 - arr[i];

        arr[i] += std::min (low, dsum - sum);
        sum += std::min (low, dsum - sum);
    }

    sort (arr.begin(), arr.end());

    uint64_t start = vtoi (arr), end;
    bool flag = true;
    uint32_t cnt = 0;

    while (flag) {
        flag = false;

        for (uint32_t i = 0; i < ndig - 1; i++) {
            if ((arr[i] + 1) < arr[i+1]) {
                arr[i]++, arr[i+1]--;
                flag = true;
                break;
            }
        }

        for (uint32_t i = 0; i < ndig; i++) {
            cout << arr[i];
        }
        cout << endl;
        cnt += flag;
    }

    for (uint32_t i = 0; i < ndig - 1; i++) {
        cout << arr[i];
        if ((arr[i]) < arr[i+1]) {
            arr[i]++, arr[i+1]--;
        }
    }

    for (uint32_t i = 0; i < ndig; i++) {
    }


    end = vtoi (arr);
    //std::cout << cnt << " " << start << " " << end << " => ";
    return std::optional<out_t> {{cnt, start, end}};
}

std::vector<uint32_t> maxv (std::uint32_t dsum, std::uint32_t ndig) {

  uint32_t md = ceil (dsum / (float) ndig);
  uint32_t sum = 0;
  std::vector<uint32_t> maxv (ndig, md);

  do {
    sum = 0;
    for (uint32_t i = 0; i < ndig; i++) {
      sum += maxv[i];
      if (sum > dsum && maxv[i] > 1)
      maxv[i]--;
    }
    sort (maxv.begin(), maxv.end());
  } while (sum > dsum);

  return maxv;
}
std::vector<uint32_t> minv (std::uint32_t dsum, std::uint32_t ndig) {

  uint32_t sum = 0;
  std::vector<uint32_t> minv (ndig, 1);

  for (uint32_t i = 0; i < ndig; i++) {
    sum += minv[i];
  }

  for (uint32_t i = 0; i < ndig; i++) {
    uint32_t low = 9 - minv[i];

    minv[i] += std::min (low, dsum - sum);
    sum += std::min (low, dsum - sum);
  }

  sort (minv.begin(), minv.end());
  return minv;
}
std::optional<out_t> find_all5 (std::uint32_t dsum, std::uint32_t ndig) {

  if (dsum < ndig || dsum > (ndig * 9)) return std::nullopt;

  auto end = maxv (dsum, ndig);
  auto start = minv (dsum,ndig);
  uint32_t cnt = 0;

  for (uint32_t i = 0; i < ndig; i++) {
      int32_t dig = (end[i] - start[i]);

      cout << dig;
  }

  //return std::optional<out_t> {{cnt, start, end}};
}
std::optional<out_t> find_all2 (std::uint32_t dsum, std::uint32_t ndig) {

  if (dsum < ndig || dsum > (ndig * 9)) return std::nullopt;

  std::vector<uint32_t> high = maxv (dsum,ndig), low = minv (dsum,ndig);

  uint64_t start = 0, end = 0, cnt = 0;
  for (uint64_t i = 0; i < ndig; i++) {
    cout << low[i];
    start = start * 10 + low[i];
    end = end * 10 + high[i];
  }

  //std::cout << cnt << " " << start << " " << end << " => " << end - start;
  return std::optional<out_t> {{cnt, start, end}};
}
std::optional<out_t> find_all3 (std::uint32_t dsum, std::uint32_t ndig) {

    if (ndig < dsum || dsum > ndig * 9) return std::nullopt;

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

    sum = 0;
    for (uint32_t i = 0; i < ndig; i++) {
        sum += minv[i];
    }
    for (uint32_t i = 0; i < ndig; i++) {
        uint32_t poss = 9 - minv[i];
        minv[i] += std::min (poss, dsum - sum);
        sum += std::min (poss, dsum - sum);
    }
    sort (minv.begin(), minv.end());

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
*/
