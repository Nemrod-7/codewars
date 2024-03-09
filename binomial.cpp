#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct term {
    int num;
    string val;
};

vector<vector<int>> binomial (int size) {
  vector<vector<int>> tri (size, vector<int> (size));

  for (int i = 0; i < size; ++i) {
      tri[i][0] = tri[i][i] = 1;

      for (int j = 1; j < i; ++j) {
          tri[i][j] = tri[i-1][j-1] + tri[i -1][j];
      }
  }
  return tri;
}

void expand (string expr) {
    const int up = expr.find("^");
    string num = expr.substr(0, up), exx = expr.substr(up + 1);
    int ex = stoi(exx);

    int size = ex + 1;
    vector<term> code (size);
    vector<vector<int>> tri = binomial (size);

    for (int i = 0; i < size; i++) {
        int mul = tri[size-1][i];
        // int nu1 = pow(t1.num, ex-i), nu2 = pow(t2.num, i);
        // int total = mul * nu1 * nu2;
        //
        // cout << total;
        //
        // if (t1.val != "") {
        //     if (ex-i > 0) {
        //       cout << t1.val << "^";
        //       cout << ex-i;
        //     }
        // }

        // cout << "+";
        // cout << mul << " " << nu1 << ' ' << nu2  << " :: " << total << "\n";
    }
}

int main () {

    //expand("(x+1)^2");      // returns "x^2+2x+1"
    //expand("(p-1)^3");      // returns "p^3-3p^2+3p-1"
    //expand("(2f+4)^6");     // returns "64f^6+768f^5+3840f^4+10240f^3+15360f^2+12288f+4096"
    //expand("(-2a-4)^0");    // returns "1"
    //expand("(-12t+43)^2");  // returns "144t^2-1032t+1849"
    //expand("(r+0)^203");    // returns "r^203"
    //expand("(-x-1)^2");     // returns "x^2+2x+1

    // (a + b)^5 = a^5 + 5 * a^4 * b^1 + 10 * a^3 b^2 + 10a^2 b^3 + 5a^1 b^4 + a^0 b^5;

    string expr = "(2f+4)^6";

    const int up = expr.find("^");
    string num = expr.substr(0, up), exx = expr.substr(up + 1);
    int ex = stoi(exx);


    // cout << num;

}
