#include <iostream>

#include <vector>
#include <cmath>
#include <tuple>
#include <complex>
#include <functional>

using namespace std;
using value_t = std::complex<double>;
using func_t = std::function<value_t(value_t)>;

struct Token {
    complex<double> num;
    string sym;

    Token (string s) : num(0), sym(s) {}
    Token (double n) : num(n), sym("") {}
    Token (complex<double> n) : num(n), sym("") {}
};

const vector<Token> zero = {Token(0.0)}, one {Token(1.0)}, two = {Token(2.0)};

bool operator == (const vector<Token> &a, const vector<Token> &b) {

    if (a.size() != b.size()) return false;

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].num != b[i].num || a[i].sym != b[i].sym) return false;
    }

    return true;
}

string showarm (const vector<Token> &x) {
    stringstream os;
    os << "[";
    for (auto &tok : x) {
        if (tok.sym == "") {
            os << tok.num;
        } else {
            os << tok.sym;
        }
    }
    os << "]";

    return os.str();
}
complex<double> operator ^ (const complex<double> &a, const complex<double> &b) {
    return pow(a,b);
}
template<class T> T getstack(vector<T> &stack) {
    T top = stack.back();
    stack.pop_back();
    return top;
}

bool isnum (const vector<Token> &term) {
    return term.size() == 1 && term[0].sym == "";
}

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

vector<Token> tokenize (const string &src) {

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

vector<Token> calc (const vector<Token> &a, const string &op, const vector<Token> &b) {

    vector<Token> arm;

    if (a.size() > 1) arm.push_back(Token("("));
    for (auto &tok : a) arm.push_back(tok);
    if (a.size() > 1) arm.push_back(Token(")"));

    arm.push_back(Token(op));

    if (b.size() > 1) arm.push_back(Token("("));
    for (auto &tok : b) arm.push_back(tok);
    if (b.size() > 1) arm.push_back(Token(")"));

    return arm;
}
vector<Token> join (const vector<Token> &a, const vector<Token> &b) {
    vector<Token> arm = a;

    for (auto &tok : b) arm.push_back(tok);

    return arm;


}
vector<Token> mkfunc (string fn, vector<Token> x) {

    vector<Token> arm = {Token(fn), Token("(")};

    for (auto &it : x) arm.push_back(it);

    arm.push_back(Token(")"));
    return arm;
}

vector<Token> add (const vector<Token> &a, const vector<Token> &b) {

    if (a == zero) return b;
    if (b == zero) return a;
    if (isnum(a) && isnum(b)) return { Token(a[0].num + b[0].num) };


    return calc(a,"+",b);
}
vector<Token> sub (const vector<Token> &a, const vector<Token> &b) {

    if (isnum(a) && isnum(b)) return { Token(a[0].num - b[0].num) };


    return calc(a,"-",b);
}
vector<Token> mul (const vector<Token> &a, const vector<Token> &b) {

    if (a == zero || b == zero) return zero;
    if (a == one) return b;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { Token(a[0].num * b[0].num) };

    return calc(a,"*",b);
}
vector<Token> div (const vector<Token> &a, const vector<Token> &b) {

    if (isnum(a) && isnum(b)) return { Token(a[0].num * b[0].num) };


    return calc(a,"/",b);
}
vector<Token> exp (const vector<Token> &a, const vector<Token> &b) {

    if (a == one || b == zero) return one;
    if (b == one) return a;
    if (isnum(a) && isnum(b)) return { Token(a[0].num ^ b[0].num) };

    return calc(a,"^",b);
}

pair<vector<Token>,vector<Token>> diff (vector<pair<vector<Token>,vector<Token>>> &vars, vector<string> &oper) {

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
        vector<Token> num =  sub(mul(d1,f2),mul(f1,d2)) ;
        vector<Token> den =  exp(f2, two);

        res = {div(f1,f2), div(num,den)} ;
    } else if (op == "^") {
        vector<Token> log = mkfunc("log", f1);//join(join(tokenize("log("), f1), tokenize(")"));
        vector<Token> inner = add( mul( d1, div(f2,f1) ), mul(d2, log));

        res = {exp(f1,f2), mul(exp(f1,f2), inner)} ;
    } else {
        throw exception();
    }

    //        cout << op << '\n';
    //        cout << "f1: " << showarm(f1) << " d1 : " << showarm(d1) << '\n';
    //        cout << "f2: " << showarm(f2) << " d2 : " << showarm(d2) << '\n';
    //        cout << " = " << showarm( mul(f1, f2));
    //        cout << "\n";

    return res;
}
vector<Token> derivate (vector<Token> code) {

    vector<Token>::iterator it = code.begin(), end = code.end();
    vector<string> oper;
    vector<pair<vector<Token>,vector<Token>>> vars;

    //showarm(code);
    //cout << "\n";

    while (it < end) {
        Token curr = *it;

        if (curr.sym == "x") {
            vars.push_back({{curr}, {Token(1.0)}});
        } else if (curr.sym == "") {
            vars.push_back({{curr}, {Token(0.0)}});
        } else if (curr.sym == "(") {
            vector<Token> sub = getsub(it,end);
            vars.push_back({ sub, derivate(sub) });
        } else if (is_operator(curr.sym)) {
            while (precedence(oper, curr.sym)) {
                vars.push_back(diff(vars,oper));
            }

            oper.push_back(curr.sym);
        } else if (is_func(curr.sym)) {
            it++;
            vector<Token> var = getsub(it,end);
            vector<Token> fx = mkfunc(curr.sym, var), dx = derivate(var);

            if (curr.sym == "log") {
                vars.push_back( { fx , div( dx, var)});
            } else if (curr.sym == "sin") {
                vars.push_back( { fx , mul( dx, mkfunc("cos", var))});
            } else if (curr.sym == "cos") {
                vars.push_back( { fx , mul( sub(zero, dx), mkfunc("sin", var)) });
            } else if (curr.sym == "tan") {
                vars.push_back( { fx , div( dx, exp(mkfunc("cos", var), two)) });
                //vars.push_back( { fx , mul(dx, exp(mkfunc("sec", var), two)) });
            } else if (curr.sym == "cot") {
                vars.push_back( { fx , div( sub(zero, dx), exp(mkfunc("sin", var), two) ) });
                //vars.push_back( { fx , div( sub(zero, dx),exp(mkfunc("csc", var), two) ) });
            }

            else if (curr.sym == "sec") {
                vars.push_back( { fx , mul(dx, mul( mkfunc("sec", var), mkfunc("tan", var))) });
            } else if (curr.sym == "csc") {
                vars.push_back( { fx , mul(dx, mul( mkfunc("cot", var), mkfunc("csc", var))) });
            }
        }

        it++;
    }

    while (!oper.empty()) {
        vars.push_back(diff(vars,oper));
    }

    return getstack(vars).second;
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
        } else if (is_operator(cell.sym)) {

            while (precedence(oper, cell.sym)) {
                complex<double> b = getstack(vars), a = getstack(vars);
                string op = getstack(oper);

                //cout << "[" << a << "]" << op << "[" << b << "]" ;
                switch (op[0]) {
                    case '+' : vars.push_back(a + b); break;
                    case '-' : vars.push_back(a - b); break;
                    case '*' : vars.push_back(a * b); break;
                    case '/' : vars.push_back(a / b); break;
                    case '^' : vars.push_back(a ^ b); break;
                    default  : break;
                }
                //cout << " => " << vars.back() << "\n";
            }
            oper.push_back(cell.sym);
        } else if (is_func(cell.sym)) {
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
            //cout << vars.back() << "  ";
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
        // cout << "[" << a << "]" << op << "[" << b << "]" ;
        // cout << " => " << vars.back() << "\n";
    }

    return getstack(vars);
}
tuple<func_t,func_t,func_t> differentiate (const string &expression) {

    cout << "expression : [" <<  expression << "]\n" << flush;
    vector<Token> pass0 = tokenize(expression);
    vector<Token> pass1 = derivate(pass0);
    vector<Token> pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return evaluate(pass0, x); },
            [pass1](value_t x) { return evaluate(pass1, x); },
            [pass2](value_t x) { return evaluate(pass2, x); },
    };
}

#include "tests.hpp"

complex<double> sec (complex<double> x) { return 1.0 / cos(x); }
complex<double> csc (complex<double> x) { return 1.0 / sin(x); }

int main () {

    // double x = 5.0;
    // double dx = 3;
    // cout << -dx / pow(sin(x), 2) << "\n";
    // cout << -dx * pow(csc(x), 2) << "\n";
    // cout << fixed << setprecision(15) << pow(pow(0.7, 32.2), 2);

    complex<double> x = {0,1};
    string expression = "3.14 * (1 + 5 * x) / log(x)";

    //expression = "x ^ 3";
    auto pass0 = tokenize(expression);
    auto pass1 = derivate(pass0);
    //auto pass2 = derivate(pass1); // 0.12722 0.402059

    //cout << showarm(pass1);

    //cout << " = " << evaluate(pass2, x) << "\n\n"; 
    //

    /*
    */


    tests();
    //showarm(pass1);

    cout << "\nexit\n";
}
