#include "base.hpp"
#include <algorithm>

class Bigint {
    private :
        string m_value;

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

        friend ostream &operator <<(ostream &os, const Bigint &x) {
            return os << x.m_value;
        }
        void operator= (const string &b) { m_value = b; }

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

        bool operator<= (const string &b) {
            if (m_value.size () < b.size()) return true;
            if (m_value.size() > b.size()) return false;

            for (int i = 0; i < m_value.size(); i++) {
                if (m_value[i] < b[i]) return true;
                if (m_value[i] > b[i]) return false;
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
int main () {

  Timer clock;

  for (int i = 10; i < 50; i++) {
      cout << i << " => ";
      string num = bigfactorial (i);

      int sz = num.size() - 1;

      while (num[sz] == '0') {
          sz--;
      }

      cout << num[sz-4];
      cout << num[sz-3];
      cout << num[sz-2];
      cout << num[sz-1];
      cout << num[sz-0];

      cout << endl;
  }

  clock.stop();
  clock.get_duration();

  return EXIT_SUCCESS;
}
