#include <iostream>
/*
template <char... c> struct make_string {
    //std::integral_constant<size_t, sizeof...(c) + 1>
    static constexpr size_t size = sizeof...(c) + 1;
    static constexpr char value[size] = {c..., '\0'};
};
*/
template <char... c> struct make_string {
    static constexpr char value[sizeof...(c) + 1] = {c..., '\0'};
};

int main () {

    using Hi = make_string<'H','i'>;

    std::cout << Hi::value[0];
    std::cout << Hi::value[1];
    std::cout << Hi::value[2];
}
