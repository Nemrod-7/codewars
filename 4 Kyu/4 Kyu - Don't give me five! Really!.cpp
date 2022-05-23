#include <iostream>

#include <chrono>

using namespace std;
#define ll long long
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (const ll &a, const ll &b) {
            if (a != b) {
                std::cout << "actual : " << a << " expected : " << b;
                std::cout << std::endl;
            }
        }
};
ll Equals (const ll &entry) { return entry;}
void Test ();

class Timer {
    //using time = chrono::steady_clock;
    private :
        chrono::steady_clock::time_point alpha, end;
        chrono::duration<double> elapsed;
        uint64_t cycle;

        void update () {
            end = chrono::steady_clock::now ();
            elapsed = end - alpha;
        }

    public :

        Timer() {
            alpha = chrono::steady_clock::now ();
            cycle = 0;
        }
        void start () { alpha = chrono::steady_clock::now ();}
        void stop () { update();}
        void get_duration () {
            std::cout << "\nDuration "
                      <<fixed<< elapsed.count()
                      << " ms" << std::endl;
        }
        bool running (double total) {
            update();
            cycle++;
            if (elapsed.count() < total) return true;
            cout << "cycles :: " ;

            string num = to_string (cycle);
            int size = num.size(), index = 0;

            while (size-->0) {
                cout << num[index++];
                if (size % 3 == 0) cout << ' ';
            }
            cout << endl;
            return false;
        }
};
////////////////////////////////////////////////////////////////////////////////

bool epured_region (ll x) {

    do {
        if (x % 10 == 5) return false;
    } while (x /= 10);

    return true;
}
ll count (ll num) {

    const ll pow9[20]  {1,9,81,729,6561,59049,531441,4782969,43046721,387420489,
    3486784401,31381059609,282429536481,2541865828329,22876792454961,205891132094649,
    1853020188851841,16677181699666569,150094635296999121,1350851717672992089};

    ll cnt = 1, dig, ex;
    int k = 0;

    do {

        dig = num % 10, ex = dig > 4 ? dig - 1 : dig;

        if (k > 0 && dig == 5) {
            cnt = pow9[k] * dig;
        } else {
            cnt += pow9[k] * ex;
        }
        k++;

    } while (num /= 10);

    return cnt;
}
ll dontGiveMeFive (ll start, ll end) {

    ll cnt = 0;

    if (start >= 0 && end >= 0) {
        cnt = count (end) - count (start) + 1;
        if (!epured_region (start)) cnt--;

    } else if (start < 0 && end < 0) {
        cnt = count (labs (start)) - count (labs (end)) + 1;
        if (!epured_region (labs (end))) cnt--;

    } else {
        cnt = count (end) + count (labs(start)) - 1;
    }

    return cnt ;
}
ll count2 (ll num) {

    const ll pow9[20]  {1,9,81,729,6561,59049,531441,4782969,43046721,387420489,
    3486784401,31381059609,282429536481,2541865828329,22876792454961,205891132094649,
    1853020188851841,16677181699666569,150094635296999121,1350851717672992089};

    ll cnt = 1, dig, ex;
    int k = 0;

    do {
        dig = num % 10, ex = dig > 4 ? dig - 1 : dig;

        if (k > 0 && dig == 5) {
            cnt = pow9[k] * dig;
        } else {
            cnt += pow9[k] * ex;
        }
        k++;

    } while (num /= 10);

    return cnt;
}
int main () {
    Timer clock;

    while (clock.running (2)) {
        count2 (2235756979031654521);
    }

    Assert::That(dontGiveMeFive(4, 17), Equals(12));
    Assert::That(dontGiveMeFive(1, 9), Equals(8));
    Assert::That(dontGiveMeFive(984, 4304), Equals(2449));
    Assert::That(dontGiveMeFive(2313, 2317), Equals(4));
    Assert::That(dontGiveMeFive(0, 1), Equals(2));
    Assert::That(dontGiveMeFive(51, 60), Equals(1));
    Assert::That(dontGiveMeFive(5, 15), Equals(9));
    Assert::That(dontGiveMeFive(40076, 2151514229639903569), Equals(326131553237897713));

    Assert::That(dontGiveMeFive(-9000000000000000000, 9000000000000000000), Equals(2401514164751985937));
    Assert::That(dontGiveMeFive(-17, 9), Equals(24));
    Assert::That(dontGiveMeFive(-17, -4), Equals(12));
    Assert::That(dontGiveMeFive(-4436, -1429), Equals(2194));
    Assert::That(dontGiveMeFive(-5, 4), Equals(9));
    Assert::That(dontGiveMeFive(-206981731, 2235756979031654521), Equals(340132150309630357));
    Assert::That(dontGiveMeFive(-4045, 2575), Equals(4819));

    Assert::That(dontGiveMeFive(-2490228783604515625, -2490228782196537011), Equals(520812180));
    /*
    /*
*/

    clock.stop();
    clock.get_duration();
}

ll dontGiveMeFive2 (ll start, ll end) {

    ll cnt = 0;

    do {
        if (epured_region (std::abs (end)) ) cnt++;
    } while (end-->start);

    return cnt;
  }
