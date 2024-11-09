#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

// without regex

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
using value_t = complex<double>;

struct Token {
    std::string sym;
    std::complex<double> val;

    Token (const std::string &label) : sym (label), val(0.0,0.0) {}
    Token (const std::complex<double> &value) : sym (""), val(value) {}
};
bool is_term (const string &sym) { return sym == "+" || sym == "-"; }
bool is_fact (const string &sym) { return sym == "*" || sym == "/"; }
bool is_func (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}
bool is_number (const string &input) {

    if (input.size() == 0) return false;
    int i = 0, end = input.size();

    if (input.front() == '(' && input.back() == ')') {
        i += 1, end -= 1;
    }

    for (; i < end; i++) {
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
    int i = 0;

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

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
std::string getsub (std::vector<std::string>::iterator &it, std::vector<std::string>::iterator nd) {
    int pile = 1;
    std::string sub;

    for (it = it + 1; pile != 0 && it != nd ; it++) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it + " ";
    }

    return sub;
}

template<class T> T getstack (vector<T> &stack) {
    if (stack.empty()) throw::exception();
    const T val = stack.back();
    stack.pop_back();
    return val;
}
bool precedence (const vector<string> &stack, const string &cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()) > order(cell);
    return order(stack.back()) >= order(cell);
}

string join (vector<string>::iterator &fs, vector<string>::iterator &nd) {

    string os;

    for (auto it = fs; it != nd; it++) {
        os += *it + " ";
    }

    if (os.size() > 0) os.pop_back();
    return os;
}

bool compare(string a, double b) {
    if (!is_number(a)) return false;
    return stod(a) == b;
}

string add (string a, string b) {

    if (is_number(a) && is_number(b)) return to_string(stod(a) + stod(b));
    if (compare(a, 0)) return b;
    if (compare(b, 0)) return a;
    if (a == b) return ( "2 * " + b);
    
    return (a + "+" + b);
}
string sub (string a, string b) { 

    if (a == b) return "0";
    if (compare(b, 0)) return a;

    if (is_number(a) && is_number(b)) return to_string(stod(a) - stod(b));
    return a + "-" + b;
}
string mul (string a, string b) {

    if (compare(a, 1)) return b;
    if (compare(b, 1)) return a;
    if (compare(a, 0) || compare(b, 0)) return "0";
    if (is_number(a) && is_number(b)) return to_string(stod(a) * stod(b));
    if (a == b) return ("2 ^ " + b);

    return (a + "*" + b);
}
string div (string a, string b) {

    if (a == b) return "1";
    if (compare(b, 1)) return a;
    if (is_number(a) && is_number(b)) return to_string(stod(a) / stod(b));
    return (a + "/" + b);
}
string exp (string a, string b) {

    if (compare(b, 1)) return a;
    if (compare(a, 1) || compare(b, 0)) return "1";
    return (a + "^" + b);
}

complex<double> operator ^ (const complex<double> &a, const complex<double> &b) {
    return pow(a,b);
}
complex<double> evaluate (std::string input, complex<double> val) {

    auto expr = tokenize(input);
    vector<string>::iterator it = expr.begin(), end = expr.end(), start = expr.begin();
    vector<string> oper;
    vector<complex<double>> vars;

    while (it < end) {
        string cell = *it;

        if (cell == "x") {
            vars.push_back(val);
        } else if (is_number(cell)) {
            vars.push_back(stod(cell));
        } else if (cell == "(") {
            vars.push_back(evaluate(getsub(it,end),val));
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
            complex<double> var = evaluate(getsub(it,end), val);

            if (cell == "cos") {
                vars.push_back(cos(var));
            } else if (cell == "sin") {
                vars.push_back(sin(var));
            } else if (cell == "tan") {
                vars.push_back(tan(var));
            } else if (cell == "log") {
                vars.push_back(log(var));
            } else if (cell == "cot") {

            }

        } else {

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
string derivate (string input) {

    if (input == "x") return "1";
    if (is_number(input)) return "0";

    auto expr = tokenize(input);
    vector<string>::iterator it = expr.begin(), end = expr.end(), start = expr.begin();

    vector<string> tx, dx, oper;
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
        }
        else if (is_operator(cell)) {
            //while (precedence(oper, cell)) {
            //
            //}

            oper.push_back(cell);
        } else if (is_func(cell)) {
            string val = getsub(it,end);
            string dx = derivate(val);

            if (cell == "sin") {

            } else if (cell == "cos") {

            } else if (cell == "tan") {

            } else if (cell == "log") {

            } else if (cell == "cot") {

            }
        }

        it++;
    }


    while (!oper.empty()) {
        auto [t2,d2] = vars.back(); vars.pop_back();
        auto [t1,d1] = vars.back(); vars.pop_back();
        string op = getstack(oper);
        //cout << "[" << t1 << "]" << oper << "[" << t2 << "]" << "\n";
        if (op == "+") {
            vars.push_back({add(t1, t2), add(d1, d2)}) ;    
        } else if (op == "-") {
            vars.push_back({sub(t1, t2), sub(d1, d2)}) ;    
        } else if (op == "*") {
            vars.push_back({mul(t1, t2),  add(mul(t1,d2), mul(d1,t2))}) ;    
        } else if (op == "/") {
            string num = sub(mul(d1,t2),mul(t1,d2));
            string den = exp(t2, "2");

            vars.push_back({div(t1, t2), div(num, den)}) ;    
        } else if (op == "^") {
            string outer = exp(t1, t2);
            string inner = "(" + add( mul( d1, div(t2,t1) ), mul(d2, "log (" +  t1 + ")")) + ")";

            vars.push_back({exp(t1, t2), mul(inner,outer)}) ;    
        }
    }

    //cout << "size : " << dx.size() << "\n";
    return vars.back().second;
}

int main () {

    string pass0 = "2 * x^3"; // -32,32
    string pass1 = derivate(pass0); // 0,48
    string pass2 = derivate(pass1); // 24, 24

    complex<double> val = {2,2};

    cout << pass0 << " => " << evaluate(pass0, val) <<  "\n";
    cout << pass1 << " => " << evaluate(pass1, val) <<  "\n";
    cout << pass2 << " => " << evaluate(pass2, val) <<  "\n";
    //cout << pass1 << "\n";



    std::cout << "\nexit\n";
    return 0;
}
