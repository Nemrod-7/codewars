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


    int width = 10, height = 4;

    const std::vector<std::pair<int,int>> corner = {{1,1}, {1,height-2}, {width-2,1}, {width-2,height-2}};
    const std::vector<std::pair<int,int>> gbh = {{0,0}, {0,height-1}, {width-1,0}, {width-1,height-1}};

    for ( int i = 0; i < 4; i++) {
        auto [px,py] = corner[i];

        cout << endl;
    }


    // fn res = Is().Not().EqualTo(3);
    // result expect = Is().Not().EqualTo(3.5);


}
