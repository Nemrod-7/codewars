#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <array>
#include <map>
#include <set>
#include <limits>
#include <chrono>

// NEQ5xBztxeg43aP
using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
        template<class T> static void That (const vector<T> &a, const vector<T> &b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
};
template<class T> T Equals (const T& entry) { return entry;}
template<class T> T EqualsContainer (const T& entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
/*
   Product Partition           Score(sc)
   [708, 2]                    1420  <---- maximum value
   [472, 3]                     950
   [354, 2, 2]                 1074
   [354, 4]                     716
   [236, 3, 2]                  723
   [236, 6]                     484
   [177, 2, 2, 2]               740    # equals to: (177^1 + 2^3) * 4
   [177, 4, 2]                  549
   [177, 8]                     370
   [118, 3, 2, 2]               500
   [118, 4, 3]                  375
   [118, 6, 2]                  378
   [118, 12]                    260
   [59, 3, 2, 2, 2]             350    # equals to: (59^1 + 3^1 + 2^3) * 5
   [59, 6, 2, 2]                276
   [59, 4, 3, 2]                272
   [59, 12, 2]                  219
   [59, 8, 3]                   210
   [59, 6, 4]                   207
   [59, 24]                     166  <---- minimum value

*/

int rate (vector<int> part) {
    int res = 0;
    map<int,int> hist;

    for (auto &num : part) {
        hist[num]++;
        // if let Some(cnt) = hist.get(&num) {
        //     hist.insert (*num, cnt+1);
        // } else {
        //     hist.insert (*num, 1);
        // }
    }

    for (auto &it : hist) {
        auto [x,y] = it;
        res += pow(x,y);
    }

    return res * part.size();
}

pair<vector<int>, int> recurse (int n, int k, vector<int> &part, string mode) {

    if (n == 1) {
        cout << rate(part) << " => [";
        for (auto &it : part) {
            cout << " " << it;
        }
        cout << "]\n";

        return {part, rate (part)};
    } else {
        pair<vector<int>,int> ref;
        ref.second = mode == "max" ? numeric_limits<int>::min() : numeric_limits<int>::max();

        while (k > 1) {
            if (n % k == 0){
                part.push_back(k);
                auto res = recurse (n / k, k, part, mode);

                if (mode == "max") {
                    if (res.second > ref.second) {
                        ref = res;
                    }
                } else {
                    if (res.second < ref.second) {
                        ref = res;
                    }
                }

                part.pop_back();
            }
            k--;
        }

        return ref;
    }
}
pair<vector<int>,int> find_spec_prod_part (int n, string mode) {

    vector<int> part;
    pair<vector<int>,int> res = recurse (n, n-1, part, mode);

    return res;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    auto res = find_spec_prod_part(1416, "max"); // ([708, 2], 1420)

    // find_spec_prod_part(1416, "min"); // ([59, 24], 166)
    // dotest(10007, "max", None);
/*


     1: 1
     3: 1,3
     6: 1,2,3,6
    10: 1,2,5,10
    15: 1,3,5,15
    21: 1,3,7,21
    28: 1,2,4,7,14,28

    What is the value of the first triangle number to have over five hundred divisors?
    // 1621954689976164466
    uint64_t num = 5000, tri, k = 0, nd = 1;

    do {
        k++;
        tri = (k * (k + 1)) / 2;
        nd = tau (tri);
    } while (nd < 500);

    cout << k << " => "<< tri << " : " << nd;
    */

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
