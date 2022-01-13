#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
  public :
  static void That (const string &actual, const string &expression) {
    cout << fixed;
    if (actual != expression) {
      cout << "actual : " << actual;
      cout << "\nexpect : " << expression;

      cout << endl;
    }
  }
};
const string &Equals (const string &entry) { return entry;}
void Test();
////////////////////////////////////////////////////////////////////////////////
class Bigint {
    private :
        string m_value;

        bool ismaller (string a, string b) {
            if (b.size() > a.size()) return true;
            if (b.size() < a.size()) return false;

            for (size_t i = 0; i < b.size(); i++) {
                if (b[i] > a[i]) return true;
                if (b[i] < a[i]) return false;
            }
            return false;
        }
    public :
        Bigint (const string &input = "0") {
          m_value = input;
          m_value.erase(0, min (m_value.find_first_not_of ('0'), m_value.size() - 1));};
          /*
        void operator= (const string &x) {
            m_value = x;
            cout << x;
            //m_value.erase(0, min (m_value.find_first_not_of ('0'), m_value.size() - 1));
        }
        */
        template<class T> operator T() { return m_value; }
        operator string ()  { return m_value; }
        friend ostream &operator<< (ostream &os, const Bigint &x) {
            return os << x.m_value;
        }

        void operator++ (int) {
            m_value = to_string (stoi (m_value) + 1);
        }
        Bigint operator+ (string b) {
            string add;
            int i = m_value.size(), j = b.size(), index = max (i,j);
            int rem = 0, num;

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
            int i,j, size;
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
            for (size_t i = 0; i < a.size(); i++) {
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

            for (size_t i = 0; i < m_value.size(); i++) {
                if (m_value[i] < b[i]) return true;
                if (m_value[i] > b[i]) return false;
            }

            return true;
        }
};

string integer_square_root (string num) {
    Bigint root, carry;

    if (num.size() % 2) {
        num = '0' + num;
    }

    for (size_t i = 0; i < num.size(); i += 2) {
        string cent = carry;
        Bigint ref = cent + num[i] + num[i + 1];
        Bigint x, n = root * "20";

        while (((n + x) * x) <= ref) {
            x++;
        }
        /// x = x - "1";
        carry = ref - ((n + x) * x);
        root = root * "10" + x;
        /*
        */
        //cout << ref << " ";

    }
    return root;
}

void Test () {
  Assert::That(integer_square_root("1"), Equals("1"));
  Assert::That(integer_square_root("5"), Equals("2"));
  Assert::That(integer_square_root("17"), Equals("4"));
  Assert::That(integer_square_root("23232328323215435345345345343458098856756556809400840980980980980809092343243243243243098799634"), Equals("152421548093487868711992623730429930751178496967"));
  Assert::That(integer_square_root("12323309809809534545458098709854808654685688665486860956865654654654324238000980980980"), Equals("3510457208086937291253621317073222057793129"));

}
int main () {

    string num = "1";

    cout << integer_square_root (num);

    return 0;
}
