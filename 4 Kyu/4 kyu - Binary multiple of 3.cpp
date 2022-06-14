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
bool checkphone (string str) {
      string phone = "(0|\33)[1-9]([0-9]{2}){4}";
      // begins with 0 or 33,
      regex eng (phone);

      return regex_match (str, eng);
}
int main () {

  const std::string multiple_of_3_regex = "(0|1(01*0)*1)*";
  string expr  = "iyinw = (9.46+8.56+(7.55)/7.56/9.51/7.56/ -5.27-8.63*7.54/12)))))";
  //                  numbers         /

  const regex number ("(-?[0-9]+(.[0-9]+)?)"), oper ("(\\+|\\-|\\/|\\*|=)"), other ("(\\(|\\))");
  const regex pattern ("([A-Z-a-z]+)|(-?[0-9]+(.[0-9]+)?)|(\\+|\\-|\\/|\\*|=)|(\\(|\\))");

  auto it = sregex_iterator (expr.begin(), expr.end(), pattern), end = sregex_iterator();
  bool running = true;

  while (running) {
      string tok = it->str();

      if (regex_match (tok, number)) {
          cout << "num : [" << tok << "]";
      } else if (regex_match (tok, oper)) {
          cout << "ope : [" << tok << "]";
      } else if (regex_match (tok, other)) {
          cout << "oth : [" << tok << "]";
      } else {
        cout << "oth : [" << tok << "]";
      }

      if (it == end) running = false;
      cout << endl;
      it++;
  }
  /*
    // ^(0|(1(01*0)*10*)+)$
    string input;// = "(1(01*0)*10*)*";

    /*
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
    */
}
