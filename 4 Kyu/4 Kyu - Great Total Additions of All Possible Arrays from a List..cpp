#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

string mklst (size_t lim, const vector<int>& args) {

    string list;
    vector<vector<int>> arg2;
    bool hist[10] = {0}, running = true;

    for (auto num : args) {
        vector<int> decomp;

        do {
            decomp.push_back (num % 10);
        } while (num /= 10);

        arg2.push_back (decomp);
    }

    while (running) {
        for (auto &vs : arg2) {
            if (vs.size()) {
                int dig = vs.back();

                if (!hist[dig] && running) {
                    list += dig;
                }
                if (list.size() == lim) running = false;
                hist[dig] = true;
                vs.pop_back();
            }
        }
    }

    return list;
}
int searchcomb (size_t depth, string list) {

    int sum = 0;
    vector<string> s1 ({""});

    while (!s1.empty()) {
        string comb = s1.back();
        s1.pop_back();

        if (comb.size() == depth) {
            //sum += accumulate (comb.begin(), comb.end(), 0);
            for (size_t i = 0; i < depth; i++) {
                sum += comb[i];
            }
        } else {
            for (auto &dig : list) {
                if (comb.find (dig) == string::npos) {
                    s1.push_back (comb + dig);
                }
            }
        }
    }

    return sum;
}
int gta (int limit, const vector<int>& args) {

    int sum = 0;
    string list = mklst (limit, args);

    while (limit-->0)
        sum += searchcomb (limit + 1, list);

    return sum;
}

void test () {

    int limit = 6;
    vector<int> numbers = {71311,42522,14489,18292};
    vector<int> base = {7,4,1,2,8,3,5,9};
    //Expected: equal to 40775


    limit = 5;
    numbers = {90487,63746,73762,9361,43107};
    base = {9,6,7,4,0,3,1,8,2};
    //Expected: equal to 6786


    limit = 5;
    numbers = {947538,677645,373496,821637,807905,670482};
    base = {9,6,3,8,4,7,2,0,1,5};
    //Expected: equal to 7830
    cout << gta (limit, numbers) << "\n";

}

int main () {

    vector<int> list = {1, 5, 6, 2, 7, 3, 4};

    int limit = 6;
    vector<int> numbers = {71311,42522,14489,18292};
    //base list = {8,9,1,2,6,7}
    test();

    return 0;
}
