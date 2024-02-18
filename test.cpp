#include <iostream>
#include <vector>

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

// result Is () { return {zero}; }

int bas(int num, int i) {
    while (num % i == 1) {
        num /= i;
    }

    return num;
}
int smbase (const unsigned long int N) {

    if (bas(N, N-1) == 0) {
        return N-1;
    }
    
    for (int i = 2; i < N; i++) {
        int num = N;

        while (num % i == 1) {
            num /= i;
        }

        if (num == 0) {
            return i;
        }
    }

    return -1;
}
int main () {

    //fn res = Is().Not().EqualTo(3);
    // result expect = Is().Not().EqualTo(3.5);

    int number = 6587;
    int mul = 10;
    int temp = number % mul;
    vector<vector<int>> base(10, vector<int> (1,1));
    int sum = 0;

    while (temp < number) {
        temp = number % mul;
        int maxv = 0;

        for (int i = 0; i < 10; i++) {
            int num = base[i].back() * i;
            base[i].push_back(num);

            if (sum + num <= temp) {
                maxv = num;
            }
            cout << num << ' ';
        }

        sum += maxv;
        cout << '\n';

        mul *= 10;
    }




}
