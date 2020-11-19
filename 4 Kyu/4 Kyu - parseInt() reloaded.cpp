#include <iostream>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (auto actual, auto expression) {
            if (actual != expression)
                std::cout<<"actual : "<<actual<<" expected : "<<expression<<std::endl;
        }
};
auto Equals (auto entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
vector<vector<string>> base {
    {"zero","one","two","three","four","five","six","seven","eight","nine",
     "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
    {".",".","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"},
    {".","hundred"},
    {".","thousand"},
    {"million"}};

long parse_int (std::string number);

int main () {

    auto start = chrono::high_resolution_clock::now();

    Test();
    //cout << parse_int("two hundred and forty-six") << endl;

    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;
}

long parse_int (std::string number) {

    unsigned num = 0, sec = 0, index,mult, cardinal, shift;
    bool mill = false;
    char *str = strdup (number.c_str()), *token = strtok (str," ,.-");

    while (token != NULL) {

      for (index = 0; index < base.size(); ++index)
          for (cardinal = 0; cardinal < base[index].size(); ++cardinal)
              if (base[index][cardinal].compare(token) == 0) {
                    mult = index == 4 ? 6 : index;
                    if (mult == 3) mill = true;
                    shift = pow (10,mult);

                    if (mill == false)
                        num = (mult < 2) ? num + (cardinal * shift) : num * shift;
                    else
                        sec = (mult < 2) ? sec + (cardinal * shift) : sec * shift;

                }

        token = strtok (NULL," ,.-");
    }
    num = num + sec;

    return num;
}

void Test() {

  Assert::That(parse_int("one"), Equals(1));
  Assert::That(parse_int("twenty"), Equals(20));
  Assert::That(parse_int("two hundred and forty-six"), Equals(246));

  Assert::That(parse_int("one million"), Equals(1000000));
}
