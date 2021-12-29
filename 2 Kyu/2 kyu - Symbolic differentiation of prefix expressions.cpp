#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
#include <utility>

using namespace std;

/* rules of derivation

add : a + b => a' + b'
min : a - b => a' - b'
mul : a * b => a.b' + a'.b
div : a / b => (a'* b − b'* a) / (b * b)
con : const => 0
lin : x     => 1
pow : x^a   => a.x^(a - 1)
exp : e^x   => e^x
exp : a^x   => a^x . ln (a)
ln  : ln(x) => 1 / x
sin : sin x => cos x
cos : cos x => -sin x
tan : tan x => 1/(cos²(x))

*/

using func = function<double(double,double)>;
map<string, func> oper {{"+", plus<double>()},{"-", minus<double>()},
              {"*", multiplies<double>()}, {"/", divides<double>()}, {"^", multiplies<double>()} };

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
bool isnum (const string &exp) {
    for (auto &it : exp) {
        if (isalpha (it)) return false;
        if (isspace (it)) return false;
        if (oper[{it}]) return false;
    }
    return true;
}
int parenthesis (const string &src) {
    int cnt = count (src.begin(), src.end(), '(');
    cnt -= count (src.begin(), src.end(), ')');
    return cnt;
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
string derivate (const string &src) {
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
        string dx1 = derivate (arg1), dx2 = derivate (arg2);

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
            number = {"*", derivate (arg1), arg2};
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
            number = {"*", derivate (arg1), arg2};
        }
    }

    return format (number);
}

void Assert (string error, string expect, string input) {

    string actual = derivate (input);

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

    string input = "(* (* (cos x) 3) (^ (sin x) 2))";
    vector<string> expr = tokenize (input); // "(/ -2 (^ (+ 1 x) 2))"
    // div : a / b => (a'* b − b'* a) / b^2
    string op = expr[0], arg1 = expr[1], arg2 = expr[2];

    double a, b;
    if (arg1 == "x") {
        a = 0;
    } else if (isnum (arg1)) {
        a = stod (arg1);
    } else {

    }

    if (arg2 == "x") {
        a = 0;
    } else if (isnum (arg2)) {
        a = stod (arg2);
    } else {

    }

    //string dx1 = derivate (arg1), dx2 = derivate (arg2);

    vector<string> numb(4);



    //cout << mkex ("-2","/" , mkex (arg2, "^", "2"));
    //cout << arg1 << " " << stod (arg1) << endl;

    //Assert ("2/(1+x) should return -2/(1+x)^2", "(/ -2 (^ (+ 1 x) 2))", "(/ 2 (+ 1 x))");
    /*
    Assert ("x/2 should return 0.5", "0.5", "(/ x 2)");



    Assert ("constant should return 0", "0", "5");
    Assert ("x should return 1", "1", "x");
    Assert ("x+x should return 2", "2", "(+ x x)");
    Assert ("x-x should return 0", "0", "(- x x)");
    Assert ("2*x should return 2", "2", "(* x 2)");
    Assert ("x+(x+x) should return 3", "3", "(+ x (+ x x))");
    Assert ("(x+x)-x should return 1", "1", "(- (+ x x) x)");

    Assert ("2*(x+2) should return 2", "2", "(* 2 (+ x 2))"); // ?????
    Assert ("x^2 should return 2*x", "(* 2 x)", "(^ x 2)");

    Assert ("sin(x) should return cos(x)", "(cos x)", "(sin x)");
    Assert ("sin(x+1) should return cos(x+1)", "(cos (+ x 1))", "(sin (+ x 1))");
    Assert ("sin(2*x) should return 2*cos(2*x)", "(* 2 (cos (* 2 x)))", "(sin (* 2 x))"),


    Assert ("cos(x) should return -1 * sin(x)", "(* -1 (sin x))", "(cos x)");
    Assert ("cos(x+1) should return -1 * sin(x+1)", "(* -1 (sin (+ x 1)))", "(cos (+ x 1))");

    Assert ("exp(x) should return exp(x)", "(exp x)", "(exp x)");
    Assert ("exp(2*x) should return 2*exp(2*x)", "(* 2 (exp (* 2 x)))", "(exp (* 2 x))");
    Assert ("ln(x) should return 1/x", "(/ 1 x)", "(ln x)");

    */

    cout << "end";

    /*
    */

}
