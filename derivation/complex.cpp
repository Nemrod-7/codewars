#include <iostream>
#include <regex>
#include <complex>
#include <cmath>

//  <variable>  ::= "x"
//  <constant>  ::= [0-9]+(.[0-9]+)?
//  <func_name> ::= "sin" | "cos" | "tan" | "cot" | "log"
//
//  <factor>     ::= <basic> ( "^" <basic> )*
//  <expression> ::= <term> ( "+" | "-"  <term> )*
//  <term>       ::= <factor> ( "*" | "/"  <factor> )*
//
//  <func_call>  ::= <func_name> "(" <expression> ")"
//  <basic>      ::= <constant> | <variable> | <func_call> | ( "(" <expression> ")" )
//
//  rules of derivation :
//  con : cst   => 0
//  lin : x     => 1
//  add : a + b => a' + b'
//  min : a - b => a' - b'
//  mul : a * b => a.b' + a'.b
//  div : a / b => (a'* b − b'* a) / (b * b)
//  pow : x^a   => a.x^(a - 1)
//  exp : e^x   => e^x
//  exp : a^x   => a^x . ln (a)
//  ln  : ln(x) => 1 / x
//  sin : sin x => cos x
//  cos : cos x => -sin x
//  tan : tan x => 1 / (cos²(x))

using namespace std;

//int order (const string &src) {
//    if (src == "+" || src == "-") return 1;
//    if (src == "*" || src == "/") return 2;
//    if (src == "^") return 3;
//    return 0;
//}
map<string,int> order {{"+",1},{"-",1},{"*",2},{"/",2},{"%",2},{"^",3}};

void show (const std::vector<std::string> &vs) {

    for (int i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }
}

vector<string> tokenize(const string &input) {
    regex tokn ("([0-9]+(\\.[0-9]+)?)|x|[-+*^]|(sin|cos|tan|cot|log)");
    sregex_token_iterator iter (input.begin (), input.end (), tokn);
    return vector<string> (iter, sregex_token_iterator ());
}
string join(vector<string> input) {
    string os;
    for (auto &it : input) os += it;
    return os;
}
string getstack(vector<string> &stack) {
    string val = stack.back();
    stack.pop_back();
    return val;
}

complex<double> stoc(const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
string ctos(const complex<double> &zx) {
    ostringstream oss;
    zx.imag() == 0 ? oss << zx.real() : oss << zx;
    return oss.str();
}

string operate(string a, string op, string b) {

    regex number ("^[0-9]+(\\.[0-9]+)?$");
    // cout << "[" << a << "] " << op << " [" << b << "]\n";
    if (regex_match(a, number) && regex_match(b, number)) {
        switch (op[0]) {
            case '+' : return ctos( stoc(a) + stoc(b)) ; break;
            case '-' : return ctos( stoc(a) - stoc(b)) ; break;
            case '*' : return ctos( stoc(a) * stoc(b)) ; break;
            case '/' : return ctos( stoc(a) / stoc(b)) ; break;
            case '^' : return ctos( pow(stoc(a), stoc(b))) ; break;
        }
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

    return a + op + b;
}

string evaluate(const string &expression, const string &value = "") {

    regex term ("^[-+*/^]$");
    regex number ("^[0-9]+(\\.[0-9]+)?$");

    const vector<string> code = tokenize(expression);
    vector<string> vars;
    vector<string> oper;
    bool running = true;
    int index = 0;
    /*cout << "(" << z1.real() << " + " << z1.imag() << "i)\n";*/

    while (running) {
        string token = code[index];

        if (token == "x") {
            /*cout << "variable : [" << token << "]";*/
            if (value == "") {
                vars.push_back(token);
            } else {
                vars.push_back(value);
            }
        } else if (regex_match(token, number)) {
            /*cout << "number   : [" << token << "]";*/
            vars.push_back(token);
        } else if (regex_match(token, term)) {
            /*cout << "operator : [" << token << "]";*/

            while (!oper.empty() && order[oper.back()] > order[token]) {
                string z2 = getstack(vars), z1 = getstack(vars);
                string op = getstack(oper);
                vars.push_back(operate(z1,op,z2));
            }

            oper.push_back(token);
        }

        index++;
        if (index == code.size()) {
            running = false;
        }
    }

    while (!oper.empty()) {
        string z2 = getstack(vars), z1 = getstack(vars);
        string op = getstack(oper);
        vars.push_back(operate(z1,op,z2));
    }

    return vars.back();
}
string differentiate (const string &input) {

    regex term ("^[-+*/^]$");
    regex numr ("^-?[0-9]+(\\.[0-9]+)?$");
    regex func ("^[sin|cos|tan|cot|log]$");

    if (input == "x") return "1";
    if (regex_match(input, numr)) return "0";

    vector<string> code = tokenize(input);


    for (int i = 0; i < code.size(); i++) {
        if (order[code[i]]) {
            string t1 = join({code.begin(), code.begin() + i});
            string t2 = join({code.begin() + i + 1, code.end()});
            // cout << " => " << res << "\n";

            if (code[i] == "+") { // add : a + b => a' + b'
                string dx1 = differentiate(t1), dx2 = differentiate(t2);

                return operate(dx1, code[i], dx2);
            } else if (code[i] == "-") { // min : a - b => a' - b'
                string dx1 = differentiate(t1), dx2 = differentiate(t2);

                return operate(dx1, code[i], dx2);
            } else if (code[i] == "*") { // mul : a * b => a.b' + a'.b
                string dx1 = operate(t1, "*", differentiate(t2));
                string dx2 = operate(differentiate(t1), "*", t2);

                return operate(dx1, "+", dx2);
            } else if (code[i] == "/") { // div : a / b => (a'* b − b'* a) / (b * b)
                string dx1 = operate(t1, "*", differentiate(t2));
                string dx2 = operate(differentiate(t1), "*", t2);
                string dx3 = operate(dx1, "-", dx2);
                string dx4 = operate(t2, "*", t2);

                return operate (dx3, "/", dx1);
            } else if (code[i] == "^") { // pow : x^a   => a.x^(a - 1)
                string dx1 = operate (t2, "-", "1");
                string dx2 = operate (t2, "*", t1);
                // cout << "[" << t1 << "] " << code[i] << " [" << dx1 << "]";

                return operate (dx2, "^", dx1);
            }
        } else if (code[i] == "cos") { // cos : cos x => -sin x

        } else if (code[i] == "sin") { // sin : sin x => cos x

        } else if (code[i] == "tan") { // tan : tan x => 1 / (cos²(x))

        } else if (code[i] == "log") { // ln  : ln(x) => 1 / x

        }
    }


    return "";
}

int main () {

    /*engine curr("2 * x^3");*/
    // (2) * (x^3) => a.b' + a'.b
    // (x) ^ (3)   => a.x^(a - 1)
    vector<string> code = {"2","*","x","^","3"};
    vector<string> t1,t2;
    t1 = {"2"}, t2 = {"x","^","3"} ;

    string actual = differentiate("x + 3");

    cout << actual;

}
