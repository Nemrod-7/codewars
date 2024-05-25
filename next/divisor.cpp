#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

template <class T> std::ostream &operator << (std::ostream &stream, const std::vector<T> &obj) {
    for (auto &it : obj) {
        stream << it << " ";
    }
    return stream;
}
template<class T> std::ostream &operator << (std::ostream &stream, const std::pair<T,T> &obj) {
    stream << "[" << obj.first << "," << obj.second << "]";
    return stream;
}

int main () {

    uint64_t num = 181; // period = 32

    vector<int> d1 = {2,5,7};
    vector<int> d2 = {2,3,5,7,16};

    for (auto &d : d2) {
        if (find(d1.begin(), d1.end(), d) == d1.end()) {
            cout << d << " ";
        }
    }

    cout << "\nexit\n";
}
