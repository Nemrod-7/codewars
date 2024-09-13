#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <complex>
#include <functional>
#include <iomanip>

using namespace std;
using value_t = complex<double>;
using func_t = function<value_t(value_t)>;

// Let f be a function.
// The derivative function, denoted by f′, is the function whose domain consists of those values of x
// such that the following limit exists:
// f′(x) = lim h→0 of (f(x + h) − f(x)) / h.
//
// con : cst   => 0
// lin : x     => 1
// add : a + b => a' + b'
// min : a - b => a' - b'
// mul : a * b => a.b' + a'.b
// div : a / b => (a'* b − b'* a) / (b * b)
// exp : x^y   => x^y . (x'.(y/x) + y'.log(x))
// log : ln(x) => x' / x
// sin : sin x => cos x
// cos : cos x => -sin x
// tan : tan x => x' / (cos(x))^2
// cot = cot x = -1 / (sin(x))^2

struct node {
    string sym;
    complex<double> val;
    node *t1, *t2;

    node (const string &label, node *t1 = nullptr, node *t2 = nullptr) : sym (label), val(0.0,0.0), t1 (t1), t2 (t2) {}
    node (const complex<double> &value, node *t1 = nullptr, node *t2 = nullptr) : sym (""), val(value), t1 (t1), t2 (t2) {}
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

complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
string ctos(const complex<double> &zx) {
    ostringstream oss;
    oss << fixed << setprecision(30) << zx;
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
bool precedence (vector<string> &stack, string cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()) > order(cell);
    return order(stack.back()) >= order(cell);
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

node *div (node *a, node *b) {

    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (a->sym == b->sym && !is_operator(a->sym)) return new node ("1");
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(stoc(a->sym) / stoc(b->sym)));

    return new node ("/",a,b);
}
node *add (node *a, node *b) {

    if (a->sym == "0") return b;
    if (b->sym == "0") return a;
    if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(stoc(a->sym) + stoc(b->sym)));

    return new node ("+",a,b);
}
node *sub (node *a, node *b) {

    if (b->sym == "0") return a;
    if (a->sym == b->sym && !is_operator(a->sym)) return new node("0");
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(stoc(a->sym) - stoc(b->sym)));

    return new node ("-",a,b);
}
node *mul (node *a, node *b) {

    if (a->sym == "1") return b;
    if (b->sym == "1") return a;
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (a->sym == b->sym && !is_operator(a->sym)) return new node ("^",a, new node("2"));
    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(stoc(a->sym) * stoc(b->sym)));

    return new node ("*",a,b);
}
node *exp (node *a, node *b) {
    // frexp
    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");
    // cout << "[" << a->sym <<  "]" << "^" << "[" << b->sym << "] => " << "\n";

    if (is_number(a->sym) && is_number(b->sym)) return new node(ctos(pow(stoc(a->sym), stoc(b->sym))));
    return new node("^",a,b);
}

node *parse (const string &input) {

    vector<string> code = tokenize(input), oper;
    vector<string>::iterator it = code.begin();
    vector<node*> tree;

    while (it < code.end()) {
        string curr = *it;

        if (curr == "x") {
            tree.push_back( new node(curr));
        } else if (curr == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (is_number(curr)) {
            tree.push_back( new node(curr));
        } else if (is_operator(curr)) {

            while (precedence(oper, curr)) {
                node *next = new node(getstack(oper));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back(curr);
        } else if (is_func(curr)) {
            it++;
            node *next = new node(curr);
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
complex<double> evaluate (node *node, complex<double> value) {

    if (node == nullptr) return 0;

    string term = node->sym;
    complex<double> a = evaluate(node->t1, value), b = evaluate(node->t2, value);
    // cout << "[" << a <<  "]" << term << "[" << b << "]\n";
    if (term == "x") {
        return value;
    } else if (is_number(term)) {
        return stoc(term);
    } else if (is_operator(term)) {
        switch (term[0]) {
            case '+' : return a + b; break;
            case '-' : return a - b; break;
            case '*' : return a * b; break;
            case '/' : return a / b; break;
            case '^' : return pow(a, b); break;
        }
    } else {
        complex<double> val = a;

        if (term == "cos") {
            return cos(val);
        } else if (term == "sin") {
            return sin(val);
        } else if (term == "tan") {
            return tan(val);
        } else if (term == "log") {
            return log(val);
        } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
            return cos(val) / sin(val);
        } else {
            cout << "Invalid operator\n";
        }

    }

    return 0;
}
node *derivate (node *curr) {

    string term = curr->sym;
    node *t1 = curr->t1, *t2 = curr->t2;

    if (term == "x") {
        return new node("1");
    } else if (is_number(term)) {
        return new node("0");
    } else if (term == "+") {
        return add(derivate(t1), derivate(t2));
    } else if (term == "-") {
        return sub(derivate(t1), derivate(t2));
    } else if (term == "*") {
        return add(mul(t1,derivate(t2)), mul(derivate(t1),t2));
    } else if (term == "/") {
        node *num = sub(mul(derivate(t1),t2),mul(t1,derivate(t2)));
        node *den = mul(t2,t2);
        return div(num, den) ;
    } else if (term == "^") {
        if (t1->sym == "x" && is_number(t2->sym)) {
            return mul(t2, exp( t1, sub(t2, new node("1")) ) ) ;
        }
        node *outer = exp(t1, t2);
        node *inner = add( mul( derivate(t1), div(t2,t1) ), mul( derivate(t2), new node("log", t1) ));
        return mul(inner,outer);
    } else if (term == "cos") {
        return sub(new node("0"), mul(derivate(t1), new node("sin", t1)));
    } else if (term == "sin") {
        return mul(derivate(t1), new node("cos", t1)) ;
    } else if (term == "tan") { // dx = 1 / (cos(x))^2
        return div(derivate(t1), exp(new node("cos", t1), new node("2")) );
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {
        return sub(new node("0"), div(derivate(t1), new node("^", new node("sin", t1), new node("2")))) ;
    }

    return nullptr;
}
tuple<func_t,func_t,func_t> differentiate (const string &expression) {

    node *pass0 = parse(expression);
    node *pass1 = derivate(pass0);
    node *pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return evaluate(pass0, x); },
        [pass1](value_t x) { return evaluate(pass1, x); },
        [pass2](value_t x) { return evaluate(pass2, x); },
    };
}


int main () {

  complex<double> x (3.41,-8.97);
  string expr = "x^x-x/x^91.1-22.9^x";

  node *pass0 = parse(expr);
  // node *pass1 = derivate(pass0);
  // node *pass2 = derivate(pass1);

  // showtree(pass1);
  // cout << evaluate(pass2, ctos(x)) << "\n";
  // cout << evaluate(pass2, x) << "\n";

  /*
  The first derivative failed! expr = "62.2/79.1^x-15.8", x = (-6.92,-7.35)
  Expected: equal to (-2.81866e+15,-2.41691e+15) (+/- (1.09951e+12,1.09951e+12))
  Actual: (0,0) => ok
  */

  /*
  The first derivative failed! expr = "5.7/17.7/69.3^x-x^x*x^51.3", x = (-9.24,-8.64)
  Expected: equal to (1.32083e+37,1.04149e+37) (+/- (5.1923e+33,2.59615e+33))
  Actual: (0,0) => ok
  */

  /*
  The first derivative failed! expr = "x^x+66.5^27.8/x*x", x = (1.33,4.47)
  Expected: equal to (1.55769e+34,-2.07692e+34) (+/- (5.0706e+30,1.01412e+31))
  Actual: (-5.1923e+33,-4.15384e+34)
  */

  /*
  The second derivative failed! expr = "x^x-x/x^91.1-22.9^x", x = (3.41,-8.97)
  Expected: equal to (417561,79455) (+/- (128,32))
  Actual: (0.532243,0.0157056)
  */





    cout << "\nend\n";
}
