#include <iostream>
#include <vector>

using namespace std;
using i64 = long int;

int ipow(int base, int exp) {
    int power = 1;

    while (exp) {
        if (exp &1)
            power *= base;
        exp >>= 1;

        base *= base;
    }

    return power;
}
vector<int> factors (int n) {
		vector<int> vs;
		vector<int> pr = {2,3,5,7};
		const int wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

		for (auto &i : pr) {
				if (n % i == 0) {
						while (n % i == 0) {
								n /= i;
						}
						vs.push_back(i);
				}
		}

		for (int i = 11, t = 2; i * i <= n; i += wheel[t], t = t == 47 ? 0 : t + 1) {
				if (n % i == 0) {
						while (n % i == 0) {
								n /= i;
						}
						vs.push_back(i);
				}
		}

		if (n > 1) vs.push_back(n);
		return vs;
}
int gcd (int a, int b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}
int lcm (int a, int b) {
    return (a / gcd (a,b)) * b;
}

int pisano_period (int n) { // OEIS A001175
    if (n == 1) return 1;

    int cycle = 1;
    vector<int> fac = factors(n);

    if (fac.size() > 1) {
        //A formulae to calculate the pisano period for integer powers k of prime numbers p is: pisano(p^k) == p^(k-1) * pisano (p)

        for (int i = 0; i < fac.size(); i++) {
            int p = fac[i];
            int an = ipow(p, i - 1) * pisano_period(p);
            cycle = lcm(cycle,an)  ;
        }

    } else {
        int a1 = 1, a0 = 1, tmp = 1;

        while (a0 != 0 || a1 != 1) {
            cycle++;
            tmp = (a0 + a1) % n, a0 = a1, a1 = tmp;
        }
    }


    return cycle;
}

int pisano1 (int m) {

    if (m == 1) return 1;
    int cycle = 1;

    return cycle  ;
}

int main () {
    const vector<int> pi = {1, 1, 3, 8, 6, 20, 24, 16, 12, 24, 60, 10, 24, 28, 48, 40, 24, 36, 24, 18, 60, 16, 30, 48, 24, 100, 84, 72, 48, 14, 120, 30, 48, 40, 36, 80, 24, 76, 18, 56, 60, 40, 48, 88, 30, 120, 48, 32, 24, 112, 300, 72, 84, 108, 72, 20, 48, 72, 42, 58, 120, 60, 30, 48, 96, 140, 120, 136};

    const vector end = {0,1};
    int a1 = 1, an = 1, a0 = 0;
    //ofstream ofs ("pisano", ios::app );

    for (int n = 1; n < pi.size(); n++) {
        int cycle = pisano_period (n);

        if (cycle != pi[n]) {
            cout << "err : " << n << " " << cycle;
            cout << endl;
        }

    }

}

void test () {

    /*
    {
        Assert::That(pisano_period(5), Equals(20));
        Assert::That(pisano_period(47), Equals(32));
        Assert::That(pisano_period(20), Equals(60));
        Assert::That(pisano_period(101), Equals(50));
    }

    {
        Assert::That(pisano_period(4253), Equals(8508));
        Assert::That(pisano_period(19661), Equals(9830));
        Assert::That(pisano_period(14207), Equals(28416));
        Assert::That(pisano_period(761), Equals(380));
    }

    {
        Assert::That(pisano_period(440119), Equals(444496));
        Assert::That(pisano_period(1672377), Equals(160720));
        Assert::That(pisano_period(4020378), Equals(483720));
        Assert::That(pisano_period(14451817), Equals(1804224));
    }

    {
        Assert::That(pisano_period(1818176898), Equals(1408320));
        Assert::That(pisano_period(2438389198053), Equals(10624179384));
    }

    */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool is_prime (i64 p) {
    if (p < 2) return false;
    if (0 == p % 2) return 2 == p;
    if (0 == p % 3) return 3 == p;

    i64 d = 5;

    while (d * d <= p) {
        if (0 == p % d) return false;
        d += 2;
        if (0 == p % d) return false;
        d += 4;
    }
    return true;
}
i64 period(const std::vector<int> &v) { // detect cycle in a generic sequence

    for (i64 i = 1; i < v.size(); i++) {
        i64 j = 0;

        while (j < v.size() - i && v[j] == v[j + i]) ++j;

        if (j + i == v.size()) {
            return i;
        }
    }

    return 0;
}
vector<int> fibonacci (int n, int m) {
    vector<int> seq (n);
    seq[0] = 0, seq[1] = 1;

    for (int i = 2; i < n; i++) {
        seq[i] = ( seq[i-2] + seq[i-1]) % m ;
    }

    return seq;
}
