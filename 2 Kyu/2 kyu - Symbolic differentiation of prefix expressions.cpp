#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <functional>
#include <regex>
//#include <utility>

using namespace std;

/* rules of derivation

con : cst   => 0
lin : x     => 1
add : a + b => a' + b'
min : a - b => a' - b'
mul : a * b => a.b' + a'.b
div : a / b => (a'* b − b'* a) / (b * b)
pow : x^a   => a.x^(a - 1)
exp : e^x   => e^x
exp : a^x   => a^x . ln (a)
ln  : ln(x) => 1 / x
sin : sin x => cos x
cos : cos x => -sin x
tan : tan x => 1 / (cos²(x))

*/

using func = function<double(double,double)>;

template<class T = void> struct power {
    const T operator ()(const T &lhs, const T &rhs) {
        return pow (lhs, rhs);
    }
};

map<string, func> oper {{"+", plus<double>()},{"-", minus<double>()},
              {"*", multiplies<double>()}, {"/", divides<double>()}, {"^", power<double>()} };

vector<string> tokenize3 (string expr) {

  vector<string> tok;
  expr.pop_back();

  const regex pattern ("\\w+|(\\(.*?\\))|[*/^+-]");
  auto it = sregex_iterator (expr.begin() + 1, expr.end(), pattern), end = sregex_iterator();

  while (it != end) {
      tok.push_back (it->str());
      it++;
  }

  return tok;
}
string calc (string a, string op, string b) {
    ostringstream os;
    regex num ("-?[0-9]*");

    if (a == "0" || b == "0") {
        if (op == "*") return "0";
        if (op == "+") return a == "0" ? b : a;
        if (op == "-" && b == "0") return a;
    }

    if (a == "1" || b == "1") {
        if (op == "*") return a == "1" ? b : a;
        if (op == "^") return a;
        if (op == "/" && b == "1") return a;
    }

    if (regex_match (a, num) && regex_match (b, num)) {
        os << oper[op] (stod (a), stod (b));
    } else {
        os << "(" << op << " " << a << " " << b << ")";
    }
    return os.str();
}
string diff (const string &src) {

    if (count (src.begin(), src.end(), ' ') == 0) {
        return (src == "x") ? "1" : "0";
    }

    vector expr = tokenize3 (src);
    string op = expr.front();


    if (oper[op]) {

        string arg1 = expr[1], arg2 = expr[2];
        string dx1 = diff (arg1), dx2 = diff (arg2);

        if (op == "+") {                   //  add : a + b => a' + b'
            return calc (dx1, op, dx2);
        } else if (op == "-") {            //  add : a - b => a' - b'
            return calc (dx1, op, dx2);
        } else if (op == "*") {            //  mul : a * b => a'* b + a * b'
            string a = calc (dx1, op, arg2), b = calc (arg1, op, dx2);
            return calc (a, "+", b);
        } else if (op == "/") {            //  div : a / b => (a'* b − b'* a) / (b * b)
            string a = calc (dx1, "*", arg2), b = calc (dx2, "*", arg1);
            string nom = calc (a, "-", b), den = calc (arg2, "^", "2");

            return calc (nom, "/", den);
        } else if (op == "^") {            // x² => 2 * x^(2-1) : * 2 (^ x (2 - 1))
            string ex = calc (arg2, "-", "1");
            arg1 = calc ("x", "^", ex);

            return calc (arg2, "*", arg1);
        }
    } else {
        string arg1 = expr[1], arg2 = arg1 + ")";

        if (op == "ln") {
            return calc ("1", "/", arg1);
        }
        if (op == "cos") {
            string ex = "-" + diff (arg1);
            arg2 = "(sin " + arg1 + ")";
            return calc (ex, "*", arg2);
        }
        if (op == "sin") {
            string ex = diff (arg1);
            arg2 = "(cos " + arg1 + ')';
            return calc (ex, "*", arg2);
        }
        if (op == "exp") {
            string ex = diff (arg1);
            arg2 = "(exp " + arg1 + ')';
            return calc (ex, "*", arg2);
        }
        if (op == "tan") {
            string ex = diff (arg1);
            arg2 = "(cos " + arg1 + ')';
            arg2 = calc (arg2, "^", "2");
            return calc (ex, "/", arg2);
        }
    }

    return "";
}

void Assert (string error, string expect, string input) {

    string actual = diff (input);

    if (actual != expect) {
        cout << "\nactual => " << "[" << actual << "] ";
        cout << "expect => " << "[" << expect << "] ";

        //cout << error << endl;
    }
}
void Test () {

  /*

  std::string result = diff("(tan x)");
        Assert::That(result, Equals("(+ 1 (^ (tan x) 2))") || Equals("(^ (cos x) -2)") || Equals("(/ 1 (^ (cos x) 2))"));

    */
}

int main () {

    ostringstream os;
    double val = 4;

    // Assert ("x^2 should return 2*x", "(* 2 x)", "(^ x 2)");
    // (^ x 3) => (* 3 (^ x 2))

    //std::string result = diff("tan (* 2 x)"); // => expected : "(* 2 (/ 1 (^ (tan (* 2 x)) 2)))"
    //cout << result;
    // Expected: equal to "(* 22 (* -1 (sin (* 22 x))))" or equal to "(* -22 (sin (* 22 x)))"
    //cout << diff ("(cos (* 22 x))"); //

    // cout << calc ("1", "^", "x");
  //  Assert::That(result, Equals("(* 3 (* 2 x))") || Equals("(* 6 x)"));
    Assert ("constant should return 0", "0", "5");
    Assert ("x should return 1", "1", "x");
    Assert ("x+x should return 2", "2", "(+ x x)");
    Assert ("x-x should return 0", "0", "(- x x)");
    Assert ("2*x should return 2", "2", "(* x 2)");
    Assert ("x/2 should return 0.5", "0.5", "(/ x 2)");
    Assert ("x+(x+x) should return 3", "3", "(+ x (+ x x))");
    Assert ("(x+x)-x should return 1", "1", "(- (+ x x) x)");

    Assert ("2*(x+2) should return 2", "2", "(* 2 (+ x 2))");
    Assert ("x^2 should return 2*x", "(* 2 x)", "(^ x 2)");
    Assert ("ln(x) should return 1/x", "(/ 1 x)", "(ln x)");
    Assert ("cos(x) should return -1 * sin(x)", "(* -1 (sin x))", "(cos x)");

    Assert ("sin(x) should return cos(x)", "(cos x)", "(sin x)");
    Assert ("sin(x+1) should return cos(x+1)", "(cos (+ x 1))", "(sin (+ x 1))");
    Assert ("sin(2*x) should return 2*cos(2*x)", "(* 2 (cos (* 2 x)))", "(sin (* 2 x))");
    Assert ("cos(x+1) should return -1 * sin(x+1)", "(* -1 (sin (+ x 1)))", "(cos (+ x 1))");

    Assert ("exp(2*x) should return 2*exp(2*x)", "(* 2 (exp (* 2 x)))", "(exp (* 2 x))");
    Assert ("exp(x) should return exp(x)", "(exp x)", "(exp x)");

    Assert ("tan (2x) must return 2 / (cos 2x)^2 ","(/ 2 (^ (cos (* 2 x)) 2))", "(tan (* 2 x))");


    /*
    */
    cout << "\nend";

}

bool isnum (const string &exp) {
    for (auto &it : exp) {
        if (isalpha (it)) return false;
        if (isspace (it)) return false;
        if (oper[{it}]) return false;
    }
    return true;
}
vector<string> tokenize (const string &src) {
  string expr;

  if (src.front() == '(' && src.back() == ')') {
      expr = src.substr (1, src.size() - 2);
  } else {
      expr = src;
  }
  string::iterator it = expr.begin ();
  vector<string> tok;

  while (it < expr.end()) {
      string buff;

      if (*it == '(') {
          int pile = 0;

          do {
              if (*it == '(') pile++;
              if (*it == ')') pile--;
              buff += *it++;
          } while (pile);

      } else {
          while (*it && *it != ' ') buff += *it++;
      }
      tok.push_back(buff);
      it++;
  }
  return tok;
}
vector<string> tokenize2 (string expr) {
  string::iterator it = expr.begin () + 1;
  vector<string> tok;
  expr.pop_back();

  while (it < expr.end()) {
      string buff;

      if (*it == '(') {
          do { buff += *it; } while (*it++ != ')');
      } else {
          while (*it && *it != ' ') buff += *it++;
      }
      tok.push_back(buff);
      it++;
  }

  return tok;
}
int parenthesis (const string &src) {
  int cnt = count (src.begin(), src.end(), '(');
  cnt -= count (src.begin(), src.end(), ')');
  return cnt;
}
string format (const vector<string> &number) {
  stringstream os;

  int i, cnt = 0;
  int minx = 99, maxx = 0;

  for (i = 0; i < 4; i++) {
    if (number[i] != "") {
      cnt++;
      minx = min (minx, i);
      maxx = max (maxx, i + 1);
    }
  }

  if (cnt == 1) return number[1];

  i = minx;
  if (number[0] == "*" && number[1] == "1") i = 2;

  if (number[i].front() != '(') {
    //cout << cnt << endl;
    //cout << "[" << number[i] << "]\n";
    os << "(";
  }
  for (; i < maxx; i++) {
    os << number[i];
    if (i < maxx - 1) os << " ";
  }

  cnt = parenthesis (os.str());
  while (cnt-->0) os << ')';

  return os.str();
}
double num (const string &var) {
  string val;
  for (auto &it : var) {
    if (it == '-' || isdigit (it) || it == '.') {
      val += it;
    } else return 0;
  }
  return stod (val);
}
string mkex (const string &a, const string &op, const string &b) {
  return '(' + op + ' '+ a + ' ' + b + ')';
}
string calc2 (const vector<string> &num) {
    //vector<string> num = tokenize (src);
    ostringstream os;
    string op = num[0], arg1 = num[1], arg2 = num[2];
    if (arg1 == "0" || arg2 == "0") {
        if (op == "*") return "0";
        if (op == "+") return arg1 == "0" ? arg2 : arg1;
    }

    if (op == "*") {
        if (arg1 == "1") {
            return arg2;
        } else if (arg2 == "1") {
            return arg1;
        }
    }

    if (isnum(arg1) && isnum(arg2)) {
        os << oper[op] (stod (arg1), stod (arg2));
    } else {
        os << "(" << op << " " << arg1 << " " << arg2 << ")";
    }
    return os.str();
  }
string derivate2 (const string &src) {
    vector expr = tokenize (src);
    string op = expr.front();
    stringstream val;
    vector<string> number (4);

    if (expr.size() < 2) {
        return (op == "x") ? "1" : "0";
    }

    if (oper[op]) {
        double a, b, c;
        string arg1 = expr[1], arg2 = expr[2];
        string dx1 = derivate2 (arg1), dx2 = derivate2 (arg2);

        if (op == "+") {                  //  add : a + b => a' + b'
            val << oper[op] (num (dx1), num (dx2));
        } else if (op == "-") {                 //  add : a - b => a' - b'
            val << oper[op] (num (dx1), num (dx2));
        } else if (op == "*") {                //  mul : a * b => a'* b + a * b'
            a = oper[op] (num (dx1), num (arg2));
            b = oper[op] (num (arg1), num (dx2));
            val << a + b;
        } else if (op == "/") {               //  div : a / b => (a'* b − b'* a) / (b * b)
            a = num (dx1) * num (arg2);
            b = num (arg1) * num (dx2);
            c = num (arg2) * num (arg2);

            val << oper[op] ((a - b), c);
        } else if (op == "^") {
            vector<string> tmp (4);
            // x² => 2 * x^(2-1) : * 2 (^ x (2 - 1))
            if (num (arg2)) {

                double nb = num (arg2) - 1;
                val << arg2;

                if (nb > 1) {
                //    exp << (nb);
                }

            } else {
                //tmp[3] = // (^ x (2 - 1)) => "^ " + arg1 + " - 1";

            }
            number[0] = "*";
            number[2] = arg1;
            //tmp[3] = arg2;
        }
        /*
        cout << dx1 << " * " << arg2 << "  -  ";
        cout << arg1 << " * " << dx2 << endl;
        */
        number[1] = val.str();
    } else {
        string arg1 = expr[1], arg2;

        if (op == "sin") {
            arg2 = "(cos " + arg1 + ')';
            number = {"*", derivate2 (arg1), arg2};
            //cout << "[" << op << "][" << arg1 << "]";
        }
        if (op == "cos") {
            arg2 = "(sin " + arg1 + ")";
            number = {"*", "-1", arg2};
        }
        if (op == "ln") {
            number = {"/", "1", arg1};
        }
        if (op == "exp") {
            arg2 = "(exp " + arg1 + ')';
            number = {"*", derivate2 (arg1), arg2};
        }
    }

    return format (number);
  }
