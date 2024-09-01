#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <complex>
#include <functional>

using namespace std;
using value_t = complex<double>;
using func_t = function<value_t(value_t)>;

// Let f be a function.
// The derivative function, denoted by f′, is the function whose domain consists of those values of x
// such that the following limit exists:
// f′(x) = lim h→0 of (f(x + h) − f(x)) / h.
//
// ex : pow derivation : x^y => x^y . (y/x + y'.log(x))


struct node {
    string sym;
    node *t1, *t2;

    node (const string &label = "", node *t1 = nullptr, node *t2 = nullptr) : sym (label), t1 (t1), t2 (t2) {}
};

void showtree(const node *node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├─" : "└─" );
        cout << "[" << node->sym << "]" << endl;

        showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
    }
}

double round(double x) {
    return floor(x * 1e8) / 1e8;
}
complex<double> round(complex<double> x) {
    return { round(x.real()),round(x.imag()) };
}
complex<double> power(complex<double> x, complex<double> y) {
    return  exp(y * log(x));
}

complex<double> stoc (const string &input) {
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

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
template<class T> T getstack (vector<T> &stack) {
    T val = stack.back();
    stack.pop_back();
    return val;
}

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

string parenthesis(vector<string>::iterator &it) {

    it += 1;
    int pile = 1;
    string sub ;

    while (true) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it++;
    }

    // it++;
    return sub;
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

string operate (const string &t1, const string &oper, const string &t2) {

    switch (oper[0]) {
        case '+' : return ctos (round(stoc(t1) + stoc(t2))) ; break;
        case '-' : return ctos (round(stoc(t1) - stoc(t2))) ; break;
        case '*' : return ctos (round(stoc(t1) * stoc(t2))) ; break;
        case '/' : return ctos (round(stoc(t1) / stoc(t2))) ; break;
        case '^' : return ctos (round(pow(stoc(t1), stoc(t2)))) ; break;
    }

    return t1 + oper + t2;
}
node *div(node *a, node *b) {

    if (a->sym == b->sym) return new node ("1");
    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) / stoc(b->sym))));

    return new node ("/",a,b);
}
node *add(node *a, node *b) {

    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "0") return b;
    if (b->sym == "0") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) + stoc(b->sym))));

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    if (a->sym == b->sym) return new node("0");
    if (b->sym == "0") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) - stoc(b->sym))));

    return new node ("-",a,b);
}
node *mul(node *a, node *b) {

    //if (a->sym == b->sym) return new node ("^",a, new node("2"));
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (a->sym == "1") return b;
    if (b->sym == "1") return a;
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(stoc(a->sym) / stoc(b->sym))));

    return new node ("*",a,b);
}
node *exp(node *a, node *b) {

    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");

    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(round(pow(stoc(a->sym), stoc(b->sym)))));
    return new node("^",a,b);
}

node *parse (const string &input) {

    vector<string> code = tokenize(input), oper;
    vector<string>::iterator it = code.begin();
    vector<node*> tree;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( new node(cell));
        } else if (cell == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (is_number(cell)) {
            tree.push_back( new node(cell));
        } else if (is_operator(cell)) {

            while(!oper.empty() && order(oper.back()) > order(cell)) {
                node *next = new node(getstack(oper));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back(cell);
        } else if (is_func(cell)) {
            it++;
            node *next = new node(cell);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            cout << "invalid identifier\n";
        }

        it++;
    }


    while(!oper.empty()) {
        node *next = new node(getstack(oper));
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}

string evaluate (node *node, string value = "") {

    if (node == nullptr) return "";

    string term = node->sym;
    string t1 = evaluate(node->t1, value), t2 = evaluate(node->t2, value);

    if (term == "x") {
        return value == "" ? term : value;
    } else if (is_operator(term)) {
        // cout << "[" << t1 << "]" << term << "[" << t2 << "]\n";
        if (is_number(t1) && is_number(t2)) {
            return operate(t1,term,t2);
        }
    } else if (is_number(t1)) {
        value_t val = stoc(t1);

        if (term == "cos") {
            return ctos(round(cos(val)));
        } else if (term == "sin") {
            return ctos(round(sin(val)));
        } else if (term == "tan") {
            return ctos(round(tan(val)));
        } else if (term == "log") {
            return ctos(round(log(val)));
        } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
            return ctos(round( cos(val) / sin(val) ));
        }
    }

    return t1 + term + t2;
}
node *derivate(node *curr) {

    string term = curr->sym;
    node *t1 = curr->t1, *t2 = curr->t2;
    // string e1 = evaluate(curr->t1), e2 = evaluate(curr->t2);
    if (term == "x") {
        return new node("1");
    } else if (is_number(term)) {
        return new node("0");
    } else if (term == "+") {
        // node *d1 = derivate(t1), *d2 = derivate(t2);
        // cout << "[" << evaluate(d1) << "](" << term << ")[" << evaluate(d2) << "]\n";
        return add(derivate(t1), derivate(t2));
    } else if (term == "-") {
        //cout << "-" << endl;
        return sub(derivate(t1), derivate(t2));
    } else if (term == "*") {
        node *a1 = mul(t1,derivate(t2)), *a2 = mul(derivate(t1),t2);
        // cout << "[" << evaluate(a1) << "](" << term << ")[" << evaluate(a2) << "]\n";
        // cout << a1->sym << " " << a2->sym << "\n";
        // cout << "[" << evaluate(add(a1,a2)) << "]\n";
        return add(a1, a2);
    } else if (term == "/") {
        node *num = sub(mul(derivate(t1),t2),mul(t1,derivate(t2)));
        node *den = mul(t2,t2);
        // cout << "[" << evaluate(d1) << "](" << term << ")[" << evaluate(d2) << "]\n";
        // cout << "[" << evaluate(num) << "](" << term << ")[" << evaluate(den) << "]\n";
        return div(num, den) ;
    } else if (term == "^") {
        node *a1 = exp(t1, t2);
        node *inner = add(div(t2,t1), mul(derivate(t2), new node("log", t1)));
        return mul(a1,inner);
    } else if (term == "cos") {
        return sub(new node("0"), new node("sin", t1));
    } else if (term == "sin") {
        return new node("cos", t1);
    } else if (term == "tan") { // dx = 1 / (cos(x))^2
        return div(derivate(t1), exp(new node("cos", t1),new node("2")) );
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {

    }

    return nullptr;
}

tuple<func_t,func_t,func_t> differential(const string &expression) {

    node *pass0 = parse(expression);
    node *pass1 = derivate(pass0);
    node *pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return stoc(evaluate(pass0, ctos(x))); },
            [pass1](value_t x) { return stoc(evaluate(pass1, ctos(x))); },
            [pass0](value_t x) { return stoc(evaluate(pass0, ctos(x))); },
    };
}
string traverse (node *curr) {

    if (curr != nullptr) {
        string symbol = curr->sym;
        string a = traverse(curr->t1), b = traverse(curr->t2);

        if (is_operator(symbol)) {
            // cout << "[" << a << "](" << symbol << ")[" << b << "]\n";
        } else {
            cout << "[" << symbol << "]";
        }

        return a + symbol + b;
    }

    return "";
}

int main () {

    string input = "2*x^3";

    node *pass0 = parse(input);
    node *pass1 = derivate(pass0);
    node *pass2 = derivate(pass1);
    // showtree(pass3);
    // showtree(pass4);
    // cout << evaluate(pass0, "(2,2)") << "\n";
    // cout << evaluate(pass1, "(2,2)") << "\n";
    // cout << evaluate(pass2, "(2,2)") << "\n";


  cout << "\nend\n";
}
