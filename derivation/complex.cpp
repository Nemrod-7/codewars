#include <iostream>

#include <vector>
#include <cmath>
#include <regex>
#include <tuple>
#include <complex>
#include <functional>

#include "tests.hpp"
#include "/home/Nemrod-7/include/Timer.hpp"

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

const std::regex trigon ("^sin|cos|tan|cot|log|asin|acos|atan|ln|log$");
const std::regex operat ("^[-+*%/^]$");
const std::regex number ("^-?\\d+(\\.\\d+)?$|^\\(-?\\d+(\\.\\d+)?,-?\\d+(\\.\\d+)?\\)$");

complex<double> cot (const complex<double> &x) { return 1.0 / tan(x); }
complex<double> sec (const complex<double> &x) { return 1.0 / cos(x); }
complex<double> csc (const complex<double> &x) { return 1.0 / sin(x); }
complex<double> sqr (const complex<double> &x) { return x * x; }

struct Token {
    complex<double> num;
    string sym;

    Token (string s) : num(0), sym(s) {}
    Token (double n) : num(n), sym("") {}
    Token (complex<double> n) : num(n), sym("") {}
};

string showtoken (const Token &tok) {
    stringstream os;

    if (tok.sym == "") {
        if (tok.num.imag() == 0.0) {
            os << tok.num.real();
        } else {
            os << "(" << tok.num.real() << "+" << tok.num.imag() << "i)";
        }
    } else {
        os << tok.sym;
    }

    return os.str();
}
string showvect (const vector<Token> &x) {
    stringstream os;
    for (auto &tok : x) {
        os << "[";
        os << showtoken(tok);
        os << "]";
    }

    return os.str();
}

const vector<Token> zero = {Token(0.0)}, one {Token(1.0)}, two = {Token(2.0)};

bool operator == (const vector<Token> &a, const vector<Token> &b) {

    if (a.size() != b.size()) return false;

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].num != b[i].num || a[i].sym != b[i].sym) return false;
    }

    return true;
}
complex<double> operator ^ (const complex<double> &a, const complex<double> &b) {
    return pow(a,b);
}
vector<Token> brace (const vector<Token> &a) {
    vector<Token> arm = {Token("(")};

    for (auto &tok : a) arm.push_back(tok);
    arm.push_back(Token(")"));

    return arm;
}
const vector<Token> operator + (const string &a, const vector<Token> &b) {

    vector<Token> arm = {Token(a)};

    for (auto &it : b) arm.push_back(it);

    return arm;
}
const vector<Token> operator + (const vector<Token> &a, const vector<Token> &b) {

    vector<Token> arm = a;

    for (auto &it : b) arm.push_back(it);

    return arm;
}
const vector<Token> operator + (const vector<Token> &a, const string &b) {

    vector<Token> arm;

    for (auto &it : a) arm.push_back(it);
    arm.push_back(Token(b));

    return arm;
}

template<class T> T getstack(vector<T> &stack) {
    T top = stack.back();
    stack.pop_back();
    return top;
}

bool isnum (const vector<Token> &term) {
    return term.size() == 1 && term[0].sym == "";
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

vector<Token> tokenize (const string &input) {

    const regex tokn ("([0-9]+(\\.[0-9]+)?)|x|[-+*/^()]|(sin|cos|tan|cot|log)");
    sregex_token_iterator iter (input.begin (), input.end (), tokn);
    vector<string> temp (iter, sregex_token_iterator ());
    vector<Token> code;

    for (size_t i = 0; i < temp.size(); i++) {
        string curr = temp[i];

        if (temp[i] == "-" && (i == 0 || regex_match(temp[i-1], operat))) {
            curr = "-" + temp[i + 1];
            i += 2;
        }

        if (regex_match(curr, number)) {
            code.push_back(stod(curr));
        } else {
            code.push_back(curr);
        }
    }

    return code;
}
vector<Token> getsub (vector<Token>::iterator &it, vector<Token>::iterator nd) {
    int pile = 1;
    vector<Token> sub;

    for (it = it + 1; pile != 0 && it != nd ; it++) {
        Token curr = *it;
        pile += (curr.sym == "(") - (curr.sym == ")");
        if (pile == 0) break;
        sub.push_back(curr);
    }

    return sub;
}

vector<Token> add (const vector<Token> &a, const vector<Token> &b) {

    if (a == zero) return b;
    if (b == zero) return a;
    if (isnum(a) && isnum(b)) return { Token(a[0].num + b[0].num) };

    return brace(a) + "+" + brace(b);
}
vector<Token> sub (const vector<Token> &a, const vector<Token> &b) {

    if (a == b) return zero;
    if (isnum(a) && isnum(b)) return { Token(a[0].num - b[0].num) };

    return brace(a) + "-" + brace(b);
}
vector<Token> mul (const vector<Token> &a, const vector<Token> &b) {

    if (a == zero || b == zero) return zero;
    if (a == one) return b;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { Token(a[0].num * b[0].num) };

    return brace(a) + "*" + brace(b);
}
vector<Token> div (const vector<Token> &a, const vector<Token> &b) {

    if (a == zero) return zero;
    if (b == one) return a;
    if (a == b) return one;
    if (isnum(a) && isnum(b)) return { Token(a[0].num / b[0].num) };

    return brace(a) + "/" + brace(b);
}
vector<Token> exp (const vector<Token> &a, const vector<Token> &b) {

    if (a == one || b == zero) return one;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { Token(a[0].num ^ b[0].num) };

    return brace(a) + "^" + brace(b);
}

pair<vector<Token>,vector<Token>> operate (vector<pair<vector<Token>,vector<Token>>> &vars, vector<string> &oper) {

    auto [f2, d2] = getstack(vars);
    auto [f1, d1] = getstack(vars);
    string op = getstack(oper);
    pair<vector<Token>,vector<Token>> res;

    if (op == "+") {
        res = {add(f1, f2), add(d1, d2)} ;
    } else if (op == "-") {
        res = {sub(f1, f2), sub(d1, d2)} ;
    } else if (op == "*") {
        res = {mul(f1, f2), add(mul(f1,d2), mul(d1,f2))} ;
    } else if (op == "/") {
        vector<Token> num =  sub(mul(d1,f2),mul(f1,d2)), den =  exp(f2, two);

        res = {div(f1,f2), div(num,den)} ;
    } else if (op == "^") {
        vector<Token> log = "log" + brace(f1);
        vector<Token> inner = add( mul( d1, div(f2,f1) ), mul(d2, log));

        res = {exp(f1,f2), mul(exp(f1,f2), inner)} ;
    } else {
        throw exception();
    }

    return res;
}
pair<vector<Token>,vector<Token>> evaluate (vector<Token> expression, const complex<double> &val = {0,0}) {
    // expression -> { fonction, derivative }
    vector<Token>::iterator it = expression.begin(), end = expression.end();
    vector<string> oper;
    vector<pair<vector<Token>,vector<Token>>> vars;
    vector<Token> fx;

    while (it < end) {
        Token curr = *it;

        if (curr.sym == "x") {
            fx = val == value_t{0,0} ? vector<Token> {curr} : vector<Token> {Token(val)};

            vars.push_back({fx, {Token(1.0)}});
        } else if (curr.sym == "") {
            vars.push_back({{curr}, {Token(0.0)}});
        } else if (curr.sym == "(") {
            vars.push_back(evaluate(getsub(it,end), val));
        } else if (regex_match(curr.sym, operat)) {
            while (precedence(oper, curr.sym)) {
                vars.push_back(operate(vars,oper));
            }
            oper.push_back(curr.sym);
        } else if (regex_match(curr.sym, trigon)) {
            it++;
            auto [var, dx] = evaluate(getsub(it,end), val);
            vector<Token> nxt = brace(var);

            if (curr.sym == "log") {
                fx = (isnum(var)) ? vector<Token> {Token(log(var[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , div( dx, nxt)});
            } else if (curr.sym == "sin") {
                fx = (isnum(var)) ? vector<Token> {Token(sin(var[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , mul( dx, "cos" + nxt)});
            } else if (curr.sym == "cos") {
                fx = (isnum(var)) ? vector<Token> {Token(cos(var[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , mul( sub(zero, dx), "sin" + nxt) });
            } else if (curr.sym == "tan") {
                fx = (isnum(var)) ? vector<Token> {Token(tan(var[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , div( dx, exp("cos" + nxt, two)) });
                //vars.push_back( { fx , mul(dx, exp(mkfunc("sec", nxt), two)) });
            } else if (curr.sym == "cot") {
                fx = (isnum(var)) ? vector<Token> {Token(cot(var[0].num))} : curr.sym + nxt;
                //fx = (isnum(nxt)) ? vector<Token> {Token(1.0 / tan(nxt[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , div( sub(zero, dx), exp("sin" + nxt, two) ) });
                //vars.push_back( { fx , div( sub(zero, dx),exp(mkfunc("csc", nxt), two) ) });
            } 

            else if (curr.sym == "sec") {
                fx = (isnum(var)) ? vector<Token> {Token(sec(nxt[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , mul(dx, mul("sec" + nxt, "tan" + nxt)) });
            } else if (curr.sym == "csc") {
                fx = (isnum(var)) ? vector<Token> {Token(sec(nxt[0].num))} : curr.sym + nxt;
                vars.push_back( { fx , mul(dx, mul("cot" + nxt, "csc" + nxt)) });
            }
        }

        it++;
    }

    while (!oper.empty()) {
        vars.push_back(operate(vars,oper));
    }

    return getstack(vars);
}

tuple<func_t,func_t,func_t> differentiate (const string &expression) {

    //cout << "expression : [" <<  expression << "]\n" << flush;
    vector<Token> pass0 = tokenize(expression);
    vector<Token> pass1 = evaluate(pass0).second;
    vector<Token> pass2 = evaluate(pass1).second;

    return {
        [pass0](value_t x) { return evaluate(pass0, x).first[0].num; },
            [pass1](value_t x) { return evaluate(pass1, x).first[0].num; },
            [pass2](value_t x) { return evaluate(pass2, x).first[0].num; },
    };
}

int main () {

    Timer clock;
    clock.start();

    complex<double> x = {1,1};
    string expression = "sin(cos(x^x^2))";

    auto code = tokenize(expression);

    auto [fx,dx] = evaluate(code, x);

    //cout << fx[0].num;
    //cout << showvect(fx);

    tests();

    clock.stop();
    clock.get_duration();
}

/*
   bool is_term (const string &src) { 
   return src == "+" || src == "-"; 
   }
   bool is_fact (const string &src) { 
   return src == "*" || src == "/"; 
   }
   bool is_func (const string &src) {
   return src == "sin" || src == "cos" || src == "tan" || src == "log" || src == "cot";
   }
   bool is_operator (const string &src) { 
   return is_term(src) || is_fact(src) || src == "^";
   }
   vector<Token> tokenize1 (const string &src) {

   vector<Token> code;
   size_t i = 0;

   while (i < src.size()) {
   if (isdigit(src[i])) {
   string buffer;

   while (isdigit(src[i]) || src[i] == '.') buffer += src[i++];
   code.push_back(stod(buffer));
   } else if (isspace(src[i])) {
   while (isspace(src[i])) i++;
   } else if (src[i] == '+') {
   code.push_back(string(1,src[i++]));
   } else if (src[i] == '*' || src[i] == '/') {
   code.push_back(string(1,src[i++]));
   } else if (src[i] == '^') {
   code.push_back(string(1,src[i++]));
   } else if (isalpha(src[i])) {
   string buffer;

   while (isalpha(src[i])) buffer += src[i++];
   code.push_back(buffer);
   } else if (src[i] == '-') {
   if (code.size() == 0 || is_operator(code.back().sym)) {
   string buffer = "-";
   i++;

   while (isdigit(src[i]) || src[i] == '.') buffer += src[i++];
   code.push_back(buffer);
   } else {
   code.push_back(string(1,src[i++]));
   }
   } else {
   code.push_back(string(1,src[i++]));
   }
   }

   return code;
   }

complex<double> evaluate (vector<Token> expr, const complex<double> &val) {

    vector<Token>::iterator it = expr.begin(), end = expr.end(); 
    vector<string> oper;
    vector<complex<double>> vars;

    while (it < end) {
        Token cell = *it;

        if (cell.sym == "x") {
            vars.push_back(val);
        } else if (cell.sym == "") {
            vars.push_back(cell.num);
        } else if (cell.sym == "(") {
            vars.push_back(evaluate(getsub(it,end), val));
        } else if (regex_match(cell.sym, operat)) {
            while (precedence(oper, cell.sym)) {
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
            oper.push_back(cell.sym);
        } else if (regex_match(cell.sym,trigon)) {
            it++;
            complex<double> var = evaluate(getsub(it,end), val);

            if (cell.sym == "cos") {
                vars.push_back(cos(var));
            } else if (cell.sym == "sin") {
                vars.push_back(sin(var));
            } else if (cell.sym == "tan") {
                vars.push_back(tan(var));
            } else if (cell.sym == "log") {
                vars.push_back(log(var));
            } else if (cell.sym == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
                vars.push_back( 1.0 / tan(var));
            } 

            else if (cell.sym == "csc") {
                vars.push_back(csc(var));
            } else if (cell.sym == "sec") {
                vars.push_back(sec(var));
            } else if (cell.sym == "sqr") {
                vars.push_back(sqr(var));
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
   */

