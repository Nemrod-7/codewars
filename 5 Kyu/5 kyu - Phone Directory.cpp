#include <iostream>
#include <sstream>
#include <string>


void Test();

using namespace std;

class PhoneDir {
      private :
          static string getname (string &str) {
              int start = str.find ('<'), end = str.find('>');
              string os = str.substr (start + 1, end - start - 1);
              str.erase (start, os.size() + 2);
              return os;
          }
          static string clean (string str) {
              string os, allowed {"- ."};
              string::iterator it = str.begin();

              while (isalnum (*it) == false) it++;

              while (it != str.end()) {

                  if (isalnum(*it) || isblank (*it) || *it == '.' || *it == '-') {
                      os += *it;
                  }
                  if (isblank (*it) && isblank (*(it + 1))) it++;
                  if (*it == '_') os += ' ';
                  it++;
              }

              os.erase(os.find_last_not_of(' ') + 1);
              return os;
          }
      public:
        static string phone (const string& orgdr, string num) {

            stringstream iss (orgdr), oss;
            string line, addr, name;

            while (getline (iss, line)) {

                if (line.find (num) != string::npos) {

                    int start = line.find ('+'), last = line.find_first_not_of ("0123456789-", start + 1) - start + 1;
                    line.erase (start, last);

                    name = getname (line), addr = clean (line);
                    if (oss.str() != "") return "Error => Too many people: " + num;
                        oss << "Phone => " << num << ", Name => " << name << ", Address => " << addr;
                }
            }

            if (oss.str() == "") return "Error => Not found: " + num;
            return oss.str();
        }
};

int main () {

  Test();
  //cout << PhoneDir::phone ("1-908-512-2222");
}

void testequal (const string &a, const string &b) {
    if (a != b) {
        int size = max (a.size(), b.size());

        cout << "actual : " << a << endl;
        cout << "expect : " << b << endl;
        cout << endl;
        /*
        */
    }
}
void Test () {

  std::string dr = "/+1-541-754-3010 156 Alphand_St. <J Steeve>\n 133, Green, Rd. <E Kustur> NY-56423 ;+1-541-914-3010\n"
      "+1-541-984-3012 <P Reed> /PO Box 530; Pollocksville, NC-28573\n :+1-321-512-2222 <Paul Dive> Sequoia Alley PQ-67209\n"
      "+1-741-984-3090 <Peter Reedgrave> _Chicago\n :+1-921-333-2222 <Anna Stevens> Haramburu_Street AA-67209\n"
      "+1-111-544-8973 <Peter Pan> LA\n +1-921-512-2222 <Wilfrid Stevens> Wild Street AA-67209\n"
      "<Peter Gone> LA ?+1-121-544-8974 \n <R Steell> Quora Street AB-47209 +1-481-512-2222\n"
      "<Arthur Clarke> San Antonio $+1-121-504-8974 TT-45120\n <Ray Chandler> Teliman Pk. !+1-681-512-2222! AB-47209,\n"
      "<Sophia Loren> +1-421-674-8974 Bern TP-46017\n <Peter O'Brien> High Street +1-908-512-2222; CC-47209\n"
      "<Anastasia> +48-421-674-8974 Via Quirinal Roma\n <P Salinger> Main Street, +1-098-512-2222, Denver\n"
      "<C Powel> *+19-421-674-8974 Chateau des Fosses Strasbourg F-68000\n <Bernard Deltheil> +1-498-512-2222; Mount Av.  Eldorado\n"
      "+1-099-500-8000 <Peter Crush> Labrador Bd.\n +1-931-512-4855 <William Saurin> Bison Street CQ-23071\n"
      "<P Salinge> Main Street, +1-098-512-2222, Denve\n"
      "<P Salinge> Main Street, +1-098-512-2222, Denve\n"
      "/+5-541-754-3010 156 Alphandria_Street. <Jr Part>\n 1333, Green, Road <F Fulgur> NW-46423 ;+6-541-914-3010!\n"
      "+5-541-984-3012 <Peter Reeves> /PO Box 5300; Albertville, SC-28573\n :+5-321-512-2222 <Paulo Divino> Boulder Alley ZQ-87209\n"
      "+3-741-984-3090 <F Flanaghan> _Chicago Av.\n :+3-921-333-2222 <Roland Scorsini> Bellevue_Street DA-67209\n"
      "+8-111-544-8973 <Laurence Pantow> SA\n +8-921-512-2222 <Raymond Stevenson> Joly Street EE-67209\n"
      "<John Freeland> Mantow ?+2-121-544-8974 \n <Robert Mitch> Eleonore Street QB-87209 +2-481-512-2222?\n"
      "<Arthur Paternos> San Antonio $+7-121-504-8974 TT-45121\n <Ray Charles> Stevenson Pk. !+7-681-512-2222! CB-47209,\n"
      "<JP Gorce> +9-421-674-8974 New-Bern TP-16017\n <P McDon> Revolution Street +2-908-512-2222; PP-47209\n"
      "<Elizabeth Corber> +8-421-674-8974 Via Papa Roma\n <C Saborn> Main Street, +15-098-512-2222, Boulder\n"
      "<Colin Marshall> *+9-421-674-8974 Edinburgh UK\n <Bernard Povit> +3-498-512-2222; Hill Av.  Cameron\n"
      "+12-099-500-8000 <Pete Highman> Ontario Bd.\n +8-931-512-4855 <W Mount> Oxford Street CQ-23071\n"
      "<Donald Drinkaw> Moon Street, +3-098-512-2222, Peterville\n";


      cout << PhoneDir::phone (dr, "1-541-754-3010");

      /*
      Expected: equal to "Phone => 1-541-754-3010, Name => J Steeve, Address => 156 Alphand St."
Actual: "Phone => 1-541-754-3010, Name => J Steeve, Address => 156 AlphandSt"


Random_Tests
Expect: "Phone => 3-741-984-3090, Name => F Flanaghan, Address => Chicago Av."
Actual: "Phone => 3-741-984-3090, Name => F Flanaghan, Address => Chicago Av"


      /*
  testequal(PhoneDir::phone(dr, "48-421-674-8974"), "Phone => 48-421-674-8974, Name => Anastasia, Address => Via Quirinal Roma");
       testequal(PhoneDir::phone(dr, "19-421-674-8974"), "Phone => 19-421-674-8974, Name => C Powel, Address => Chateau des Fosses Strasbourg F-68000");
       testequal(PhoneDir::phone(dr, "1-921-512-2222"), "Phone => 1-921-512-2222, Name => Wilfrid Stevens, Address => Wild Street AA-67209");
       testequal(PhoneDir::phone(dr, "1-481-512-2222"), "Phone => 1-481-512-2222, Name => R Steell, Address => Quora Street AB-47209");
       testequal(PhoneDir::phone(dr, "1-098-512-2222"), "Error => Too many people: 1-098-512-2222");
       testequal(PhoneDir::phone(dr, "5-555-555-5555"), "Error => Not found: 5-555-555-5555");
       /*
  */
}

string replaceall (string &str, string from, string to) {
  size_t start_pos = 0;

  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace (start_pos, from.length(), to);
    start_pos += to.length();
  }

  return str;
}
