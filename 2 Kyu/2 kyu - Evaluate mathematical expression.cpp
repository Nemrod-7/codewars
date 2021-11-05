#include <iostream>
#include <algorithm>
#include <stack>

using namespace std;
using index = string::iterator;

template<class T> T getstack (stack<T> &S) {
    if (S.empty()) return 0;
    T val = S.top();
    S.pop();
    return val;
  }

double getnum (index &it) {
    string num;

    num += *it++;
    while (isdigit(*it) || *it == '.') num += *it++;

    return stod (num);
}
string getsub (index &it) {
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
int getop (char c) {
    switch (c) {
        case '-' : return 1;
        case '+' : return 1;
        case '*' : return 2;
        case '/' : return 2;
        default  : return 0;
    }
}

double operation (double a, double b, char op) {

    double val;
    switch (op) {
        case '+' : val = a + b; break;
        case '-' : val = a - b; break;
        case '*' : val = a * b; break;
        case '/' : val = a / b; break;
    }

    return val;
}
double calc (string src) {

    src.erase(remove (src.begin(), src.end(), ' '), src.end());
    index it = src.begin();
    cout << fixed;
    stack<double> val;
    stack<char> ops;

    double num, sign = 1;

    while (it != src.end()) {
        char tok = *it;
        int op = getop (tok);

        if (tok == '-' && getop (*(it - 1))) {
            sign = -1;
            op = 0;
        }

        if (tok == '(') {
            string sub = getsub (it);
            num = calc (sub) * sign;
            val.push (num);
            num = 0, sign = 1;

        } else if (isdigit (*it)) {
            num = getnum (it) * sign;
            val.push (num);
            num = 0, sign = 1;

        } else {
            if (op) {
                while (!ops.empty() && getop (ops.top()) >= op) {
                    char op = getstack (ops);
                    double b = getstack(val), a = getstack (val);
                    val.push(operation (a, b, op));
                    cout << a << " " << op << " " << b << " = " << operation (a, b, op) << endl;
                }
                ops.push(tok);
            }

            it++;
        }
    }

    while (!ops.empty()) {
        char op = getstack (ops);
        double b = getstack (val), a = getstack (val);
        val.push(operation (a, b, op));
    }

    return !val.empty() ? val.top() : 0;
}

int main () {

    /*
    Assert::That(calc("63 - -20 / 84 - 33 * 87 + -17 * 41 + -10"), 3514.76);
    
    Assert::That(calc("(64) * (-97 / 51 * -(55)) - (21 * -((((71 - -33)))) - 44)"), 8922.901960784315);
    Assert::That(calc("2 + 3 * 4 / 3 - 6 / 3 * 3 + 8"), 8.0);
    Assert::That(calc("-7 * -6 / 3"), Equals(14.0));
    Assert::That(calc("10- 2- -5"), Equals(13.0));
    Assert::That(calc("(1 + 1) / 3"), 0.6666666666666666);
    Assert::That(calc("3 -(-1)"), Equals(4.0));
    Assert::That(calc("1 + 1"), Equals(2.0));
    Assert::That(calc("8/16"), Equals(0.5));
    Assert::That(calc(""), 0.0);
    Assert::That(calc("1 + 1 / 3"), 1.3333333333333333);
    Assert::That(calc("(1 + 1) / 3"), 0.6666666666666666);
    Assert::That(calc("6 + - (-4)"), 10.0);
    Assert::That(calc("6 + - (4)"), 2.0);
    Assert::That(calc("2 + -2"), Equals(0.0));
    Assert::That(calc("(((10)))"), Equals(10.0));
    Assert::That(calc("3 * 5"), Equals(15.0));
    Assert::That(calc("(123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) - (123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) + (13 - 2)/ -(-11)"), Equals(1.0));

    /*
        */
    //cout << expression;

    //while ()

    //cout << calc (expression);
    //cout << 1 + 1 / (double)3;
}
