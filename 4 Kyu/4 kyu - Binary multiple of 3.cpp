#include <iostream>
#include <fstream>
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

  //ofstream os ("binary.csv");

    // ^(0|(1(01*0)*10*)+)$
    string input ("(1(01*0)*10*)*");// = "(1(01*0)*10*)*";

    //while (cin >> input) {

        //if (input == "exit") break;

        regex eng (input);

        for (int i = 0; i < 100; i++) {

            bitset<32> num (i);
            string str = clean(num.to_string());

            //if (i % 3 == 0) {
            if (regex_match (str, eng)) {

              cout << setw(3) << i << " ";
              cout << setw(8) << str;
              cout << " " << (i % 3 == 0 ? "true  " : "false ");

              cout <<  "\n";
            }
        }
    //}
    //os.close();
}
