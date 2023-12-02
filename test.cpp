#include <iostream>

using namespace std;


enum type_e {zero, equals, unequal};

struct testing {

};

struct result : public testing {
    type_e type;

    result Not () {
        return *this;
    }

    template<typename T> 
        result EqualTo(const T &x) {
            if (type == zero) type = equals;

            return *this;
        }
};




result Is () { return {zero}; }



int main () {

    //fn res = Is().Not().EqualTo(3);
    result expect = Is().Not().EqualTo(3.5);


}
