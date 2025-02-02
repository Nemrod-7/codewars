#include <iostream>

#include <vector>
#include <cmath>

#include <tuple>
#include <complex>
#include <functional>

// current solution without regex

// Let f be a function.
// The derivative function, denoted by f′, is the function whose domain consists of those values of x
// such that the following limit exists:
// f′(x) = lim h→0 of (f(x + h) − f(x)) / h.
//
// cst : 1     => 0
// lin : x     => 1
// add : a + b => a' + b'
// min : a - b => a' - b'
// mul : a * b => a.b' + a'.b
// div : a / b => (a'* b − b'* a) / (b * b)
// exp : x^y   => x^y . (x'.(y/x) + y'.log(x))
// log : ln(x) => x' / x
// sin : sin x => x'.cos x
// cos : cos x => -x'.sin x
// tan : tan x => x' / (cos(x))^2
// cot = cot x = -x' / (sin(x))^2

using namespace std;
using value_t = std::complex<double>;
using func_t = std::function<value_t(value_t)>;

/////////////////////////////////////////////////////////////////////////////////////////
string showvect (const std::vector<std::string> &vs) {
    string os;
    for (size_t i = 0; i < vs.size(); i++) {
      os += "[" + vs[i] + "]";
    }

    return os;
}
string showtransform (string t1, string op, string t2, string res) {
    string os = "[" + t1 + "]" + op + "[" + t2 + "]" ;
    os += " => [";
    os += res;
    // os += showvect(code);
    return os + "]";
}
/////////////////////////////////////////////////////////////////////////////////////////
complex<double> operator ^ (const complex<double> &a, const complex<double> &b) {
    return pow(a,b);
}

bool is_term (const string &src) {
    return src == "+" || src == "-";
}
bool is_fact (const string &src) {
    return src == "*" || src == "/";
}
bool is_func (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool is_number (const string &input) {

    if (input.size() == 0) return false;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '-' && isdigit(input[i+1])) continue;
        if (input[i] != '.' && input[i] != ',' && !isdigit(input[i])) {
            return false;
        }
    }

    return true;
}
bool is_operator (const string &input) {
    return is_fact(input) || is_term(input) || input == "^";
}

vector<string> tokenize (const string &input) {

    vector<string> code;
    size_t i = 0;

    while (i < input.size()) {

        if (isdigit(input[i])) {
            string buffer;

            while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
            code.push_back(buffer);
        } else if (isspace(input[i])) {
            while (isspace(input[i])) i++;
        } else if (input[i] == '+') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '*' || input[i] == '/') {
            code.push_back(string(1,input[i++]));
        } else if (input[i] == '^') {
            code.push_back(string(1,input[i++]));
        } else if (isalpha(input[i])) {
            string buffer;

            while (isalpha(input[i])) buffer += input[i++];
            code.push_back(buffer);
        } else if (input[i] == '-') {
            if (code.size() == 0 || is_operator(code.back())) {
                string buffer = "-";
                i++;

                while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
                code.push_back(buffer);
            } else {
                code.push_back(string(1,input[i++]));
            }
        } else {
            code.push_back(string(1,input[i++]));
        }
    }

    return code;
}

complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}

std::string getsub (std::vector<std::string>::iterator &it, std::vector<std::string>::iterator nd) {
    int pile = 1;
    std::string sub;

    for (it = it + 1; pile != 0 && it != nd ; it++) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it ;
    }

    //if (sub.size() && sub.back() == ' ') sub.pop_back();
    return sub;
}

template<class T> T getstack (vector<T> &stack) {
    if (stack.empty()) throw::exception();
    const T val = stack.back();
    stack.pop_back();
    return val;
}

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
bool precedence (const vector<string> &stack, const string &cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()) > order(cell);
    return order(stack.back()) >= order(cell);
}
bool compare (string a, double b) {
    if (!is_number(a)) return false;
    return stod(a) == b;
}

string mktok (string src) {
    if (is_number(src) || src == "x") return src;
    return "(" + src + ")";
}

string add (string a, string b) {

    if (a == "inf" || b == "inf") return "inf";
    if (is_number(a) && is_number(b)) return to_string(stod(a) + stod(b));
    if (compare(a, 0)) return b;
    if (compare(b, 0)) return a;
    if (a == b) return ( "2 * " + b);

    return mktok(a) + "+" + mktok(b);
}
string sub (string a, string b) {

    if (a == b) return "0";
    if (a == "inf") return "inf";
    if (b == "inf") return "-inf";

    if (compare(b, 0)) return a;

    if (is_number(a) && is_number(b)) return to_string(stod(a) - stod(b));
    return mktok(a) + "-" + mktok(b);
}
string mul (string a, string b) {

    if (a == "inf" || b == "inf") return "inf";
    if (compare(a, 1)) return b;
    if (compare(b, 1)) return a;
    if (compare(a, 0) || compare(b, 0)) return "0";
    if (is_number(a) && is_number(b)) return to_string(stod(a) * stod(b));
    //if (a == b) return (a + "^2");

    return mktok(a) + "*" + mktok(b);
}
string div (string a, string b) {

    if (a == b) return "1";
    if (a == "inf") return "inf";
    if (b == "inf") return "0";
    if (compare(b, 1)) return a;

    if (is_number(a) && is_number(b)) {
        return to_string(stod(a) / stod(b));
    }

    return mktok(a) + "/" + mktok(b);
}
string exp (string a, string b) {

    if (compare(b, 1)) return a;
    if (compare(a, 1) || compare(b, 0)) return "1";

    if (is_number(a) && is_number(b)) {
        return to_string(pow(stod(a),  stod(b)));
    }

    return mktok(a) + "^" + mktok(b);
}

pair<string,string> diff (vector<pair<string,string>> &vars, vector<string> &oper) {
    auto [f2,d2] = vars.back(); vars.pop_back();
    auto [f1,d1] = vars.back(); vars.pop_back();
    string op = getstack(oper);
    pair<string,string> res;

    if (op == "+") {
        res = {add(f1, f2), add(d1, d2)} ;
    } else if (op == "-") {
        res = {sub(f1, f2), sub(d1, d2)} ;
    } else if (op == "*") {
        res = {mul(f1, f2),  add(mul(f1,d2), mul(d1,f2))} ;
    } else if (op == "/") {
        string num =  sub(mul(d1,f2),mul(f1,d2)) ;
        string den =  exp(f2, "2");
        res = {div(f1,f2), div(num,den)} ;

    } else if (op == "^") {
        string ex = exp(f1,f2), inner;
        inner = add( mul( d1, div(f2,f1) ), mul(d2, "log(" +  f1 + ")"));
        res = {ex, mul(ex,inner)} ;
    }

    return res;
}
complex<double> evaluate (const std::string &input, complex<double> val) {

    auto expr = tokenize(input);
    vector<string>::iterator it = expr.begin(), end = expr.end();
    vector<string> oper;
    vector<complex<double>> vars;

    while (it < end) {
        string cell = *it;

        if (cell == "x") {
            vars.push_back(val);
        } else if (is_number(cell)) {
            vars.push_back(stod(cell));
        } else if (cell == "(") {
            vars.push_back(evaluate(getsub(it,end), val));
        } else if (is_operator(cell)) {

            while (precedence(oper, cell)) {
                complex<double> b = getstack(vars), a = getstack(vars);
                string op = getstack(oper);

                switch (op[0]) {
                    case '+' : vars.push_back(a + b); break;
                    case '-' : vars.push_back(a - b); break;
                    case '*' : vars.push_back(a * b); break;
                    case '/' : vars.push_back(a / b); break;
                    case '^' : vars.push_back(a ^ b); break;
                    default  : break;
                }
            }
            oper.push_back(cell);
        } else if (is_func(cell)) {
            it++;
            complex<double> var = evaluate(getsub(it,end), val);

            if (cell == "cos") {
                vars.push_back(cos(var));
            } else if (cell == "sin") {
                vars.push_back(sin(var));
            } else if (cell == "tan") {
                vars.push_back(tan(var));
            } else if (cell == "log") {
                vars.push_back(log(var));
            } else if (cell == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
                vars.push_back( 1.0 / tan(var));
            }
        }

        it++;
    }

    while (!oper.empty()) {
        complex<double> b = getstack(vars), a = getstack(vars);
        string op = getstack(oper);

        switch (op[0]) {
            case '+' : vars.push_back(a + b); break;
            case '-' : vars.push_back(a - b); break;
            case '*' : vars.push_back(a * b); break;
            case '/' : vars.push_back(a / b); break;
            case '^' : vars.push_back(a ^ b); break;
            default  : break;
        }
    }

    return getstack(vars);
}
string derivate (const string &input) {

    auto expr = tokenize(input);
    vector<string>::iterator it = expr.begin(), end = expr.end();

    vector<string> oper;
    vector<pair<string,string>> vars;

    while (it < end) {
        string cell = *it;

        if (cell == "x") {
            vars.push_back({cell,"1"});
        } else if (is_number(cell)) {
            vars.push_back({cell,"0"});
        } else if (cell == "(") {
            string sub = getsub(it,end), dub = derivate(sub);
            vars.push_back({sub,dub});
        } else if (is_operator(cell)) {
            while (precedence(oper, cell)) {
                vars.push_back(diff(vars,oper));
            }
            oper.push_back(cell);
        } else if (is_func(cell)) {
            it++;
            string var = getsub(it,end);
            string fx = cell + "(" + var + ")", dx = derivate(var);

            if (cell == "log") {
                vars.push_back( { fx , div( dx, var)});
            } else if (cell == "sin") {
                vars.push_back( { fx , mul( dx,"cos(" + var + ")")});
            } else if (cell == "cos") {
                vars.push_back( { fx , mul( sub("0", dx), "sin(" + var + ")") });
            } else if (cell == "tan") {
                vars.push_back( { fx , div( dx, "(cos(" + var + "))^2" ) });
                //vars.push_back( { fx , mul(dx, "(sec(" + var + "))^2") });
            } else if (cell == "cot") {
                vars.push_back( { fx , div( sub("0", dx), "(sin(" + var + "))^2") });
                //vars.push_back( { fx , mul(sub("0", dx), "(csc(" + var + "))^2") });
            }

            else if (cell == "sec") {
                vars.push_back( { fx , mul(dx, mul("(sec(" + var + "))", "(tan(" + var + "))")) });
            } else if (cell == "csc") {
                vars.push_back( { fx , mul(dx, mul("(cot(" + var + "))", "(csc(" + var + "))")) });
            }
        }

        it++;
    }

    while (!oper.empty()) {
        vars.push_back(diff(vars,oper));
    }

    return  getstack(vars).second;
}
tuple<func_t,func_t,func_t> differentiate (const string &expression) {

    //cout << "expression : [" <<  expression << "]\n" << flush;
    string pass0 = expression;
    string pass1 = derivate(pass0);
    string pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return evaluate(pass0, x); },
            [pass1](value_t x) { return evaluate(pass1, x); },
            [pass2](value_t x) { return evaluate(pass2, x); },
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "tests.hpp"
// #include "/home/Nemrod-7/include/Timer.hpp"

complex<double> sec (complex<double> x) { return 1.0 / cos(x); }
complex<double> csc (complex<double> x) { return 1.0 / sin(x); }
complex<double> sqr (const complex<double> &x) {
    return x * x;
}

int main () {

    // Timer clock;
    // clock.start();


    // tests();

    // clock.get_duration();

    cout << "\nexit\n";
}
