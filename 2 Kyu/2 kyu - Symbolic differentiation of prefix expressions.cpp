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

  expr.pop_back();

  regex re ("\\w+|-?[0-9]+|(\\(.*?\\))|[*/^+-]");
  sregex_token_iterator it (expr.begin() + 1, expr.end(), re);
  return vector<string> (it, sregex_token_iterator ());
}

string calc (string a, string op, string b) {

    regex num ("^-?[0-9]*$");
    if (regex_match (a, num) && regex_match (b, num)) {
        ostringstream os;
        os << oper[op] (stod (a), stod (b));
        return os.str();
    }

    if (op == "+") {
        if (a == "0") return b;
        if (b == "0") return a;
    } else if (op == "-") {
        if (b == "0") return a;
    } else if (op == "*") {
        if (a == "0" || b == "0") return "0";
        if (a == "1") return b;
        if (b == "1") return a;
    } else if (op == "/") {
        if (b == "1") return a;
    } else if (op == "^") {
        if (a == "1" || b == "1") return a;
    }

    return "(" + op + " " + a + " " + b + ")";
}

string diff (const string &src) {

    regex num ("^-?[0-9]*$");

    if (regex_match (src, num)) return "0";
    if (src == "x") return "1";

    vector expr = tokenize3 (src);
    string op = expr.front();
    /*
    for (auto it : expr) {
        cout << "[" << it << "]";
    }
    cout << endl;
    */
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

    /*

    // diff ("(^ (sin x) 3)");
    (* (cos x) (* 3 (^ (sin x) 2)))
    (* (* 3 (^ (sin x) 2)) (cos x))

    */
    cout << diff ("(^ (sin x) 3)");

    // Assert ("x^2 should return 2*x", "(* 2 x)", "(^ x 2)");
    // (^ x 3) => (* 3 (^ x 2))

    //std::string result = diff("tan (* 2 x)"); // => expected : "(* 2 (/ 1 (^ (tan (* 2 x)) 2)))"
    //cout << result;
    // Expected: equal to "(* 22 (* -1 (sin (* 22 x))))" or equal to "(* -22 (sin (* 22 x)))"
    //cout << diff ("(cos (* 22 x))"); //

    // cout << calc ("1", "^", "x");
  //  Assert::That(result, Equals("(* 3 (* 2 x))") || Equals("(* 6 x)"));
  /*

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

    */

    cout << "\nend";

}

string calc2 (string a, string op, string b) {
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
        return to_string (oper[op] (stod (a), stod (b)));
      //  os << oper[op] (stod (a), stod (b));
    } else {
        return "(" + op + " " + a + " " + b + ")";
        //os << "(" << op << " " << a << " " << b << ")";
    }
    return os.str();
}
