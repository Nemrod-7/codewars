#include <iostream>

using namespace std;

/*
template <int x> struct factorial {
    enum : uint64_t {value = x * factorial<x - 1>::value};
    //static const uint64_t value = x * factorial<x - 1>::value;
};

template <> struct factorial<1> {enum : uint64_t {value = 1};};
*/

template<int T> struct factorial :
    integral_constant<uint64_t, T * factorial <T - 1>::value> {};

template<> struct factorial<1> : integral_constant<uint64_t, 1> {};

int main () {

    cout << factorial<20>::value;

}
