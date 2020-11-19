#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// In the preloaded section are some functions that can help.
// They can be used as a small library.
// There is no file to include, only the templates below.

#include <vector>


class PrimeConsec {
    private:
        static int countPrimeDiv(long long num) {
            int i = 1, count = 0, div;

            i = 2;
            while (num > 1) {
                while (num % i == 0) {
                    num /= i;
                    count++;
                }
                i++;
            }

            return count;

        }

    public:
        static int consecKprimes(int k, std::vector<long long> &arr) {
            int count = 0;
            bool flag = false;

            for (auto act : arr) {
                if (countPrimeDiv (act) == k) {
                    if (flag == true)
                        count++;
                    flag = true;
                } else
                    flag = false;
            }

            return count;
        }


};
