#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>

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

ll dontGiveMeFive (ll start, ll end) {

    ll cnt = 0;

    do {
        //cout << end << " " << bitset<8> (end) << endl;
        if (epured_region (abs (end)) ) cnt++;
    //cout << end << " ";
    } while (end-->start);

    return cnt;
}
ll dontGiveMeFive2 (ll start, ll num) {

    ll cnt = 0;


    return cnt;
}

int main () {

  ll a = 0, b = 1000;
  int cnt = 0, x = 0;

  for (ll i = 1; i < 30; i++) {
      ll num = i * 100;
      cout << setw(4) << num << " " << dontGiveMeFive (a, num) << " " ;


      if (i < 5) {
          cout << i * 82 - (80 * 0) - (i - 1);
      } else if (i == 5) {
          cout << i * 82 - (80 * 0) - (i + 0);
      } else if (i < 15) {
          cout << i * 82 - (80 * 1) - (i + 0);
      } else if (i == 15) {
          cout << i * 82 - (80 * 1) - (i + 1);
      } else if (i < 25) {
          cout << i * 82 - (80 * 2) - (i + 1);
      } else if (i == 25) {
          cout << i * 82 - (80 * 2) - (i + 2);
      } else if (i < 30) {
          cout << i * 82 - (80 * 3) - (i + 2);
      }

      int end =  log10 (num) - 1;
      int fiv = 0;

      for (int k = 0; k <= end; k++) {
          fiv += pow (9, end - k) * pow (10, k);
      }
      cout  << " " << fiv;
      cout << "\n";
  }
  /*
  if (num >= 100 && num < 1000) {
      int cent = num / 100;
      cnt += 82 * cent - (cent - 1);
      if (num == 500) cnt -= 1;
      if (num > 599) cnt -= 81;
  }
  do {
      int dig = num % 10;
      if (x == 0) {
          cnt += dig > 5 ? dig : dig + 1;
      }
      num /= 10;
      x++;
  } while (num);

  */

  for (int num = 100; num < 900; num += 100) {

/*
    int end =  log10 (num) - 1;
    int fiv = 0;

    for (int k = 0; k <= end; k++) {
        fiv += pow (9, end - k) * pow (10, k);
    }

*/
    //cout << dontGiveMeFive (0, num) - num << "\n";

    //cout << floor (log (num)) << "\n";

  }

  //dontGiveMeFive (0,  1000);

/*
  Assert::That(dontGiveMeFive(-17, 9), Equals(24));
  Assert::That(dontGiveMeFive(1, 9), Equals(8));
  Assert::That(dontGiveMeFive(4, 17), Equals(12));
  Assert::That(dontGiveMeFive(-17, -4), Equals(12));


  Assert::That(dontGiveMeFive(984, 4304), Equals(2449));
  Assert::That(dontGiveMeFive(2313, 2317), Equals(4));
  Assert::That(dontGiveMeFive(-4045, 2575), Equals(4819));
  Assert::That(dontGiveMeFive(-4436, -1429), Equals(2194));
  /*
  */


  /*
    Assert::That(dontGiveMeFive(40076, 2151514229639903569), Equals(326131553237897713));

    Assert::That(dontGiveMeFive(-206981731, 2235756979031654521), Equals(340132150309630357));
    Assert::That(dontGiveMeFive(-2490228783604515625, -2490228782196537011), Equals(520812180));
    Assert::That(dontGiveMeFive(-9000000000000000000, 9000000000000000000), Equals(2401514164751985937));

    Assert::That(dontGiveMeFive(0, 1), Equals(2));
    Assert::That(dontGiveMeFive(5, 15), Equals(9));
    Assert::That(dontGiveMeFive(-5, 4), Equals(9));
    Assert::That(dontGiveMeFive(51, 60), Equals(1));

    */

    cout << "endl";


}
