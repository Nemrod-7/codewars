#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1

#include <iostream>
#include <vector>
//#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>
using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
typedef pair<unsigned,unsigned> test_t;
class Assert {
    public :
        static void That (test_t actual, test_t expression) {
            if (actual != expression) {
              std::cout<<"actual : "<<actual.first <<" expected : "<<expression.first;

              std::cout<<std::endl;

            }

        }
};
test_t Equals (test_t entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
double zeta_proto (int n) {
    int inf = 999;
    const double epsilon = 1e-7;
    double diff = 1.0, zeta = 0.0;

    for (int i = 1; i < inf/*abs (diff - zeta) >= epsilon*/ ; i++) {
        //cout << pow (i, n);
        double temp = zeta;

        zeta += 1 / (i * i);
        //cout << pow (i, n) << " ";
        //diff = temp - zeta;
    }


    return zeta;
  }

uint64_t factorial (int n) {

    uint64_t mult = 1;
    for (int i = 1; i <= n ; ++i)
        mult *= i;
    return mult;
}
uint64_t binomial (unsigned n, unsigned k) {
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;
    //return binomial(n - 1, k - 1) + binomial(n - 1, k);
    //return factorial(n) / (factorial (k) * factorial (n - k));
    uint64_t res = 1;          //B32

    k = min (k, n - k);
    for (unsigned i = 1; i <= k; i++)
        res *= (n + 1 - i) / (double)i;     // B34

    return res;
}
long double bernoulli_recurse (unsigned n) {
    if (n == 0) return 1;
    if (n == 1) return 0.5;
    if (n % 2) return 0;
    long double sum = 0.0;
    unsigned k = 0;

    for (; k <= n - 1; k++) {
        sum += binomial (n, k) * bernoulli_recurse (k) * 1 / (n - k + 1);
    }

    return 1 - sum;
}
pair<int64_t, int64_t> bernoulli_number (int64_t n) {
    if (n == 0) return {1,1};
    if (n == 1) return {-1,2};
    if (n % 2) return {0,1};

    pair<int64_t, int64_t> frac {1,1};
    int64_t num = 0, den = 0, k = 0;
    //sum from k = 0 to N - 1 binom (N, k) * B(k)
    //pair<unsigned, unsigned> divi, calc, sum;

    for (; k <= n - 1; k++) {
        num = bernoulli_number (k).first * binomial (n, k);
        den = bernoulli_number (k).second * (n - k + 1);
        frac.first = frac.second * num + frac.first * den;
        frac.second = frac.second * den;
        //cout << frac.first << " ";

        int64_t gcd = __gcd (frac.first, frac.second);
        frac.first /= gcd, frac.second /= gcd;

    }

    return {frac.second - frac.first, frac.second};
}

double bernoulli_cumulat (int n) {
    if (n == 0) return 1;
  //  if (n == 1) return 0.5;
    //if (n %  2) return 0;

    int64_t num, den;
    pair<int64_t, int64_t> frac {1,1};

    for (int k = 0; k <= n; k++)
        for (int i = 0; i <= k; i++) {
            num = pow (-1, i) * binomial (k, i) * pow (i, n);
            den = (k + 1);
            int64_t gcd = __gcd (num, den);
            //num /= gcd, den /= gcd;

            frac.first = frac.second * num + frac.first * den;
            frac.second = frac.second * den;

            //cout << frac.first << " "<<endl;
            gcd = __gcd (frac.first, frac.second);
            frac.first /= gcd, frac.second /= gcd;
        }
      //  cout << frac.first / (double)frac.second << endl;
    return frac.first / (double)frac.second ;
}
double bernoulli_riemann (int n) {
    if (n == 0) return 1;
    if (n == 1) return 0.5;
    if (n % 2) return 0;

    return -riemann_zeta (1 - n) * n;
}

void pascal_triangle (int n) {

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i;++j)
           //binomial (i, j);
            cout << binomial (i, j) << " ";

        cout << endl;
    }
}
vector<int> pascal_tri (int n) {

    vector<int> tri(n + 2);

    for (int i = 0; i <= n; i++) {
        vector<int> buffer = {1};
        for (int j = 1; j <= n; j++)
            buffer.push_back(tri[j - 1] + tri[j]);

        tri = buffer;
    }

    for (int i = 0; i <= n; i++)
        cout << tri[i] << " ";
    return tri;
}

pair<int,int> add (pair<int,int> a, pair<int,int> b) {

    pair<int,int> sum;

    sum.first = a.second * b.first + a.first * b.second;
    sum.second = a.second * b.second;

    return sum;
}


void display (pair<int,int> actual) {
    cout << actual.first <<"/"<< actual.second << endl;
}

int main () {

    auto start = std::chrono::steady_clock::now();


        //cout << fixed<< bernoulli_cumulat (i) << endl;
       //display (bernoulli_number (i));
      for (int i = 0; i < 30; ++i)
          display (bernoulli_number (i));
    //sum from k = 0 to N - 1 binom (N, k) * B(k)
    //b(k) = sum / binom (N, N - 1)
    /*
    int i = 0;
    //cout << "number  recursive     riem-zeta" << endl;
    for (i = 1; i < 20;++i)
        //bernoulli_cumulat (i);
        cout << "B" << i << "  : " << scientific
             << bernoulli_recurse (i) << "   "
             << bernoulli_riemann (i) << "   "
             << bernoulli_cumulat (i) << "   "
             << endl;
             /*
    */

        //cout << binomial (3, 0);
        //factorial(20);
        //binomial(i, i+3);
        //pascal_triangle (20);
          //std::riemann_zeta(2) ;
    //    cout << i << " " << bern (i) << endl;

    auto end = std::chrono::steady_clock::now();
    //std::chrono::duration<double,  std::milli> elapsed = (end - start);
    std::cout << "Process took " << fixed
              << std::chrono::duration<double,std::milli>(end - start).count()
              << " ms" << std::endl;
}


void Test() {
    //Assert::That(bernoulli_number(0), {1,1});
    //Assert::That(bernoulli_number(1), {-1,2});
    //Assert::That(bernoulli_number(2), {1,6});
    //Assert::That(bernoulli_number(3), {0,0});
    //Assert::That(bernoulli_number(4), {-1,30});
    //Assert::That(bernoulli_number(6), {1,42});
    //Assert::That(bernoulli_number(42),{1520097643918070802691,1806});
  //  Assert::That(bernoulli_number(22),{854513,138});

}
