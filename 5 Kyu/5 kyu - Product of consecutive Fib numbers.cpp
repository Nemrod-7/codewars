#include <iostream>
#include <vector>

typedef unsigned long long ull;

class ProdFib {
    public:
        static std::vector<ull> productFib (ull prod) {
            ull first = 0, second = 1, temp;

            while (first * second < prod) {
                temp = first;
                first = second;
                second = temp + second;

                if (first * second == prod) return {first,second,1};
            }
            return {first,second, 0};
        }

};

int main () {

}
