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

vector<pair<uint64_t,uint64_t>> factors (uint64_t n) {
    vector<pair<uint64_t,uint64_t>> vs;
    vector<int> pr = {2,3,5,7};
    const int wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

    for (auto &i : pr) {
        if (n % i == 0) {
            int ex = 0;
            while (n % i == 0) {
                n /= i, ex++;
            }
            vs.push_back({i,ex});
        }
    }

    for (int i = 11, t = 2; i * i <= n; i += wheel[t], t = t == 47 ? 0 : t + 1) {
        if (n % i == 0) {
            int ex = 0;
            while (n % i == 0) {
                n /= i, ex++;
            }
            vs.push_back({i,ex});
        }
    }

    if (n > 1) vs.push_back({n,1});

    return vs;
}

vector<uint64_t> divisor (uint64_t n) {
    vector<uint64_t> divisor = {1};
    vector<pair<uint64_t,uint64_t>> fac = factors(n);

    for (auto &[pr,ex] : fac) {
        int mult = 1, end = divisor.size();

        for (int j = 0; j < ex; j++) {
            mult *= pr;

            for (int k = 0; k < end; k++) {
                divisor.push_back(divisor[k] * mult);
            }
        }
    }

    sort(divisor.begin(), divisor.end());
    return divisor;
}

int main () {

  uint64_t prime = 47; // period = 32
  std::vector<uint64_t> div = { 3,4,6,8,12,16,24,32,48,96};
  const int U[2][2] = {{1,1},{1,0}};
  const int I[2][2] = {{0,1},{0,1}};


  int f1 = 3;
  cout << f1 * U[0][0] * f1 * U[1][1] - f1 * U[0][1] * f1 * U[1][0];



    cout << "\nexit\n";
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
