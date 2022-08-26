#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <bitset>

using namespace std;

const string powtwo = "^0|10+$";
const string divtwo = "^0|1(0|1)*0$";
const string divtre = "^0|(1(01*0)*10*)*$";
const string divfor = "^0|1(0|1)*(0|1)?00$";


string clean (string str) {
    int end = min (str.find_first_not_of ('0'), str.size() - 1);
    str.erase (0, end);
    return str;
}
void test2 () {

    string input;
    while (cin >> input) {

        if (input == "exit") break;

        regex eng (input);

        for (int i = 0; i < 25; i++) {

            bitset<32> num (i);
            string str = clean(num.to_string());

            if (regex_match (str, eng)) {

              cout << setw(3) << i << " ";
              cout << setw(8) << str;
              cout << ' ' << (i % 3 == 0 ? "true  " : "false ");

              cout <<  "\n";
            }
        }
    }
}

enum {q0,q1,q2,q3,q4,q5,q6};

bool dfa_7 (const string& binary) {
  int state = q0;
  // "1(0|1)"
  for (auto &c : binary) {
      switch (state) {
          case q0 : if (c == '0') state = q0; else state = q1; break;
          case q1 : if (c == '0') state = q2; else state = q3; break;
          case q2 : if (c == '0') state = q4; else state = q5; break;
          case q3 : if (c == '0') state = q6; else state = q0; break;
          case q4 : if (c == '0') state = q1; else state = q2; break;
          case q5 : if (c == '0') state = q3; else state = q4; break;
          case q6 : if (c == '0') state = q5; else state = q6; break;
      }
  }

  return state == q0;
}
int main () {


    regex divseven ("^(0|111|100((1|00)0)*011|(101|100((1|00)0)*(1|00)1)(1((1|00)0)*(1|00)1)*(01|1((1|00)0)*011)|(110|100((1|00)0)*010|(101|100((1|00)0)*(1|00)1)(1((1|00)0)*(1|00)1)*(00|1((1|00)0)*010))(1|0(1((1|00)0)*(1|00)1)*(00|1((1|00)0)*010))*0(1((1|00)0)*(1|00)1)*(01|1((1|00)0)*011))+$");

    str = "1";
    if (regex_match (str, divseven)) {
        cout << " :: " << setw(8) << str;
        cout <<  "\n";
    }

}
