#include <iostream>
#include <vector>
#include <cmath>
#include <regex>

using namespace std;

/*

<constant>  ::= [0-9]+ ( "." [0-9]+ )?
<variable>  ::= "x"
<func_name> ::= "sin" | "cos" | "tan" | "cot" | "log"

<expression> ::= <term> ( ( "+" | "-" ) <term> )*
<term>       ::= <factor> ( ( "*" | "/" ) <factor> )*
<factor>     ::= <basic> ( "^" <basic> )*
<func_call>  ::= <func_name> "(" <expression> ")"
<basic>      ::= <constant> | <variable> | <func_call> | ( "(" <expression> ")" )


-=[ rules of derivation ]=-
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

void display (vector<string> expr) {
    for (auto it : expr) {
        cout << "[" << it << "]";
    }
    cout << endl;
}
using func = function<double(double,double)>;

template<class T = void> struct power {
    const T operator ()(const T &lhs, const T &rhs) {
        return pow (lhs, rhs);
    }
};
map<string, func> operate {{"+", plus<double>()},{"-", minus<double>()},
              {"*", multiplies<double>()}, {"/", divides<double>()}, {"^", power<double>()} };

string clean (const string &expr) {
    if (expr.front() == '(' && expr.back() == ')') {

        return expr.substr(1, expr.size() - 2);
    } else {
        return expr;
    }
}
vector<string> tokenize (const string &expr) {
    regex re ("=>|_?[0-9]+(\\.[0-9]+)?|\\(.*\\)|[-+*/%()\\^=\\[\\]]|_?[a-zA-Z]+_?");
    // regex re ("=>|(_|-)?[0-9]+(\\.[0-9]+)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?");
    sregex_token_iterator it (expr.begin(), expr.end(), re);
    return vector<string> (it, sregex_token_iterator());
}
string calc (string a, string op, string b) {

    regex cst ("^-?[0-9]+(\\.[0-9]+)?$");
    if (regex_match (a, cst) && regex_match (b, cst)) {
        ostringstream os;
        os << operate[op] (stod (a), stod (b));
        return os.str();
    }

    if (op == "+") {
        if (a == b) { op = "*", a = "2"; }
        if (a == "0") return b;
        if (b == "0") return a;
    } else if (op == "-") {
        if (a == b) return "0";
        if (b == "0") return a;
    } else if (op == "*") {
        if (a == b) { op = "^", a = "2"; }
        if (a == "0" || b == "0") return "0";
        if (a == "1") return b;
        if (b == "1") return a;
    } else if (op == "/") {
        if (a == b) return "1";
        if (a == "0") return "0";
        if (b == "1") return a;
    } else if (op == "^") {
        if (a == "1" || b == "1") return a;
        if (b == "0") return "1";
        if (a == "0") return "0";
    }

    return "(" + a + op + b + ")";
}
string simpl (const string &expr) {

    if (expr.size() > 2) {
        vector<string> tok = tokenize (clean(expr));
        // cout << " => [" << expr << "]\n";
        if (tok.size() == 3) {
            return calc (tok[0], tok[1], tok[2]);
        }
    }
    return expr;
}
string differentiate ( const string &input) {

    regex cst ("^-?[0-9]+(\\.[0-9]+)?$");

    if (regex_match (input, cst)) return "0";
    if (input == "x") return "1";

    auto code = tokenize (clean(input));
    string arg1, arg2, dx1, dx2;

    for (auto &cell : code) cell = simpl(cell);

    if (code[0] == "cos") {
        arg1 = code[1], arg2 = "(sin " + arg1 + ")";
        string ex = "-" + differentiate (arg1);
        return calc (ex, "*", arg2);
    } else if (code[0] == "sin") {
        arg1 = code[1], arg2 = "(cos " + arg1 + ')';
        string ex = differentiate (arg1);
        return calc (ex, "*", arg2);
    } else if (code[0] == "exp") {
        arg2 = "(exp " + code[1] + ')';
        string ex = differentiate (code[1]);
        return calc (ex, "*", arg2);
    } else if (code[0] == "tan") {
        arg1 = "(cos " + code[1] + ')';
        arg2 = calc (arg1, "^", "2");
        string ex = differentiate (code[1]);
        return calc (ex, "/", arg2);
    } else if (code[0] == "cot") {

    } else if (code[0] == "log") {
        return calc ("1", "/", code[1]);
    } else if (code[0] == "ln") {
        return calc ("1", "/", code[1]);
    } else {
        arg1 = code[0], arg2 = code[2];
        dx1 = differentiate(arg1), dx2 = differentiate(arg2);
        // cout << arg1 << " " << arg2 << " " << dx1 << " " << dx2 << "\n";
        if (code[1] == "+") { //  add : a + b => a' + b'
            return calc (dx1, code[1], dx2);
        } else if (code[1] == "-") { //  add : a - b => a' - b'
            return calc (dx1, code[1], dx2);
        } else if (code[1] == "*") { //  mul : a * b => a'* b + a * b'
            string a = calc (dx1, "*", arg2), b = calc (dx2, "*", arg1);
            return calc (a, "+", b);
        } else if (code[1] == "/") { //  div : a / b => (a'* b − b'* a) / (b * b)
            string a = calc (dx1, "*", arg2), b = calc (dx2, "*", arg1);
            string nom = calc (a, "-", b), den = calc (arg2, "*", arg2);
            return calc (nom, "/", den);
        } else if (code[1] == "^") { // x² => 2 * x^(2-1) : * 2 (^ x (2 - 1))
          string arg3 = calc (arg1, "^", calc (arg2, "-", "1"));
          return calc (dx1, "*", calc (arg2, "*", arg3));
        }
    }

    return "";
}
void Assert (string error, string expect, string input) {

    string actual = differentiate (input);
    static int cycle;
    cycle++;

    if (actual != expect) {
        cout << "test " << cycle << "=>";
        cout << "\nactual => " << "[" << actual << "]\n";
        cout << "expect => " << "[" << expect << "]\n";
        //cout << error << endl;
    }
}
int main () {

    string input = "(x + 2)";

    // display(code);

    Assert ("x should return 1", "1", "x");
    Assert ("constant should return 0", "0", "5");
    Assert ("x+x should return 2", "2", "x+x");
    Assert ("x-x should return 0", "0", " x -x");
    Assert ("2*x should return 2", "2", "x * 2");

    Assert ("x/x should return 0", "0", "  x / x");
    Assert ("x/2 should return 0.5", "0.5", " x/ 2");

    Assert ("x*x should return 0", "(2*x)", " x* x");
    Assert ("x^2 should return 2*x", "(2*x)", " x^ 2");
    Assert ("x^0 should return 0", "0", "x^ 0");
    Assert ("x^1 should return 1", "1", "x^ 1");
    Assert ("x^3 should return 3x^2", "(3*(x^2))", " x^ 3");

    Assert ("ln(x) should return 1/x", "(1/x)", "ln x");
    Assert ("cos(x) should return -1 * sin(x)", "(-1*(sin x))", "cos x");

    Assert ("sin(x+1) should return cos(x+1)", "(cos (x+1))", "sin (x+ 1)");
    Assert ("(cos x)^0 should return 0", "0", "(cos x) ^ 0");
    Assert ("(sin x)^0 should return cos x", "(cos x)", "(sin x) ^1");

    Assert ("(cos x)^1 should return -sin x", "(-1*(sin x))", "(cos x)^ 1");
    Assert ("sin(x) should return cos(x)", "(cos x)", "sin x");

    Assert ("sin(2*x) should return 2*cos(2*x)", "(2*(cos (2*x)))", "sin (2 * x)");
    Assert ("cos(x+1) should return -1 * sin(x+1)", "(-1*(sin (x+1)))", "(cos (x+ 1))");

    Assert ("2*(x+2) should return 2", "2", "2 * (x + 2)");

    Assert ("x+(x+x) should return 3", "3", "x + (x + x)");
    Assert ("(x+x)-x should return 1", "1", "(x+ x) - x");
    Assert ("exp^(* 1 x) should return exp^x", "(exp x)", "exp (1 * x)");
    Assert ("exp(x) should return exp(x)", "(exp x)", "(exp x)");

    Assert ("exp(2*x) should return 2*exp(2*x)", "(2*(exp (2*x)))", "(exp (2 * x))");

    Assert ("tan (2x) must return 2 / (cos 2x)^2 ","(2/(cos (2*x)^2)", "(tan (2*x))");
    Assert ("(sin x)^3 should return 3.sin (x).cos (x)", "((cos x)*(3*((sin x)^2)))", "((sin x) ^  3)");
    /*

    */

    cout << "end\n";
}
