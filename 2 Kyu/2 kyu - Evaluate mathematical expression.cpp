#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <algorithm>
#include <functional>

#include <chrono>

using namespace std;

using func = function<double(double,double)>;

map<string,int> order {{"+",1},{"-",1},{"*",2},{"/",2},{"%",2}};
map<string,func> operate {{"+", plus<double>()},{"-", minus<double>()},{"*", multiplies<double>()},{"/", divides<double>()}};
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
void display (vector<string> expr) {
    for (auto it : expr) {
        cout << "[" << it << "]";
    }
    cout << endl;
}
////////////////////////////////////////////////////////////////////////////////
vector<string> tokenize (const string &expr) {
  // regex token ("\\*|\\+|/|-?[0-9]+(\\.[0-9]+)?|-|\\(|\\)");
  regex token ("-?[0-9]+(\\.[0-9]+)?|[/*+%()-]");
  sregex_token_iterator it (expr.begin(), expr.end(), token);
  return vector<string> (it, sregex_token_iterator());
}
template<class T> T getstack (vector<T> &S) {
    if (S.empty()) return 0;
    T val = S.back();
    S.pop_back();
    return val;
}

double pass1 (vector<string> expr) {

    regex number ("^-?[0-9]+(\\.[0-9]+)?$");
    vector<string>::iterator it = expr.begin();
    vector<string> oper;
    vector<double> vars;
    // display(expr);

    while (it - expr.begin() < expr.size()) {

        int sign = 1;

        if (*it == "-" && order[*(it - 1)]) {
            sign = -1;
            it++;
        }

        string token = *it;
        cout << "[" << token <<"]"  << " ";

        if (regex_match(token, number)) {
            vars.push_back(stod(token));
        } else if (token == "(") {
            auto ng = it + 1;
            int pile = 0;

            while (true) {
                pile += (*it == "(") - (*it == ")");
                if (pile == 0) break;
                it++;
            }

            vars.push_back(pass1 ({ng,it}) * sign);

        } else if (token != ")") {
            cout << oper.size() << " " << vars.size();

            while (!oper.empty() && order[oper.back()] >= order[token]) {
                double b = getstack (vars), a = getstack (vars);
                string op = getstack (oper);
                vars.push_back(operate[op] (a,b));
                // cout << a << op << b << " => " << vars.back() << "\n";
                // cout << op << "|";

            }
            oper.push_back(token);
        }
        cout << "\n";
        it++;
    }

    while (!oper.empty()) {
        double b = getstack (vars), a = getstack (vars);
        vars.push_back(operate[getstack (oper)] (a,b));
    }

    return !vars.empty() ? vars.back() : 0;
}
double calc2 (const string &input) {

    vector<string> expr = tokenize (input);
    regex number ("^-?[0-9]+(\\.[0-9]+)?$");
    vector<string>::iterator it = expr.begin();
    vector<string> oper;
    vector<double> vars;
    // display(expr);
    while (it - expr.begin() < expr.size()) {

        int sign = 1;

        if (*it == "-" && order[*(it - 1)]) {
            sign = -1;
            it++;
        }

        string token = *it;

        if (regex_match(token, number)) {
            vars.push_back(stod(token));
        } else if (token == "(") {
            auto ng = it + 1;
            int pile = 0;

            while (true) {
                pile += (*it == "(") - (*it == ")");
                if (pile == 0) break;
                it++;
            }

            // vars.push_back(calc2 ({ng,it}) * sign);
        } else if (token != ")") {

            while (!oper.empty() && order[oper.back()] >= order[token]) {
                double b = getstack (vars), a = getstack (vars);
                string op = getstack (oper);
                vars.push_back(operate[op] (a,b));
                // cout << a << op << b << " => " << vars.back() << "\n";
                // cout << op << "|";
            }
            oper.push_back(token);
        }

        it++;
    }

    while (!oper.empty()) {
        double b = getstack (vars), a = getstack (vars);
        vars.push_back(operate[getstack (oper)] (a,b));
    }

    return !vars.empty() ? vars.back() : 0;
}
double calc (string input) {

  return pass1 (tokenize(input));
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();

    Assert::That(calc ("2 + 3 * 4 / 3 - 6 / 3 * 3 + 8"), 8.0);

    Assert::That(calc ("(123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) - (123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20) + (13 - 2)/ -(-11)"), Equals(1.0));
    Assert::That(calc ("(64) * (-97 / 51 * -(55)) - (21 * -((((71 - -33)))) - 44)"), 8922.901960784315);
    Assert::That(calc (""), 0.0);
    Assert::That(calc ("3 -(-1)"), Equals(4.0));

    Assert::That(calc ("-7 * -6 / 3"), Equals(14.0));
    Assert::That(calc ("10- 2- -5"), Equals(13.0));

    Assert::That(calc ("8/16"), Equals(0.5));
    Assert::That(calc ("2 + -2"), Equals(0.0));
    Assert::That(calc ("3 * 5"), Equals(15.0));
    Assert::That(calc ("1 + 1 / 3"), 1.3333333333333333);
    Assert::That(calc ("(1 + 1) / 3"), 0.6666666666666666);

    Assert::That(calc ("1 + 1"), Equals(2.0));
    Assert::That(calc ("6 + - (-4)"), 10.0);
    Assert::That(calc ("6 + - (4)"), 2.0);
    Assert::That(calc ("(((10)))"), Equals(10.0));

    // Assert::That(calc ("63 - -20 / 84 - 33 * 87 + -17 * 41 + -10"), -3514.761905);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////

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
    pile += (*it == '(') - (*it == ')');
    if (pile == 0) return sub;

    sub += *it;
  }
  return sub;
}
