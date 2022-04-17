#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <cmath>

using namespace std;

vector<int> gonext (int num, list<pair<int,int>> ls) {
    vector<int> next;
    //if (ls.size() == 0) return {};
    cout << num << " ";
    for (auto it = ls.begin(); it != ls.end(); ) {
        auto &[fst, scd] = *it;
        if (fst == num || scd == num) {
            if (fst == num) next.push_back (scd);
            if (scd == num) next.push_back (fst);
            it = ls.erase (it);
            gonext (next.back(), ls);
        } else {
          it++;
        }
    }

    return next;
}
int main(){

    int N = 15;
    list<pair<int,int>> ls;
    map<int,int> hist;

    for (int a = 1; a < N; a++) {
        for (int b = a + 1; b < N + 1 ; b++) {
            int sum = a + b;
            cout << a * a << " " << b * b << " :: " << sum * sum << endl;

            if ((((a * a) + (b * b))) == (2 * a * b) ) {
                hist[a]++, hist[b]++;
                ls.push_back ({a,b});
                cout << setw(2) << a << " " << setw(2) << b << " :: " << sum << endl;
            }
        }
    }

    int minv = 999;
    for (auto num : hist) {
        //cout << num.first << " :: freq -> " << num.second << endl;
        minv = min (minv, num.second);
    }

    vector<int> nxt;
    for (auto num : hist) {
        if (num.second == minv) nxt.push_back(num.first);
    }

    for (auto it = ls.begin(); it != ls.end(); it++) {
        auto &[fst, scd] = *it;
        //if (fst == 9 || scd == 9) gonext (9, ls);
    }


}
