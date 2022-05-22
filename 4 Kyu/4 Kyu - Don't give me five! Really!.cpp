#include <iostream>
#include <iomanip>
#include <cmath>

#include <assert.h>

using namespace std;
#define ll long long
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (const ll &a, const ll &b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
};
ll Equals (const ll &entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
bool epured_region (ll x) {

    do {
        if (x % 10 == 5) return false;
    } while (x /= 10);

    return true;
}
ll dontGiveMeFive2 (ll start, ll end) {

    ll cnt = 0;

    do {
        if (epured_region (abs (end)) ) cnt++;
    } while (end-->start);

    return cnt;
}

ll count (ll num) { // count from zero to num

    const int size =  log10 (num);
    ll cnt = 1;             // dont forget to add zero !

    for (int k = 0; k <= size; k++) {
        int dig =  num / static_cast<int> (powl (10, k)) % 10;
        int ex = dig;

        if (dig > 4) ex -= 1;

        if (k > 0 && dig == 5) {
            cnt = powl (9, k) * (ex + 1);
        } else {
            cnt += powl (9, k) * ex;
        }
    }

    return cnt;
}
ll dontGiveMeFive (ll start, ll end) {

    ll cnt = 0;

    if (end > 0) {
        cnt += count (end);
    } else {
        cnt -= count (abs (end)) - 2;
    }

    if (start > 0) {
        cnt -= count (start);
    } else {

        cnt += count (abs(start)) - 2;
    }

    return cnt + 1;

    return count (end - start);
}

int main () {


    int num = 28574;

    /*
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            ll a = dontGiveMeFive (i,  j), b = dontGiveMeFive2 (i, j);

            if (a != b) {
                cout << i << " -> " << j;
                cout << " exp : " << a << " got " << b << "\n";
            }
        }
    }
    */

  Assert::That(dontGiveMeFive(5, 15), Equals(9));
  Assert::That(dontGiveMeFive(51, 60), Equals(1));
  
  /*
  Assert::That(dontGiveMeFive(1, 9), Equals(8));
  Assert::That(dontGiveMeFive(4, 17), Equals(12));
  Assert::That(dontGiveMeFive(-17, 9), Equals(24));
  Assert::That(dontGiveMeFive(-17, -4), Equals(12));

  Assert::That(dontGiveMeFive(984, 4304), Equals(2449));
  Assert::That(dontGiveMeFive(2313, 2317), Equals(4));
  Assert::That(dontGiveMeFive(-4045, 2575), Equals(4819));
  Assert::That(dontGiveMeFive(-4436, -1429), Equals(2194));

  Assert::That(dontGiveMeFive(-5, 4), Equals(9));
  Assert::That(dontGiveMeFive(0, 1), Equals(2));
  /*

  Assert::That(dontGiveMeFive(40076, 2151514229639903569), Equals(326131553237897713));
  /*

    Assert::That(dontGiveMeFive(-206981731, 2235756979031654521), Equals(340132150309630357));
    Assert::That(dontGiveMeFive(-2490228783604515625, -2490228782196537011), Equals(520812180));
    Assert::That(dontGiveMeFive(-9000000000000000000, 9000000000000000000), Equals(2401514164751985937));

    */

    cout << "endl";


}
