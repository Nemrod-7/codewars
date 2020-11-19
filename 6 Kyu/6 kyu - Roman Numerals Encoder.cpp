#include <iostream>
#include <sstream>
#include <vector>

string solution (int number);
using namespace std;
int main () {
    solution (1990);

    return 0;
}

std::string solution (int number) {

    int index = 0;
    vector<int>           dec{1000,900,500,400, 100, 90, 50, 40, 10,  9,  5,   4,  1};
    vector<string> roman{"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    ostringstream os;

    while (number) {
        if (number >= dec[index]) {

            cout  << number << " " << roman[index] << endl;
            os << roman[index];
            number -= dec[index];
        }
        else
          index++;
    }

    return os.str();
}
