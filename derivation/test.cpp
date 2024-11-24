
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

double sec (double x) { return 1.0 / cos(x); }
double csc (double x) { return 1.0 / sin(x); }

struct Token {
    complex<double> num;
    string sym;

    Token (string s) : num(0), sym(s) {}
    Token (double n) : num(n), sym("") {}
    Token (complex<double> n) : num(n), sym("") {}

    bool operator == (const Token &x) {
        return x.num == num && x.sym == sym;
    }
};

complex<double> operator ^ (const complex<double> &a, const complex<double> &b) {
    return pow(a,b);
}
template<class T> T getstack(vector<T> &stack) {
    T top = stack.back();
    stack.pop_back();
    return top;
}

bool isnum (vector<Token> term) {
    return term.size() == 1 && term[0].sym == "";
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
bool is_operator (const string &input) { 
    return is_term(input) || is_fact(input) || input == "^";
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

    vector<Token> code;
    size_t i = 0;

    while (i < input.size()) {
        if (isdigit(input[i])) {
            string buffer;

            while (isdigit(input[i]) || input[i] == '.') buffer += input[i++];
            code.push_back(stod(buffer));
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
            if (code.size() == 0 || is_operator(code.back().sym)) {
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
vector<Token> join (const vector<Token> &a, const string &op, const vector<Token> &b) {

    vector<Token> arm;

    //arm.push_back(Token("("));
    for (auto &tok : a) arm.push_back(tok);
    //arm.push_back(Token(")"));

    arm.push_back(Token(op));

    //arm.push_back(Token("("));
    for (auto &tok : b) arm.push_back(tok);
    //arm.push_back(Token(")"));

    return arm;
}

const vector<Token> zero = {Token("")};

bool operator == (const vector<Token> &a, const vector<Token> &b) {

    if (a.size() != b.size()) return false;

    for (int i = 0; i < a.size(); i++) {
        if (a[i].num != b[i].num || a[i].sym != b[i].sym) return false;
    }

    return true;
}

vector<Token> add (const vector<Token> &a, const vector<Token> &b) {

    const vector<Token> zero = {Token(0.0)};

    if (isnum(a) && isnum(b)) return { Token(a[0].num + b[0].num) };
    if (isnum(a) && a[0].num == 0.0) return b;
    if (isnum(b) && b[0].num == 0.0) return a;
    //if (isnum(b) && b == zero) return a;

    

    return join(a,"+",b);
}
vector<Token> sub (const vector<Token> &a, const vector<Token> &b) {

    if (isnum(a) && isnum(b)) return { Token(a[0].num - b[0].num) };

    return join(a,"-",b);
}
vector<Token> mul (const vector<Token> &a, const vector<Token> &b) {

    if (isnum(a) && isnum(b)) return { Token(a[0].num * b[0].num) };
    if (a == zero) return zero;

    return join(a,"*",b);
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
        //string num =  sub(mul(d1,f2),mul(f1,d2)) ;
        //string den =  exp(f2, "2");
        ////cout << "\n\nden : " << "[" << den << "]" << "\n";
        //res = {div(f1,f2), div(num,den)} ;

        //cout << op << "\n";
        //cout << "fx1 : " << f1 << " dx1 : " << d1 << "\n";
        //cout << "fx2 : " << f2 << " dx2 : " << d2 << "\n";
        //cout << "\n";
        //cout << fixed << "[" << num << "]" << "/" << "[" << den << "]"  << flush;

        //cout << "[" << res.first << "][" << res.second << "]"  << flush;

    } else if (op == "^") {
        //string ex = exp(f1,f2), inner;
        //inner = add( mul( d1, div(f2,f1) ), mul(d2, "log(" +  f1 + ")"));
        //res = {ex, mul(ex,inner)} ;
    } else {
        throw exception();
    }

    return res;
}
vector<Token> derivate (vector<Token> code) {

    vector<Token>::iterator it = code.begin(), end = code.end();

    vector<string> oper;
    vector<pair<vector<Token>,vector<Token>>> vars;

    while (it < end) {
        Token curr = *it;

        if (curr.sym == "(") {

        } else if (is_operator(curr.sym)) {
            while (precedence(oper, curr.sym)) {
                vars.push_back(diff(vars,oper));
            }

            oper.push_back(curr.sym);
        } else if (is_func(curr.sym)) {
            it++;

        } else if (curr.sym == "x") {
            vars.push_back({{curr}, {Token(1.0)}});
        } else if (curr.sym == "") {
            vars.push_back({{curr}, {Token(0.0)}});
        }

        it++;
    }

    while (!oper.empty()) {
        vars.push_back(diff(vars,oper));
    }

    return getstack(vars).second;
}

int main () {

    //    double x = 5.0;
    //    double dx = 3;
    //    cout << -dx / pow(sin(x), 2) << "\n";
    //    cout << -dx * pow(csc(x), 2) << "\n";
    //cout << fixed << setprecision(15) << pow(pow(0.7, 32.2), 2);

    string expression = "2 * x^3";

    expression = "x * 2";
    auto pass0 = tokenize(expression);
    auto pass1 = derivate(pass0);


    for (auto &tok : pass1) {
        //cout << "[";
        if (tok.sym == "") {
            cout << tok.num;
        } else {
            cout << tok.sym;
        }
        //cout << "]";
    }


}
