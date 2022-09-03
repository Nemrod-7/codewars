#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

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
    for (auto it : list) {
        cout << it << " ";
    }

    return sum;
}
int main () {

    gta(7, 123489, 5, 67);
    return 0;
}
