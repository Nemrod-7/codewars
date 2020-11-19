#include <iostream>

using namespace std;


template <int m, int n>
    struct ackermann
        : std::integral_constant <int, ackermann<m - 1, ackermann<m, n - 1>::value>::value> {};

template<> struct ackermann<0,0> : std::integral_constant<int, 1> {};
template<int n> struct ackermann<0,n> : std::integral_constant <int, n + 1> {};
template<int m> struct ackermann<m,0> : ackermann<m - 1, 1> {};

int main () {

    //cout << ackermann<2, 2>::value << endl; // 7

}
