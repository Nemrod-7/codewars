#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>

using namespace std;

bool isnaive (unordered_map<int,int> &hist) {
  for (auto &it : hist) {
      if (it.second % 2 == 0)
          return false;
  }
  return true;
}
template<std::size_t S> int solve (std::array<int, S> a0) {

    int cnt = S;
    unordered_map<int,int> hist;

    for (size_t i = 0; i < S; i++) {

        hist.clear();
        hist[a0[i]] = 1;
        cout << a0[i] << ' ';
        for (size_t j = i + 1; j < S; j++) {
            hist[a0[j]]++;
            cout << a0[j] << ' ';
            if (isnaive (hist)) cnt++;
        }

        cout << endl;

    }
    cout << cnt;
    return cnt;
}

template<std::size_t S> int solve2 (std::array<int, S> a0) {

    int cnt = S;

    for (size_t i = 0; i < S; i++) {
        vector<pair<int,int>> hist (1);
        hist[0] = {a0[i], 1};

        for (size_t j = i + 1; j < S; j++) {

            bool found = false;
            for (int k = 0; k < hist.size(); k++) {
                if (hist[k].first == a0[j]) {
                    hist[k].second++;
                    found = true;
                    break;
                }
            }
            if (!found) hist.push_back ({a0[j], 1});
            found = true;
            for (int k = 0; k < hist.size(); k++) {
                if (hist[k].second % 2 == 0) {
                    found = false;
                    break;
                }
            }

            cnt += found;
        }
    }

    return cnt;
}
int main () {


  std::array<int, 4> a1 = {2, 2, 2, 3};


  cout << solve2 (a1);
}
