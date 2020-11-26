#include <iostream>
#include <map>

using namespace std;

void test();
/*
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
1 1 2 3 3 4 5 5 6  6  6  8  8  8 10  9 10 11 11 12 12 12 12

u[7] = 5
u[6] = 4

8 - u[7] = 3
8 - u[6] = 4

u[8] =       2     +       3     = 5
u[8] =     u[3]    +    u[4]     = 5
u[8] = u[8 - u[7]] + u[8 - u[6]] = 5

=> u[k] = u[k - u[k - 1]] + u[k - u[k - 2]]

:: u[13] = u[13 - u[13 - 1]] + u[13 - u[13 - 2]]
:: u[13] = u[13 - u[12]] + u[13 - u[11]]
:: u[13] = u[13 - 8] + u[13 - 6]
:: u[13] = u[5] + u[7]
:: u[13] = 3 + 5
:: u[13] = 8
*/

std::map<int,int> u {{0,1},{1,1},{2,1}};

class Fibkind {
    public :
        static int lengthSupUK (int n, int k) {
            int cnt = 0;

            for (size_t i = 1; i <= n; i++) {
                if (i >= u.size())
                    u[i] = u[i - u[i - 1]] + u[i - u[i - 2]];

                if (u[i] >= k) cnt++;
            }
            return cnt;
        }
        static int comp (int n) {
            int cnt = 0;

            for (size_t i = 2; i <= n; ++i) {
              if (i >= u.size())
                    u[i] = u[i - u[i - 1]] + u[i - u[i - 2]];

                if (u[i] < u[i - 1]) cnt++;
            }
            return cnt;
        };
};

int main () {

    //cout << Fibkind::U (90000) << ' '; // => 44337
    //cout << Fibkind::U (900) << ' '; // => 455


}
