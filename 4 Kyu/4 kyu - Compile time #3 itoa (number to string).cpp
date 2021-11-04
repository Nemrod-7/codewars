#include <iostream>

using namespace std;
using integer = unsigned long long;

template<integer x> struct dig :
    std::integral_constant<char, x < 10 ? x + '0' : x + 'a' - 10> {};

template <integer X, integer Base = 10, char... C> struct mk_str :
    mk_str<X / Base, Base, dig<X % Base>::value, C...> {};

template <integer Base, char... C> struct mk_str<0, Base, C...> {
    static constexpr char value[sizeof ...(C) + 1] = {(C)..., 0};
};

template <integer Base, char... C> constexpr char mk_str<0, Base, C...>::value[];

template <integer X, integer Base = 10>  struct itoax : mk_str<X, Base> {};

template <integer Base> struct itoax<0, Base> {
    static constexpr char value[2] {'0'};
};

int main () {

    //cout << static_cast<char> (itoax<1234>::value);
    //cout << itoax<1234>::value;
    //mk_str<1234>();

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
