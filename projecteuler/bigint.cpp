
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "base.hpp"

using namespace std;

class Bigint {
    private :
        string m_value;

        int substract (string &a, string &b, size_t pos) {

            size_t lena = a.size(), lenb = b.size();
            int rem = 0;

            if (lena >= lenb + pos) {
              string tmp = string (lenb, '0');
              int u = 0;

              for (size_t i = 0; i < lenb; ++i) {
                  size_t pos2 = lenb - 1 - i;

                  u += a[pos + pos2] - b[pos2];
                  tmp[pos2] += u + (u < 0 ? 10 : 0);

                  u = (u < 0 ? -1 : 0);
              }

              if (u == 0) {
                  rem = 1;
              }
              else if (pos > 0) {
                  if (a[pos - 1] > '0') {
                      a[pos - 1]--;
                      rem = 1;
                  }
              }

              if (rem != 0)
                  a.replace (pos, tmp.size(), tmp);
            }

            return rem;
        }
        bool ismaller (string a, string b) {
            if (b.size() > a.size()) return true;
            if (b.size() < a.size()) return false;

            for (int i = 0; i < b.size(); i++) {
                if (b[i] > a[i]) return true;
                if (b[i] < a[i]) return false;
            }
            return false;
        }
    public :
        Bigint (const string &input = "0") : m_value (input) { m_value.erase(0, min (m_value.find_first_not_of ('0'), m_value.size() - 1));};
        /*
        void operator= (const string &x) {
          //  m_value = x;
            cout << x;
            //m_value.erase(0, min (m_value.find_first_not_of ('0'), m_value.size() - 1));
        }
        */
        template<class T> &operator T() { return m_value; }
        size_t size() { return m_value.size(); }
        friend ostream &operator <<(ostream &os, const Bigint &x) {
            return os << x.m_value;
        }
        //void operator= (const string &b) { m_value = b; }
        void operator= (const char *b) { m_value = (b); }
        void operator++ (int) {
            m_value = to_string (stoi (m_value) + 1);
        }

        Bigint operator+ (string b) {
            string add;
            int i = m_value.size(), j = b.size(), index = max (i,j);
            int rem = 0, digit, num;

            while (index-->0) {
                num = (i > 0 ? (m_value[--i] - '0') : 0) + (j > 0 ? (b[--j] - '0') : 0) + rem;
                rem = num / 10;
                add += ((num % 10) + '0');
            }

            if (rem > 0) add += (rem + '0');

            reverse (add.begin(), add.end());
            add.erase(0, min (add.find_first_not_of ('0'), add.size() - 1));

            return add;
        }
        Bigint operator* (string b) {
            string mul, a = m_value;
            int lena = a.size(), lenb = b.size(), ans[1000]{0};

            if (m_value == "0") return m_value;
            if (b == "0") return b;
            /*
            if ((lena == 1 && m_value[0] == '0')|| (lenb == 1 && b[0] == '0')) {
                return  m_value;
            }
            */
            int i,j,rem = 0, num, size;
            reverse (a.begin(), a.end()), reverse(b.begin(), b.end());

            for (i = 0; i < lena; ++i) {
                for (j = 0; j < lenb; ++j) {
                    ans[i + j] +=  (a[i] - '0') * (b[j] - '0');
                    ans[i + j + 1] = ans[i + j + 1] + ans[i + j] / 10;
                    ans[i + j] %= 10;
                }
            }
            size = i + j;
            while (ans[size] == 0) size--;

            do {
                mul.push_back ((ans[size] + '0'));
            } while (size-->0);

            return mul;
        }
        Bigint operator- (string b) {
            string a = m_value;
            int carry = 0;
            bool minus = false;
            string sub;

            if (ismaller (a, b)) {
                swap (a, b);
                minus = true;
            }

            reverse (a.begin(), a.end()), reverse (b.begin(), b.end());
            while (b.size() != a.size()) b += '0';
            for (int i = 0; i < a.size(); i++) {
                int dig = a[i] - b[i] - carry;

                if (dig < 0) {
                    dig += 10, carry = 1;
                } else {
                    carry = 0;
                }

                sub += to_string (dig);
            }

            reverse (sub.begin(), sub.end());
            sub.erase(0, min (sub.find_first_not_of ('0'), sub.size() - 1));

            return minus ? '-' + sub : sub;
        }
        Bigint operator/ (string b) {
          string a = m_value;
          size_t lena = a.size(), lenb = b.size ();
          string quot ("0"), den (a);

          for (size_t i = 0 ; lena >= lenb; ++i, --lena) {
              if (i > 0) quot += '0';
              char d = '*';

              while (quot[i] != d) {
                  d = quot[i];
                  quot[i] += substract (den, b, i);
              }
          }

          quot.erase(0, min (quot.find_first_not_of ('0'), quot.size() - 1));
          //den.erase(0, min (den.find_first_not_of ('0'), den.size() - 1));
          return quot;
        }

        bool operator== (const string &b) { return m_value == b; }
        bool operator<= (const string &b) {
            if (m_value.size () < b.size()) return true;
            if (m_value.size() > b.size()) return false;

            for (int i = 0; i < m_value.size(); i++) {
                if (m_value[i] < b[i]) return true;
                if (m_value[i] > b[i]) return false;
            }

            return true;
        }
        bool operator > (const string &b) {
            if (m_value.size() > b.size()) return true;
            if (m_value.size() < b.size()) return false;

            for (int i = 0; i < m_value.size(); i++) {
                if (m_value[i] > b[i]) return true;
                if (m_value[i] < b[i]) return false;
            }

            return true;
        }
};

string square (const string &src) {

    string num = src;
    uint64_t root = 0;
    int ndig = src.size();

    if (ndig % 2) {
        num = "0" + num;
    }
    int carry = 0;

    for (int i = 0; i < num.size(); i += 2) {

        int ref = (carry * 100) + (num[i] - '0') * 10 + (num[i + 1] - '0');
        int x = 0, n = 20 * root;

        while (((n + x) * x) <= ref) {
            x++; //n = ((20 * root + x) * x);
        }


        if (x > 1) x--;
        //n = (20 * root + x) * x;
        carry = ref - ((n + x) * x);
        root = root * 10 + x;
    }

    if (carry < 0) root -= 1;
        //cout << carry << endl;


    return to_string (root);
  }
string bigfactorial (int num) {

    if (num < 0) return "";

    int rem = 0;
    string  os;
    vector<int> fact = {1};

    for (int i = 1; i <= num; i++) {
        for (auto &dig : fact) {
           int val = dig * i + rem;
           dig = val % 10;
           rem = val / 10;
        }

        while (rem) {
          fact.push_back(rem % 10);
          rem /= 10;
        }
    }

    for (int i = fact.size() - 1; i >= 0; i--) {
        os += fact[i] + '0';
    }

    return os;
}

Bigint power (int i, int j) {
    string a = to_string(i);
    Bigint num = to_string(1);

    while (j-->0)
        num = num * a;

    return num;
}

bool is_square (double x) {
    int sq = sqrt(x);
    return (sq * sq) == (int) x;
}
double trunc (double x) { return round (x * 1e5) / 1e5; }

vector<int> convergence (const double x) {

    vector<int> frac;
    double rn = x, ip = floor (rn), fp = rn - ip;
    map<double, bool> cycle;
    const double thresh = 1e-8;
    //printf ("%f => ", rn);
    frac.push_back (static_cast<int> (round(ip)));

    while (fp > thresh) {
        rn = 1 / fp;
        ip = floor (rn);
        fp = rn - ip;

        if (cycle[trunc (fp)]) {
            break;
        } else {
            cycle[trunc (fp)] = true;
        }
        frac.push_back (static_cast<int> (round(ip)));
        //printf ("%5.2f %5.2f %5.2f\n", ip, fp, rn);
    }

    return frac;
}

string diophantine5 (int n) {
    double sq = sqrt (n);
    if (is_square (n)) return "1";

    Bigint delta (to_string (n));
    Bigint x, y, z, eq;
    vector<int> a = convergence (sq);
    vector<Bigint> p (3), q (3);
    int size = a.size() - 1;

    p[0] = "0", p[1] = "1", p[2] = to_string (a[0]);
    q[0] = "1", q[1] = "0", q[2] = "1";

    for (int k = 0; k < 40; k++) {
        string an = to_string (a[(k % size) + 1]);
        x = p[k+1] + p[k+2] * an;
        y = q[k+1] + q[k+2] * an;
        z = delta * y * y;
        eq = x * x - z;

        cout << x << "^2 - " << delta << " * " << y << "^2 => " << eq ;
        cout << '\n';

        if (eq == "1") {
            return x;
        }

        p.push_back(x);
        q.push_back(y);
        /*
        */

        //cout << endl;
    }


    return "0";
}

string pell (int n) {

    if (is_square (n)) return "1";

    Bigint delta (to_string(n)), sq (to_string ( static_cast<int> (sqrt(n))));
    Bigint an (sq * "2"), y (sq), z ("1");
    Bigint x, b, tmp;
    vector<Bigint> p (2), q(2);

    p[0] = "1", p[1] = "0";
    q[0] = "0", q[1] = "1";

    for (int i = 0; i < 30; i++) {
        y = an * z - y;
        z = (delta - y * y) / z;
        an = (sq + y) / z;

        p = {p[1], an * p[1] + p[0]};
        q = {q[1], an * q[1] + q[0]};

        x = sq * q[1] + p[1];
        b = q[1];

        if (x * x - delta * b * b == "1") {
            cout << x << "^2 - " << delta << " * " << b << "^2 => ";
            cout << x * x - delta * b * b << '\n';
            break;
        }
    }
    /*
    */
    return x;
}
int main () {

  Timer clock;

  map<int,bool> sqr;
  for (int i = 1; i < 100; i++) {
      sqr[i*i] = true;
  }

  Bigint val, maxv;
  string res;

  for (int i = 1; i <= 1000; i++) {
      if (sqr[i] == false) {

          cout << i << " = > ";
          val = pell (i);// = diophantine5 (i);

          if (val > maxv) {
              maxv = val;
              res = to_string (i);
          }
      }
  }
  cout << res ;

  /*
  */



    clock.stop();
    clock.get_duration();

    return EXIT_SUCCESS;
}
