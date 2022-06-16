#include <iostream>

#include <cmath>
#include <map>
#include <stack>
#include <algorithm>
#include <functional>

using namespace std;
using func = function<double(double,double)>;

template<class T = void> struct modul {
    const T operator ()(const T &lhs, const T &rhs) {
        return fmod (lhs, rhs);
    }
};
map<string, double> value;
map<char,int> order {{'+', 1},{'-',1},{'*',2},{'/',2},{'%',2}};
map<char,func> operate {{'+', plus<double>()},{'-', minus<double>()},
              {'*', multiplies<double>()}, {'/', divides<double>()}, {'%', modul<double>()} };

template<class T> T getstack (stack<T> &S) {
    if (S.empty()) return 0;
    T val = S.top();
    S.pop();
    return val;
}

double getnum (string::iterator &it) {
    string num;

    num += *it++;
    while (isdigit(*it) || *it == '.') num += *it++;

    return stod (num);
}
string getid (string::iterator &it) {
    string id;

    while (isalnum (*it)) id += *it++;

    return id;
}
string getsub (string::iterator &it) {
    int pile = 1;
    string sub;

    while (*it++) {
        if (*it == '(') pile++;
        if (*it == ')') pile--;
        if (pile == 0) return sub;

        sub += *it;
    }
    return sub;
}

double interpret (std::string expr) {

    expr.erase (remove (expr.begin(), expr.end(), ' '), expr.end());
    if (!expr.size()) throw runtime_error ("Empty expression");

    string::iterator it = expr.begin();
    bool running = true;
    stack<double> val;
    stack<char> ops;

    int sign = 1;

    while (running) {

        int op = order[*it];

        if (*it == '-' && order[*(it - 1)]) {
            sign = -1, op = 0;
        }
        if (*it == '(') {
            string sub = getsub (it);
            double num = interpret (sub) * sign;
            val.push (num);
            sign = 1;
        } else if (isalpha (*it)) {
            string id = getid (it);

            if (*it == '=') {
                string sub = getsub (it);
                value[id] = interpret (sub) * sign;
                return value[id];
            } else {
                if (value.find (id) == value.end())
                    throw::logic_error ("invalid identifier");

                val.push (value[id] * sign);
                sign = 1;
            }

        } else if (isdigit (*it)) {
            val.push (getnum (it) * sign);
            sign = 1;

        } else {

            if (op) {

                while (!ops.empty() && order[ops.top()] >= op) {
                    char op = getstack (ops);
                    double b = getstack(val), a = getstack (val);

                    operate[op] (a, b);
                    val.push (operate[op] (a, b));
                }
                ops.push(*it);
            }

            it++;
        }

        if (it >= expr.end()) running = false;
    }
    while (!ops.empty()) {
        char op = getstack (ops);
        double b = getstack (val), a = getstack (val);
        val.push (operate[op] (a, b));
    }

    return !val.empty() ? val.top() : 0;
}


int main () {
    void Test ();
    string expr;// = "train6=5"; // 18

    expr = "afkd = -72 % (-80 / 64 * -69) - -96";
    /*
    interpret2 ("afkd = -72 % (-80 / 64 * -69) - -96");
    interpret ("urvv = -12 % (34 % -19 * -70) - -15");
    interpret ("eef = 24 * (-5 * 14 - -25) - -53");
    interpret ("mzcol = 46 / -(-22 / -51 - -17) * 32");
    interpret ("iyinw = -9 - (38 + -95 + 68) - 100");
    */
    //value = {{"afkd", 24},{"eef", -1027}, {"iyinw", -120}, {"mzcol", -84.4454},{"urvv", 3}};

    Test();
    cout << "end";
}

/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
  public :
  static void That (const double& actual, const double& expression) {
    cout << fixed;
    if (actual != expression) {
      cout << "actual : " << actual;
      cout << "\nexpect : " << expression;

      cout << endl;
    }
  }
};
const double& Equals (const double& entry) { return entry;}
void Test();
////////////////////////////////////////////////////////////////////////////////
void Test () {
    Assert::That(interpret("1 + 1"), Equals(2.0));
    Assert::That(interpret("2 - 1"), Equals(1.0));
    Assert::That(interpret("2 * 3"), Equals(6.0));
    Assert::That(interpret("8 / 4"), Equals(2.0));
    Assert::That(interpret("7 % 4"), Equals(3.0));

    Assert::That(interpret("x = 1"), Equals(1.0));
    Assert::That(interpret("x"), Equals(1.0));
    Assert::That(interpret("x + 3"), Equals(4.0));
    Assert::That(interpret("(4 + 2) * 3"), Equals(18));
    /*

    try {
        interpret("y");
        Assert::That(0, Equals(1));
    } catch (...) {
        Assert::That(1, Equals(1));
    }

    */
}

/*
double operation (double a, double b, char op) {
    double val;

    switch (op) {
        case '+' : val = a + b; break;
        case '-' : val = a - b; break;
        case '*' : val = a * b; break;
        case '/' : val = a / b; break;
        case '%' : val = fmod (a,b); break;
    }

    return val;
}
int getop (char c) {
    if (c == '-' || c == '+') return 1;
    if (c == '*' || c == '/' || c == '%') return 2;
    return 0;
  }
*/
