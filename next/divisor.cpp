#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

int main () {

    int p = 11;

    if ((((p-1) % 5) == 0)) {
        cout << "p-1";
    }
    if (((p+1) % 5) == 0) {
        cout << "p+1";
    }


    cout << "\n";
}

////////////////////////////////////////////////////////////////////////////////
// void divisors2 (const vector<factor> &fac) {
//     vector<int> array;
//     int factors_count = 1;
//     for (int i = 0; i < fac.size(); ++i) {
//         factors_count *= 1 + fac[i].second;
//     }
//     array.resize(factors_count);
//
//     array[0] = 1;
//     int count = 1;
//
//     for (int index = 0; index < fac.size(); ++index) {
//         const int count_so_far = count;
//         auto [prime,exponent] = fac[index];
//         int multiplier = 1;
//
//         for (int j = 0; j < exponent; ++j) {
//             multiplier *= prime;
//
//             for (int i = 0; i < count_so_far; ++i) {
//                 array[count++] = array[i] * multiplier;
//             }
//         }
//     }
//
//     for (int i = 0; i < array.size(); i++) {
//         // cout << array[i] << " ";
//     }
// }
