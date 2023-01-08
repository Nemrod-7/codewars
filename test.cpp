#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <array>
#include <set>
#include <chrono>
using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        template<class T> static void That (const T& a, const T& b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
        template<class T> static void That (const vector<T> &a, const vector<T> &b) {
            if (a != b) {
                cout << "actual : ";
                for (auto &it : a) cout << it << " ";

                cout << " expected : ";
                for (auto &it : b) cout << it << " ";

                cout << endl;
            }
        }
};
template<class T> T Equals (const T& entry) { return entry;}
template<class T> T EqualsContainer (const T& entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
/*
   Product Partition           Score(sc)
   [708, 2]                    1420  <---- maximum value
   [472, 3]                     950
   [354, 2, 2]                 1074
   [354, 4]                     716
   [236, 3, 2]                  723
   [236, 6]                     484
   [177, 2, 2, 2]               740    # equals to: (177^1 + 2^3) * 4
   [177, 4, 2]                  549
   [177, 8]                     370
   [118, 3, 2, 2]               500
   [118, 4, 3]                  375
   [118, 6, 2]                  378
   [118, 12]                    260
   [59, 3, 2, 2, 2]             350    # equals to: (59^1 + 3^1 + 2^3) * 5
   [59, 6, 2, 2]                276
   [59, 4, 3, 2]                272
   [59, 12, 2]                  219
   [59, 8, 3]                   210
   [59, 6, 4]                   207
   [59, 24]                     166  <---- minimum value
   */

vector<int> sieve (int num) {

    bool *primes = new bool[num + 1];
    vector<int> sieve {2};

    fill_n (primes, num + 1, true);

    for (int p = 3; p * p <= num ; p += 2)
        if (primes[p] == true)
            for (int i = p * p; i <= num; i += 2 * p)
                primes[i] = false;

    for (int i = 3; i <= num; i += 2)
        if (primes[i] == true)
            sieve.push_back(i);

    return sieve;
}
string p_factors (int num) {

    vector<int> primes = sieve (num);

    for (auto &p : primes) {
        int ex = 0;

        while (num % p == 0) {
            num /= p;
            ex++;
        }

        if (ex > 0) {
            cout << p;
            if (ex > 1) cout << "^" << ex;
            if (num > 1) cout << " * ";
        }
    }

    return "";
}
int radical (int maxn, int n) {

    vector<int> primes = sieve (maxn);
    vector<pair<int,int>> hist;
    for (int k = 2; k <= maxn; k++) {
        int rad = 1;

        for (auto &p : primes) {
            if (k % p == 0) {
                rad *= p;
            }
            if (p >= k) break;
        }
        hist.push_back({rad, k});
    }
    sort(hist.begin(), hist.end());
    return hist[n - 1].second;
}

bool is_prime (int num) {

    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (int i = 5; i <= sqrt(num); i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
vector<int> factorize (int n) {

    vector<int> factors;

    for (int k = 2; k < n; k++)
        if (n % k == 0)
            factors.push_back(k);

    return factors;
}
int product (const vector<int> &clust) {
    int prod = 1;

    for (auto &num : clust)
        prod *= num;

    return prod;
}

vector<int> path; // it will store all current factors

void recurse(int max, int val) {

    if (val == 1) {
        for (int i = 0; i < path.size(); i++)
            cout << path[i] << " ";

        cout << endl;
        return;
    }

    for (int i = max; i > 1; i--) {
        if (val % i == 0) {
            path.push_back(i);
            recurse(i, val / i);
            path.pop_back();
        }
    }
}
void Output(int value) {
    cout << "Result for " << value << ": " << endl;
    recurse(value, value);
}

std::set<int> sieve2 (const int num) {

    const int end = sqrt (num);
    bool primes[num / 2 + 1];
    std::set<int> sieve;

    std::fill_n (primes, num / 2 + 1, true);
    sieve.insert(2);

    for (int p = 3; p <= end ; p += 2)
        if (primes[p / 2] == true)
            for (int i = p * p; i <= num ; i += 2 * p)
                primes[i / 2] = false;

    for (int i = 3; i <= num; i += 2)
        if (primes[i / 2] == true)
            sieve.insert(i);

    return sieve;
}
std::array<int, 3> findEmirp (const int lim) {

    int cnt = 0, maxv = 0, sum = 0;
    std::set<int> primes = sieve2 (lim * 2);

    for (auto p : primes) {
        int num = p, rev = 0;

        if (p > lim) break;

        do {
            rev = rev * 10 + num % 10;
        } while (num /= 10);

        if (rev != p && primes.find(rev) != primes.end()) {
            cnt++;
            maxv = max (maxv, p);
            sum += p;
            cout << rev << " " << p << endl;
        }
    }

    return {cnt, maxv, sum};
}
std::string sierpinski (int n) {

    const int dim = pow (3, n);
    std::string asc;

    for (int y = 0; y < dim; y++) {
        for (int x = 0; x < dim; x++) {
            bool flag = true;

            for (int i = dim / 3; i ; i /= 3) {
                if ((y % (i * 3)) / i == 1 && (x % (i * 3)) / i == 1) {
                    flag = false;
                }
            }
            asc += (flag == true ? "██" : "  ");
        }
        if (y < dim - 1) asc += "\n";
    }

    return asc;
}

char *human_readable_time (unsigned seconds, char *time_string) {

    int hh = 0, mm = 0, ss = 0;

    ss = seconds % 60;
    mm = seconds / 60 % 60;
    hh = seconds / 3600;
    //human_readable_time (59, time);

    printf ("%02i:%02i:%02i\n", hh,mm,ss);
    return time_string; // return it
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();tom


    human_readable_time (0, "00:00:00");
    human_readable_time (59, "00:00:59");
    human_readable_time (60, "00:01:00");
    human_readable_time (90, "00:01:30");
    human_readable_time (3599, "00:59:59");
    human_readable_time (3600, "01:00:00");
    human_readable_time (45296, "12:34:56");
    human_readable_time (86399, "23:59:59");
    human_readable_time (86400, "24:00:00");
    human_readable_time (359999, "99:59:59");

    /*
    */

    /*
    HH = hours, padded to 2 digits, range: 00 - 99
    MM = minutes, padded to 2 digits, range: 00 - 59
    SS = seconds, padded to 2 digits, range: 00 - 59
    */

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}

void Test () {

    int n = 10;
    std::array<int, 3> arr = {0, 0, 0};

    Assert::That(findEmirp(n), Equals(arr));
    n = 50;
    arr = {4, 37, 98};
    Assert::That(findEmirp(n), Equals(arr));
    n = 100;
    arr = {8, 97, 418};
    Assert::That(findEmirp(n), Equals(arr));
    n = 200;
    arr = {15, 199, 1489};
    Assert::That(findEmirp(n), Equals(arr));
    n = 500;
    arr = {20, 389, 3232};
    Assert::That(findEmirp(n), Equals(arr));
    n = 750;
    arr = {25, 743, 6857};
    Assert::That(findEmirp(n), Equals(arr));
    n = 1000;
    arr = {36, 991, 16788};
    Assert::That(findEmirp(n), Equals(arr));
}
