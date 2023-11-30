
#include <iostream>

int main () {

    std::string src = "<~this is a test~>";

    //int fst = src.find("<~");
    //int scd = src.find("~>");

    int pow[5] = {1, 85, 7225, 614125, 52200625};
    int pw = 1;

    for (int i = 0; i < 5; i++) {
        std::cout << pw << ' ';
        pw *= 85;
    }

    //std::cout << scd - fst - 2;
    //std::cout << src.substr(fst + 2, scd - fst - 2 );

}
