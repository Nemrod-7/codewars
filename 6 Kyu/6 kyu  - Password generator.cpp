#include <iostream>
#include <string>
#include <random>
using namespace std;

std::string password_gen() {
       random_device rd;
    mt19937 generate (rd());
    uniform_int_distribution<> distrib (48, 122); // 33 - 122
    char nu;
    string os;
    size_t size = max (8, static_cast<int> (rd() % 20));

    while (os.size() < size) {
        nu = distrib (rd);
        if (isalnum (nu))
            os += nu;
    }

    if (!any_of(os.begin(), os.end(), ::islower)) os += rd() % 24 + 'a';
    if (!any_of(os.begin(), os.end(), ::isupper)) os += rd() % 24 + 'A';
    if (!any_of(os.begin(), os.end(), ::isdigit)) os += rd() % 9 + '0';
    return os;

}
