#include <iostream>

using namespace std;
using integer = unsigned long long;

template<char... c> struct make_string {
    static constexpr char value[sizeof...(c) + 1] = {c..., '\0'};
};

template<integer X, integer Base = 10> struct itoax {
    static const integer value = X % Base + '0';
};

template<integer X, integer Base = 10> struct mk_str {
    static const char value = X % Base + '0';

    mk_str () {
        cout << value;
        mk_str<X / 10>();
    }
    /*
    */
};
template<> struct mk_str<0> {};

int main () {

    //cout << static_cast<char> (itoax<1234>::value);
    //cout << itoax<1234>::value;
    //mk_str<1234>();
    cout << make_string<'h', 'e'>::value[0];
}

/*
string itoa (int num, int base) {
    string os;

    do {
        os += num % base + '0';
        num /= base;
    } while (num > 0);


    return os;
}
void reverse (char *src) {

  int tmp, i = 0, j = length (src) - 1;

  while (i < j) {
    tmp = src[i];
    src[i] = src[j];
    src[j] = tmp;
    i++, j--;
  }
}
*/
void Test () {
  /*
    Assert::That(itoax<10,  2>::value, Equals("1010"));
    Assert::That(itoax<10,  8>::value, Equals(  "12"));
    Assert::That(itoax<10, 16>::value, Equals(   "a"));
    */
}
