#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>
#include <functional>

#include <chrono>

using namespace std;

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
vector<string> tokenize (const string &expr) {
  regex token ("\\*|\\+|/|-?[0-9]+(\\.[0-9]+)?|-|\\(|\\)");
  sregex_token_iterator it (expr.begin(), expr.end(), token);
  return vector<string> (it, sregex_token_iterator());
}
template<class T> T getstack (vector<T> &S) {
    if (S.empty()) return 0;
    T val = S.back();
    S.pop_back();
    return val;
}
double getnum (string::iterator &it) {
    string num;

    num += *it++;
    while (isdigit(*it) || *it == '.') num += *it++;

    return stod (num);
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
double calc (string src) {   // convert expression to reverse polish notation using shunting-yard algorithm

    src.erase(remove (src.begin(), src.end(), ' '), src.end());
    string::iterator it = src.begin();
    double num, sign = 1;

    vector<double> val;
    vector<char> ops;

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

            val.push_back  (num);
            num = 0, sign = 1;

        } else if (isdigit (*it)) {
            num = getnum (it) * sign;
            val.push_back (num);
            sign = 1;

        } else {
            if (op) {
                while (!ops.empty() && order[ops.back()] >= op) {
                    char op2 = getstack (ops);
                    double b = getstack (val), a = getstack (val);

                    val.push_back (operate[op2](a, b));
                }

                ops.push_back (tok);
            }

            it++;
        }
    }
    while (!ops.empty()) {
        char op = getstack (ops);
        double b = getstack (val), a = getstack (val);
        val.push_back (operate[op](a,b));

    }

    return !val.empty() ? val.back() : 0;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    string expression = "(123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) - (123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) + (13 - 2)/ -(-11)";

    /*
    regex token ("\\*|\\+|/|-?[0-9]+(\\.[0-9]+)?|-|\\(|\\)");
    sregex_token_iterator it (expression.begin(), expression.end(), token);
    return vector<string> (it, sregex_token_iterator());

    for (auto &it : V) {
        cout << "[" << it << "]";
    }

    /*
    Assert::That(calc ("(64) * (-97 / 51 * -(55)) - (21 * -((((71 - -33)))) - 44)"), 8922.901960784315);

    Assert::That(calc ("2 + 3 * 4 / 3 - 6 / 3 * 3 + 8"), 8.0);
    Assert::That(calc ("-7 * -6 / 3"), Equals(14.0));
    Assert::That(calc ("10- 2- -5"), Equals(13.0));
    Assert::That(calc ("8/16"), Equals(0.5));
    Assert::That(calc ("1 + 1 / 3"), 1.3333333333333333);
    Assert::That(calc ("2 + -2"), Equals(0.0));
    Assert::That(calc ("3 * 5"), Equals(15.0));
    Assert::That(calc (""), 0.0);
    Assert::That(calc ("(1 + 1) / 3"), 0.6666666666666666);
    Assert::That(calc ("3 -(-1)"), Equals(4.0));
    Assert::That(calc ("1 + 1"), Equals(2.0));
    Assert::That(calc ("6 + - (-4)"), 10.0);
    Assert::That(calc ("6 + - (4)"), 2.0);
    Assert::That(calc ("(((10)))"), Equals(10.0));
    Assert::That(calc ("(123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) - (123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) + (13 - 2)/ -(-11)"), Equals(1.0));
//    Assert::That(calc2("63 - -20 / 84 - 33 * 87 + -17 * 41 + -10"), -3514.761905);
  */
    cout << "end";
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;

}

void Test () {

}
////////////////////////////////////////////////////////////////////////////////
/*
vector<string> tokenize (string src) {
  regex re ("\\w+|(-?[0-9]+(.[0-9]+)?)|(\\+|\\-|\\/|\\*|=)|(\\(|\\))");
  sregex_token_iterator it (src.begin(), src.end(), re);

  return vector<string> (it, sregex_token_iterator());
}
int getsub2 (vector<string>::iterator str) {
  int pile = 0;
  vector<string>::iterator it = str;

  while (true) {
    if (*it == "(") pile++;
    if (*it == ")") pile--;
    if (pile <= 0) return it - str;
    it++;
  }

  return 0;
}
double interpret (vector<string> expr) {

  //  if (!expr.size()) return 0;// throw runtime_error ("Empty expression");
  const regex alpha ("[a-zA-Z]+"), number ("(-?[0-9]+(.[0-9]+)?)"), oper ("(\\+|\\-|\\/|\\*|=)"), par ("(\\(|\\))");
  //bool running = true;
  stack<double> val;
  stack<string> ops;
  auto it = expr.begin();

  map<string,int> order {{"+", 1},{"-",1},{"*",2},{"/",2},{"%",2}};
  map<string,func> operate {{"+", plus<double>()},{"-", minus<double>()},
  {"*", multiplies<double>()}, {"/", divides<double>()}};
  double sign = 1;

  while (it != expr.end()) {
    string tok = *it;

    if (regex_match (tok, alpha)) {
      cout << "var : [" << tok << "]";
    } else if (regex_match (tok, number)) {
      val.push (stod(tok) * sign);
      sign = 1;
    } else if (regex_match (tok, oper)) {
      int op = order[tok];

      if (tok == "-" && order[*(it - 1)]) {
        sign = -1;
      } else {
        while (!ops.empty() && order[ops.top()] >= op) {
          string op2 = getstack (ops);
          double b = getstack (val), a = getstack (val);
          val.push  (operate[op2](a, b));
        }

        ops.push (tok);
      }
    } else if (regex_match (tok, par)) {
      auto nxt = it + getsub2 (it);
      double num = interpret ({it + 1, nxt}) * sign;
      val.push (num);
      it = nxt, sign = 1;
    } else {
      //    cout << "dev : [" << tok << "]";
    }

    it++;
  }

  while (!ops.empty()) {
    string op2 = getstack (ops);
    double b = getstack (val), a = getstack (val);
    //cout << a << op2 << b << " = " << operate[op2](a, b) << endl;
    val.push  (operate[op2](a, b));
  }

  return !val.empty() ? val.top() : 0;
}
double calc2 (string expr) {

  return interpret (tokenize(expr));
}
*/
