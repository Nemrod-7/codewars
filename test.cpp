#include <iostream>
#include <vector>

using namespace std;

enum type_e {zero, equals, unequal};

struct testing {

};

struct result : public testing {
    type_e type;

    result Is() {
        return *this;
    }
    result Not () {
        return *this;
    }

    template<typename T>
        result EqualTo(const T &x) {
            if (type == zero) type = equals;

            return *this;
        }
};

// result Is () { return {zero}; }

int main () {


  cout << "\nend\n";
}
