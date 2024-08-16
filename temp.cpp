#include <iostream>
#include <string>
#include <vector>

using namespace std;

// rules of derivation :
// con : cst   => 0
// lin : x     => 1
// add : a + b => a' + b'
// min : a - b => a' - b'
// mul : a * b => a.b' + a'.b
// div : a / b => (a'* b − b'* a) / (b * b)
// pow : x^a   => a.x^(a - 1)

bool isnum (const string &src) {
    for (auto &it : src) {
        if (!isdigit(it)) return false;
    }
    return true;
}
bool is_operator(string input) {
    return input == "+" || input == "-" || input == "*" || input == "^" || input == "/";
}
int pow(int base, int exp) {
    int result = 1;
    for (;;) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) break;
        base *= base;
    }

    return result;
}
int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;

}

vector<string> tokenize(const string &input) {
    vector<string> token;

    for (auto &it : input) {
        if (!isspace(it)) {
            token.push_back(string(1,it));
        }
    }

    return token;
}

string getstack(vector<string> &stack) {
    string val = stack.back();
    stack.pop_back();
    return val;
}
string join(vector<string> input) {
    string os;
    for (auto &it : input) os += it;
    return os;
}

string operate(string a, string op, string b) {

    // cout << "[" << a << "] " << op << " [" << b << "]\n";
    if (isnum(a) && isnum(b)) {
        switch (op[0]) {
            case '+' : return to_string( stoi(a) + stoi(b)) ; break;
            case '-' : return to_string( stoi(a) - stoi(b)) ; break;
            case '*' : return to_string( stoi(a) * stoi(b)) ; break;
            case '/' : return to_string( stoi(a) / stoi(b)) ; break;
            case '^' : return to_string((int) pow(stoi(a), stoi(b))) ; break;
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
string differentiate(const string &input) {

    if (input == "x") return "1"; // constant
    if (isnum(input)) return "0"; // variable

    vector<string> code = tokenize(input);

    for (int i = 0; i < code.size(); i++) {
        if (is_operator(code[i])) {
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
        }
    }

    return "";
}
string evaluate (const string &expression, const string &number = "") {

    vector<string> code = tokenize(expression);

    int index = 0; 
    vector<string> vars;
    vector<string> oper;

    while (index < code.size()) {

        if (code[index] == "x") {
            string value;
            if (number == "") {

            }
            vars.push_back(code[index]);
        } else if (isnum(code[index])) {
            vars.push_back(code[index]);
        } else if (is_operator(code[index])) {

            while (!oper.empty() && order(oper.back()) >= order(code[index])) {
                string v2 = getstack(vars), v1 = getstack(vars), op = getstack(oper);
                vars.push_back(operate(v1,op,v2));
            }
            oper.push_back(code[index]);
        }

        index++;
    }

    while (!oper.empty()) {
        string v2 = getstack(vars), v1 = getstack(vars), op = getstack(oper);
        vars.push_back(operate(v1,op,v2));
    }

    return vars.back();
}

int main () {

    string expression = "2*x^3";

    string pass1 = differentiate(expression);
    string result = evaluate(pass1);
    // cout << "result : " << actual << "\n";

    // cout << simpl("x", "^", "3");

}
