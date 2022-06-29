#include <iostream>
#include <regex>

using namespace std;

string number_format (long long n) {
    regex re ("(-?[0-9]{1,3})(?=([0-9]{3})+$)");// R"((-?\d{1,3})(?=(\d{3})+$))"
    return regex_replace (to_string (n), re, "$&.");;
}

int main () {

    cout << number_format (-42090245);
}
