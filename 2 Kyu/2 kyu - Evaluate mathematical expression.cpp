#include <iostream>
#include <algorithm>
#include <map>
#include <stack>
#include <functional>

using namespace std;
using index = string::iterator;
using func = function<double(double,double)>;

map<char,int> order {{'+', 1},{'-',1},{'*',2},{'/',2},{'%',2}};
map<char,func> operate {{'+', plus<double>()},{'-', minus<double>()},
              {'*', multiplies<double>()}, {'/', divides<double>()}};

/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
      template<class T> static void That (const T& actual, const T& expression) {
          cout << fixed;
          if (actual != expression) {
              cout << "actual : " << actual;
              cout << "\nexpect : " << expression;

              cout << endl;
          }
    }
};
template<class T> const T& Equals (const T& entry) { return entry;}
void Test();
////////////////////////////////////////////////////////////////////////////////

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

double calc (string src) {
    // convert expression to reverse polish notation using shunting-yard algorithm
    src.erase(remove (src.begin(), src.end(), ' '), src.end());
    index it = src.begin();
    double num, sign = 1;

    stack<double> val;
    stack<char> ops;

    while (it != src.end()) {

        char tok = *it;
        int op = order[tok];

        if (tok == '-' && order[*(it - 1)]) {
            sign = -1;
            op = 0;
        }

        if (tok == '(') {
            string sub = getsub (it);
            num = calc (sub) * sign;
            //cout << num << " ";
            val.push (num);
            num = 0, sign = 1;

        } else if (isdigit (*it)) {
            num = getnum (it) * sign;
            val.push (num);
            sign = 1;

        } else {
            if (op) {

                while (!ops.empty() && order[ops.top()] >= op) {
                    char op2 = getstack (ops);
                    double b = getstack(val), a = getstack (val);
                    // cout << operate[op] (a, b) << " ";
                    val.push(operate[op](a, b));
                    //cout << a << " " << op << " " << b << " = " << operation (a, b, op) << endl;
                }
                /*
                */
                ops.push(tok);
            }

            it++;
        }
    }

    while (!ops.empty()) {
        char op = getstack (ops);
        double b = getstack (val), a = getstack (val);
        val.push(operate[op](a,b));
        //cout << a << " " << op << " " << b << " = " << operation (a, b, op) << endl;

    }
    //cout << (!val.empty() ? val.top() : 0) << " ";
    return !val.empty() ? val.top() : 0;
}

int main () {

    string expression  = "(9.46+8.56+(7.55)/7.56/9.51/7.56/ -5.27-8.63*7.54/7.23* -0.84-5.64/(9.47* -7.40+ -5.94/6.80)/ -7.39*7.45-8.26/ -0.33+ -6.96+7.72-6.12+ -8.59+7.38*8.87+ -0.44+ -3.20+ -2.25- -2.13/(7.99-8.69*9.26-8.33+(7.12/6.71+6.75)+6.49+ -4.73+5.36*9.52/ -0.84/6.71-8.94/ -3.07-(9.52+ -3.57)+8.22- -8.54-5.09+ -1.97/ -3.68/7.13)/7.15*6.67-9.87+7.78/7.96*9.97/(9.50)/(9.02+7.97)+(((7.20+ -8.52+9.04/8.07/ -7.91/8.38/8.54))))";

    //expression = "-7 * -(6 / 3)";

    Assert::That(calc("2 + 3 * 4 / 3 - 6 / 3 * 3 + 8"), 8.0);
    //Assert::That(calc("63 - -20 / 84 - 33 * 87 + -17 * 41 + -10"), -3514.761905);
    Assert::That(calc("(64) * (-97 / 51 * -(55)) - (21 * -((((71 - -33)))) - 44)"), 8922.901960784315);
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
void Test () {

}
////////////////////////////////////////////////////////////////////////////////
/*

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

bool isnumber (index it) {
  if (isdigit (*it)) return true;
  if (*it == '-') {
      if (isdigit (*(it - 1))) return false;
      if (isdigit (*(it + 1))) return true;
  }
  return false;
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
*/
