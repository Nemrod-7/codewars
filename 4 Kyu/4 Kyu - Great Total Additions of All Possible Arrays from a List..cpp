#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class combinations {
    private :
        inline static int size;
        inline static std::vector<std::vector<int>> ts, combs;

        static void helper (std::vector<int> comb, int index) { // make unique combinations of subsets 1 / 1

            for (size_t i = 0; i < ts[index].size(); i++) {

                int dig = ts[index][i];
                if (std::find (&comb[0], &comb[index], dig) != &comb[index]) continue;

                comb[index] = dig;

                if (index == size - 1) {
                    combs.push_back (comb);
                } else {
                    helper (comb, index + 1);
                }
            }
        }

    public :
        static std::vector<std::vector<int>> subset (const std::vector<std::vector<int>> &data) {       // make unique combinations of subsets

            size = data.size(), ts = data;
            helper (std::vector<int> (size), 0);

            return combs;
        }
        static std::vector<std::vector<int>> distinct (int depth, const std::vector<int> &data) { // make unique combinations of n choose k

            size = data.size();
            std::vector<bool> mask (size);
            std::fill (mask.end() - depth, mask.end(), true);

            do {
                std::vector<int> comb;

                for (int i = 0; i < size; ++i) {
                    if (mask[i]) { comb.push_back(data[i]); }
                }

                combs.push_back (comb);

            } while (std::next_permutation(mask.begin(), mask.end()));

            return combs;
        }
        static std::vector<std::vector<int>> integral (int depth, const std::string &dict) { // all possible combinations : n choose k

            size = dict.size();
            int index = 0;
            std::vector<int> base (depth), comb (depth);

            while (base[depth] == 0) {

                for (int i = 0; i < depth; i++)
                    comb[i] = dict[base[i]];

                combs.push_back (comb);

                base[0]++;

                while (base[index] == size) {
                    base[index++] = 0;
                    base[index]++;
                }
                index = 0;
            }
            return combs;
        }
};

vector<int> vc (int n) {
    vector<int> ls;

    do {
        ls.push_back (n % 10);
    } while (n /= 10);

    return ls;
}
vector<int> mklst (int a, int b, int c) {

    int flag = true;
    vector<int> list;
    vector<vector<int>> lss = {vc(a), vc(b), vc(c)};

    while (flag) {
      flag = false;
      for (auto &vs : lss) {
        if (vs.size()) {
          int dig = vs.back();
          flag = true;
          vs.pop_back();
          list.push_back (dig);
        }
      }
    }

    return list;
}

int gta (int lim, int a, int b, int c) {

    int sum = 0;
    vector<int> list = mklst (a,b,c);

    for (int i = 0; i < lim; i++) {
        cout << list[i];
        for (int j = i; j < i; j++) {
          cout << list[j];

        }

        cout << endl;
    }

    return sum;
}

void distinct (int depth, const std::vector<int> &data) { // make unique combinations of n choose k

  int size = data.size();
  int index = 0;
  std::vector<int> base (depth), comb (depth);

  while (base[depth] == 0) {

      for (int i = 0; i < depth; i++) {
          comb[i] = data[base[i]];
      //    cout << data[base[i]];
      }
      //combs.push_back (comb);
      base[0]++;

      while (base[index] == size) {
          base[index++] = 0;
          base[index]++;
      }
      index = 0;
  }
}

int main () {

    distinct (2, {1, 5, 6, 2, 7, 3, 4});


    return 0;
}
