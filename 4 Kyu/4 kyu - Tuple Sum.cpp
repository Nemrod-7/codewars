#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

template <typename... Ts> double tuple_sum (const std::tuple<Ts...>& tpl);

template <size_t I = 0, typename... Ts> constexpr void printTuple (tuple<Ts...> tup) {
    if constexpr(I == sizeof...(Ts)) { // If we have iterated through all elements
        return; // Last case, if nothing is left to iterate, then exit the function
    } else {
        //cout << get<I>(tup) << " "; // Print the tuple and go to next element
        printTuple<I + 1>(tup); // Going for next element.
    }
}
int main () {

    tuple tup = std::make_tuple (3.14, 42, 'a', "Hello", "World"s, std::vector<int>{1, 2, 3});

    //std::apply([](auto&&... args) {((std::cout << args << '\n'), ...);}, tup);
}
