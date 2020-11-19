#include <iostream>
/////////////////////////////mid & pow func////////////////////////
template<size_t a,size_t b> struct mid : std::integral_constant<size_t, (a + b) / 2> {};
template<size_t a> struct pow : std::integral_constant<size_t,a * a> {};
///////////////////////////////sqrt func//////////////////////////
template<size_t res, size_t l = 1, size_t r = res> struct sqroot_;
template<size_t res, size_t r> struct sqroot_<res, r, r> : std::integral_constant<size_t, r> {};

template<size_t res, size_t l, size_t r> struct sqroot_ :
    std::integral_constant<size_t,
    sqroot_<res, (pow<mid<r, l>::value>::value >= res ? l : mid<r, l>::value + 1),
                 (pow<mid<r, l>::value>::value >= res ? mid<r, l>::value : r)>::value> {};

template<size_t n> struct sqroot : std::integral_constant<size_t, sqroot_<n,1,n>::value> {};
//////////////////////////////////////////////////////////////////
/*
template<int N, int i> struct check :
    std::conditional_t<i <= 4,
        std::true_type,
            std::conditional_t<(N % i == 0 || N % (i + 2) == 0),
                std::false_type,
                check<N, i - 6>>> {};

template<unsigned p> struct is_prime :
    std::conditional_t<p == 2 || p == 3 || p == 5,
        std::true_type,
        std::conditional_t<p % 2 == 0 || p % 3 == 0,
            std::false_type,
            check<p, sqroot<p>::value>>> {};
*/
/*
template<unsigned p> struct is_prime :
    std::conditional_t<p == 2 || p == 3 || p == 5 || p == 7 || p == 11,
        std::true_type,
        std::conditional_t<p % 2 == 0 || p % 3 == 0 || p % 5 == 0 || p % 7 == 0 || p % 11 == 0,
            std::false_type,
            std::true_type>> {};

template<> struct is_prime<1> : std::false_type {};
*/
template<int n, int i> struct is_prime_ :
    std::conditional_t<n >= i * i,
        std::conditional_t<n % i == 0, std::false_type, is_prime_<n, i + 1>>,
        std::true_type> {};

template<> struct is_prime_<0, 2> : std::false_type{};
template<> struct is_prime_<1, 2> : std::false_type{};
template<int n> struct is_prime : is_prime_<n, 2>{};

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
/*
template<int N, int i> struct check :
    //static const bool value = (N % i) && check<N, i - 1>::value;
    std::conditional_t<N % i == 0, std::false_type, check<N, i - 1>> {};

template<unsigned p> struct is_prime {
  static const bool value =
  p == 2 || p == 3 || p == 5
  ? true
  : p % 2 == 0 || p % 3 == 0
  ? false
  : check<p, p / 2>::value;
};
*/

int main () {

        /*
        */
        std::cout << is_prime<1>::value;
        std::cout << is_prime<2>::value;
        std::cout << is_prime<3>::value;
        std::cout << is_prime<4>::value;
        std::cout << is_prime<5>::value;
        std::cout << is_prime<6>::value;
        std::cout <<" ";
        std::cout << is_prime<7>::value;
        std::cout << is_prime<11>::value;
                std::cout << is_prime<12>::value;
        std::cout << is_prime<13>::value;
                std::cout << is_prime<14>::value;
        std::cout << is_prime<63727>::value;

}
/*
bool is_prime(int num) {

    if (num <= 3) return num > 1;
    if (num % 2 == 0 || num % 3 == 0) return false;

    const unsigned end = sqrt (num);

    for (unsigned i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}*/
