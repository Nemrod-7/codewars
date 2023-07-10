
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class combinations {
    private :
        inline static int size;
        inline static std::vector<std::vector<int>> ts, combs;
        //const std::vector<std::vector<int>> &ls;
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
            std::vector<int> base (depth + 1), comb (depth);

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

int main () {

      // probelm 206      Concealed Square
      // 1_2_3_4_5_6_7_8_9_0
      std::string number = "1_2_3_4_5_6_7_8_9_0";

      // auto base = combinations::integral (7, "0123456789");


      /*
    */
}
