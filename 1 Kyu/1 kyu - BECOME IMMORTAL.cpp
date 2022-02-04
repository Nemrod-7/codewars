#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (const int64_t &a, const int64_t &b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
};
int64_t Equals (const int64_t &entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
int64_t elder_age (int64_t m, int64_t n, int64_t l, int64_t t) {

    int64_t board[m * n] = {0}, cnt = 0;

    for (int64_t y = 0; y < n; y++) {
        for (int64_t x = 0; x < m; x++) {

            int64_t num = (x ^ y) - l;
            if (num > 0) cnt += num;
        }

    }

    while (cnt - t > 0) {
        cnt -= t;
    }

    return cnt;
}
int64_t elder_age2 (int64_t m, int64_t n, int64_t l, int64_t t) {

    int64_t cnt = 0;

    for (int64_t y = 0; y < n; y++) {
        for (int64_t x = 0; x < m; x++) {

            int64_t num = (x ^ y);
            cnt += num;

            cout << num << " ";

        }
        cout << endl;
    }
    cout << cnt << endl;

    return cnt;
}
int64_t elder_age3 (int64_t m, int64_t n, int64_t l, int64_t t) {

    int64_t cnt = (m * (m - 1) / 2) * n;
    /*
    for (int64_t y = 0; y < n; y++) {
        for (int64_t x = 0; x < m; x++) {

            int64_t num = (x ^ y);
            cnt += num;

            cout << num << " ";

        }
        cout << endl;
    }
    */
    cout << cnt << endl;
    return cnt;
}
int main () {

  auto start = std::chrono::high_resolution_clock::now();
  //elder_age2(28827050410, 35165045587, 7109602, 13719506);
  elder_age2 (2,2,0,100007);
  elder_age2 (3,3,0,100007);
  elder_age2 (4,4,0,100007);
  elder_age2 (5,5,0,100007);
  elder_age2 (6,6,0,100007);
  elder_age2 (7,7,0,100007);
  elder_age2 (8,8,0,100007);
  /*
  */


  //int m = 6, n = 6;

  //Assert::That(elder_age2(8, 5, 1, 100), Equals(5));

  /*
  Assert::That(elder_age2(8,8,0,100007), Equals(224));
  Assert::That(elder_age2(25,31,0,100007), Equals(11925));
  Assert::That(elder_age2(5,45,3,1000007), Equals(4323));
  Assert::That(elder_age(31,39,7,2345), Equals(1586));
  Assert::That(elder_age(545,435,342,1000007), Equals(808451));
  Assert::That (elder_age(28827050410, 35165045587, 7109602, 13719506), Equals(5456283));
  */


  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
