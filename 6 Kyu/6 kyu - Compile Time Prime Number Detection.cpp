#include <iostream>
/*
template<int n, int i>
struct is_prime_ : std::conditional_t<(n % i) == 0, std::false_type, is_prime_<n, i - 1>>{};

template<int n>
struct is_prime_<n, 1> : std::true_type{};

template<int n>
struct is_prime_<n, 0> : std::false_type{};

template<unsigned n>
struct is_prime : is_prime_<n, n / 2>{};
*/


template<int N, int i> struct check :
    //static const bool value = (N % i) && check<N, i - 1>::value;
    //std::conditional_t<N % i == 0, std::false_type, check<N, i - 1>> {};



template<int N> struct check<N,1> : std::true_type {};
/*

template<unsigned p> struct is_prime {
    static const bool value =
    p == 2 || p == 3 || p == 5
        ? true
        : p % 2 == 0 || p % 3 == 0
            ? false
            : check<p, p / 2>::value;
};
*/
template<unsigned p> struct is_prime :
    std::conditional_t<p == 2 || p == 3 || p == 5,
        std::true_type,
        std::conditional_t<p % 2 == 0 || p % 3 == 0,
            std::false_type,
            check<p, p / 2>>> {};

template<> struct is_prime<1> : std::false_type {};

int main () {

    std::cout << is_prime<1000>::value;
}
