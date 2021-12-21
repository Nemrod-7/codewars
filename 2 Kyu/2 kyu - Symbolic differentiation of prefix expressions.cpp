#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <functional>

using namespace std;
using funct = function<double(double,double)>;

enum types {NUL, VAR, NUM, SUB, OPE, FUN};

map<string,funct> oper2 {{"+", plus<double>()},{"-", minus<double>()},
              {"*", multiplies<double>()}, {"/", divides<double>()},{"^",{}}};

map<string,int> func {{"exp",1},{"ln",1},{"sin",1},{"cos",1},{"tan",1}};
map<string,int> oper {{"+", 1},{"-",1},{"*",2},{"/",2},{"^",3}};
// + - * / ^ cos sin tan exp ln

/* rules of derivation

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
int identify (string src) {
    if (oper[src]) return OPE;
    if (func[src]) return FUN;
    if (src.front() == '(') return SUB;

    bool alp = 0, dig = 0;
    for (auto &it : src) {
        if (isdigit (it)) dig = true;
        if (isalpha (it)) alp = true;
    }
    if (dig && alp)  return SUB;
    if (dig == true) return NUM;
    if (alp == true) return VAR;

    return 0;
}
string interpret (const string &src) {
    vector expr = tokenize (src);

    string os, op = expr.front(), arg1, arg = expr.back();
    int type = identify (op);

    os += '(';

    switch (type) {
        case OPE : {
            arg1 = expr[1];

            if (op == "+") {

            }
            if (op == "-") {

            }
            if (op == "*") {

            }
            if (op == "/") {
              
            }
            cout << "[" << arg << "]";
            cout << "[" << op << "]";
            cout << "[" << arg1 << "]";
            break;
        }
        case FUN : {

            if (op == "cos") {       // cos : cos x => -sin x
                os += "* -1 ";

                os += "(sin " + arg + ")";

            } else if (op == "sin") { // sin : sin x => cos x

                os += "cos " + arg;
            } else if (op == "ln") {  // ln  : ln(x) => 1 / x

                os += "/ 1 " + arg;
            } else if (op == "exp") { // exp : a^x   => a^x . ln (a)

            } else if (op == "tan") { // tan : tan x => 1/(cos²(x))

            }
            break;
        }
        case NUM : return "0";
        case VAR : return "1";

        default : break;
    }
    os += ')';

    return os;
}
void Assert (string error, string expect, string input) {

    string actual = interpret (input);

    if (actual != expect) {
        cout << "actual => " << "[" << actual << "] ";
        cout << "expect => " << "[" << expect << "] ";

        cout << error << endl;
    }
}

int main () {
    // (* (+ x 3) 5)  // same as 5 * (x + 3)
    string expr = "+ -2 x";
    // expr = "* 2 (+ x 1)";
    interpret (expr);


    //Assert ("x^2 should return 2*x", "(* 2 x)", "(^ x 2)");

    //expr = "* - A / B C - / A K L";
    //expr = "+ x 1";
}

void Test () {
  /*
        Assert ("constant should return 0", "0", "5");
        Assert ("x should return 1", "1", "x");
        Assert ("x+x should return 2", "2", "(+ x x)");
        Assert ("x-x should return 0", "0", "(- x x)");
        Assert ("2*x should return 2", "2", "(* x 2)");
        Assert ("x/2 should return 0.5", "0.5", "(/ x 2)");
        Assert ("x+(x+x) should return 3", "3", "(+ x (+ x x))");
        Assert ("(x+x)-x should return 1", "1", "(- (+ x x) x)");

        Assert ("2*(x+2) should return 2", "2", "(* 2 (+ x 2))"); // ?????

        Assert ("x^2 should return 2*x", "(* 2 x)", "(^ x 2)");

        Assert ("cos(x) should return -1 * sin(x)", "(* -1 (sin x))", "(cos x)");
        Assert ("sin(x) should return cos(x)", "(cos x)", "(sin x)");
        Assert ("sin(x+1) should return cos(x+1)", "(cos (+ x 1))", "(sin (+ x 1))"),
        Assert ("cos(x+1) should return -1 * sin(x+1)", "(* -1 (sin (+ x 1)))", "(cos (+ x 1))"),

        Assert ("exp(x) should return exp(x)", "(exp x)", "(exp x)");
        Assert ("ln(x) should return 1/x", "(/ 1 x)", "(ln x)")};
        Assert ("sin(2*x) should return 2*cos(2*x)", "(* 2 (cos (* 2 x)))", "(sin (* 2 x))"),
        Assert ("exp(2*x) should return 2*exp(2*x)", "(* 2 (exp (* 2 x)))", "(exp (* 2 x))")};

        std::string result = diff("(tan x)");
        Assert::That(result, Equals("(+ 1 (^ (tan x) 2))") || Equals("(^ (cos x) -2)") || Equals("(/ 1 (^ (cos x) 2))"));

        mktest ("2/(1+x) should return -2/(1+x)^2", "(/ -2 (^ (+ 1 x) 2))", "(/ 2 (+ 1 x))"),

    */
}
////////////////////////////////////////////////////////////////////////////////
/*
struct token {
    int type;
    string ex;
};

string getstack (stack<string> &s1) {
    string os = s1.top();
    s1.pop();
    return os;
}

string getsub2 (string::iterator &it) {
    int pile = 1;
    string sub;

    while (*it--) {
        if (*it == '(') pile--;
        if (*it == ')') pile++;
        if (pile == 0) return sub;

        sub += *it;
    }
    return sub;
}
string interpret2 (string expr) {
    string::iterator it = expr.end();
    bool running = true;
    stack<string> s1;

    while (running) {
        string var;
        bool flag = false;

        if (*it == ')') {
            var = getsub2 (it);
            flag = true;
        } else {
            while (*it && *it != ' ') var += *it--;
        }

        if (var.size()){
            reverse (var.begin(), var.end());

            //cout << "[" << var << "]";
            if (flag) {
                var = interpret2(var);
            }

            if (oper[var]) {
                string op1 = getstack (s1), op2 = getstack(s1);
                string temp = "(" + op1 + var + op2 + ")";

                var = temp;
            }

            s1.push (var);
        }

        if (it == expr.begin() - 1) running = false;
        it--;
    }

    return s1.top();
  }


  // Convert prefix to Infix expression
  string preToInfix(string expr) {

  string::iterator it = expr.end();
  stack<string> s1;
  bool running = true;
  // length of expression
  int length = expr.size();
  // reading from right to left
  while (it != expr.begin() - 1) {
  cout << *it << " ";
  // check if symbol is operator
  if (oper[*it]) {

  // pop two operands from stack
  string op1 = getstack (s1), op2 = getstack (s1);
  // concat the operands and operator
  string temp = "(" + op1 + *it + op2 + ")";

  // Push string temp back to stack
  s1.push(temp);
} else { // if symbol is an operand
// push the operand to the stack
s1.push(string (1, *it));
}
it--;
}

// Stack now contains the Infix expression
return s1.top();
}

*/
