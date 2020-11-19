#include <vector>
#include <algorithm>
class Decomp {
  public:
      static std::vector<long long> decompose(long long n) {

          long long n2 = 0, num;
          std::vector<long long> V = {n};

          do {
              num = V.back ();
              V.pop_back();
              n2 += num * num;

              while (num-->1)
                  if (num * num <= n2) {
                      V.push_back (num);
                      n2 -= num * num;
                  }

          } while (n2 != 0 && V.size() != 0);
          sort (V.begin(), V.end());
          return V;
     }
};
