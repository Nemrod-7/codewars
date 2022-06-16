#include <iostream>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <bitset>

using namespace std;
string clean (string str) {
    str.erase (0, str.find_first_not_of ('0'));
    //reverse(str.begin(), str.end());

    return str;
}

int main () {

    // ^(0|(1(01*0)*10*)+)$
    string input;// = "(1(01*0)*10*)*";

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
    /*
    */
}



enum {q0,q1,q2,q3,q4,q5,q6};

bool read_commands(const string& commands) {
  int state = q1;

  for (auto &c : commands)
  switch (state) {
    case q1 : if (c == '1') state = q2; break;
    case q2 : if (c == '0') state = q3; break;
    case q3 : state = q2; break;
  }

  return state == q2;
}

bool isinv (char c) { return c != '0' && c != '1'; }

bool ismod7 (string str) {

  if (str == "") return false;
  if (str == "0") return true;

  int A = 0;

  for (auto &c : str) {
    if (isinv (c)) return false;
    A = 2 * A + (c - '0');
  }
  cout << " " << A << " :: ";

  return A % 7 == 0;
}
