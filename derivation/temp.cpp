#include <iostream>
#include <memory>
#include <complex>
#include <functional>

// #include "tests.hpp"

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

struct ast {
    std::string sym;
    std::complex<double> val;
    std::shared_ptr<ast> t1, t2;

    ast(const std::string &src, std::shared_ptr<ast> a = nullptr, std::shared_ptr<ast> b = nullptr) : sym(src), val(0.0), t1 (a), t2 (b) {}
    ast(std::complex<double> src, std::shared_ptr<ast> a = nullptr, std::shared_ptr<ast> b = nullptr) : sym(""), val(src), t1 (a), t2 (b) {}
};

using namespace std;
using value_t = std::complex<double>;
using func_t = std::function<value_t(value_t)>;
using node = std::shared_ptr<ast>;

node add(node a, node b) {

    if (a->sym == "" && a->val == 0.0) return b;
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val + b->val);

    return make_shared<ast> ("+",a,b);
}
node sub(node a, node b) {

    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val - b->val);
    return make_shared<ast> ("-",a,b);
}
node exp(node a, node b) {

    if (a->val == 1.0 || b->val == 1.0) return a;
    if (b->sym == "" && b->val == 0.0) return make_shared<ast>(1.0);
    if (a->sym == "" && b->sym == "") return make_shared<ast>(pow(a->val , b->val));

    return make_shared<ast>("^",a,b);
}
node div(node a, node b) {

    if (a->sym == "" && a->val == 0.0) return make_shared<ast>(0.0);
    if (b->sym == "" && b->val == 1.0) return a;
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val / b->val);

    return make_shared<ast> ("/",a,b);
}
node mul(node a, node b) {

    if (a->val == 1.0) return b;
    if (b->val == 1.0) return a;
    if ((a->sym == "" && a->val == 0.0) || (b->sym == "" && b->val == 0.0)) return make_shared<ast> (0.0);
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val * b->val);

    return make_shared<ast> ("*",a,b);
}

complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
bool is_term (const string &sym) { return sym == "+" || sym == "-"; }
bool is_fact (const string &sym) { return sym == "*" || sym == "/"; }
bool is_operator (const string &input) {
    return is_fact(input) || is_term(input) || input == "^";
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
bool is_func (const string &input) {
    return input == "sin" || input == "cos" || input == "tan" || input == "log" || input == "cot";
}

node getstack (vector<node> &stack) {
    node val = stack.back();
    stack.pop_back();
    return val;
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
int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
bool precedence (const vector<node> &stack, const string &cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()->sym) > order(cell);
    return order(stack.back()->sym) >= order(cell);
}

node parse (const string &input) {

    vector<string> code = tokenize(input);
    vector<string>::iterator it = code.begin();
    vector<node> tree, oper;

    while (it < code.end()) {
        string curr = *it;

        if (curr == "x") {
            tree.push_back( make_shared<ast>(curr));
        } else if (curr == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (is_number(curr)) {
            tree.push_back( make_shared<ast>(stoc(curr)));
        } else if (is_operator(curr)) {
            while (!oper.empty() && precedence(oper,curr)) {
                node next = getstack(oper);
                next->t2 = getstack(tree), next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back( make_shared<ast>(curr));
        } else if (is_func(curr)) {
            it++;
            node next = make_shared<ast>(curr);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            cout << "Invalid identifier\n";
        }

        it++;
    }

    while (!oper.empty()) {
        node next = getstack(oper);
        next->t2 = getstack(tree), next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}
complex<double> evaluate (const node &node, const complex<double> &value) {

    if (node == nullptr) return 0;

    string term = node->sym;
    complex<double> a = evaluate(node->t1, value), b = evaluate(node->t2, value);

    if (term == "x") {
        return value;
    } else if (term == "") {
        return node->val;
    } else if (is_operator(term)) {
        complex<double> xz;

        switch (term[0]) {
            case '+' : xz = a + b; break;
            case '-' : xz = a - b; break;
            case '*' : xz = a * b; break;
            case '/' : xz = a / b; break;
            case '^' : xz = pow(a, b); break;
        }
        //cout << "[" << a <<  "]" << term << "[" << b << "] => ";
        //cout << xz << "\n";
        return xz;
    } else if (term == "cos") {
        return cos(a);
    } else if (term == "sin") {
        return sin(a);
    } else if (term == "tan") {
        return tan(a);
    } else if (term == "log") {
        return log(a);
    } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
        return 1.0 / tan(a);
    } else {
        cout << "Invalid operator\n";
    }

    return 0;
}
node derivate (const node curr) {

    string term = curr->sym;
    node t1 = curr->t1, t2 = curr->t2;
    //cout << "[" << term << "|" << curr->val << "]" << "\n" << flush;
    if (term == "x") {
        return  make_shared<ast>(1.0);
    } else if (term == "") {
        return  make_shared<ast>(0.0);
    } else if (term == "+") {
        return add(derivate(t1), derivate(t2));
    } else if (term == "-") {
        return sub(derivate(t1), derivate(t2));
    } else if (term == "*") {
        return add(mul(t1,derivate(t2)), mul(derivate(t1),t2));
    } else if (term == "/") {
        node num = sub(mul(derivate(t1),t2),mul(t1,derivate(t2)));
        node den = exp(t2, make_shared<ast>(2.0));
        return div(num, den) ;
    } else if (term == "^") {
        node outer = exp(t1, t2);
        node inner = add( mul( derivate(t1), div(t2,t1) ), mul( derivate(t2),  make_shared<ast>("log", t1) ));
        return mul(inner,outer);
    } else if (term == "cos") {
        return sub( make_shared<ast>(0.0), mul(derivate(t1),  make_shared<ast>("sin", t1)));
    } else if (term == "sin") {
        return mul(derivate(t1),  make_shared<ast>("cos", t1)) ;
    } else if (term == "tan") { // dx = x' / (cos(x))^2
        return div(derivate(t1), exp( make_shared<ast>("cos", t1),  make_shared<ast>(2.0)));
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {
        return sub( make_shared<ast>(0.0), div(derivate(t1),  make_shared<ast>("^",  make_shared<ast>("sin", t1),  make_shared<ast>(2.0)))) ;
    }

    return nullptr;
}

tuple<func_t,func_t,func_t> differentiate (const string &expression) {

    node pass0 = parse(expression);
    node pass1 = derivate(pass0);
    node pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return evaluate(pass0, x); },
            [pass1](value_t x) { return evaluate(pass1, x); },
            [pass2](value_t x) { return evaluate(pass2, x); },
    };
}

void showtree(const node node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├─" : "└─" );

        if (node->sym == "") {
            cout << "[" << node->val << "]" << endl;
        } else {
            cout << "[" << node->sym << "]" << endl;
        }

        showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
    }
}


int main () {

    // exp : x^y   => x^y . (x'.(y/x) + y'.log(x))
    // x^63 => x^63 * (63/x)) => 63 * x^62

    complex<double> x(-3.35,3.35) ;
    string expr = "x^63+58.5+19.2*38*55.2";

    node pass0 = parse(expr);
    node pass1 = derivate(pass0);
    // node *pass2 = derivate(pass1);

    node curr = make_shared<ast>("log");
    curr->t1 = make_shared<ast>("x");

    //showtree(pass1);
    //showflat(pass1);
    //ast t1 = mul(ast(63.0), exp(ast(x), ast(62.0)));
    // ast t2 = mul( exp(new ast(x), new ast(63.0)), div(new ast(63.0),  new ast(x)) );

    // complex<double> x1 = pow(x, 3);
    // complex<double> x2 = pow(x, 2);
    // complex<double> ex = {-16, 16} ; // pow(x, 3.0);
    // ex= pow(x, 3.0);
    //    cout << fixed;
    //
    //    cout << pow(x, 62.0) * 63.0 << "\n";
    //    cout << pow(x, 63.0) * 63.0 / x << "\n";
    //    cout << pow(x, 63.0) / x * 63.0 << "\n";

    tests();

    // shared_ptr<ast> tree =


    cout << "\nexit\n";
}
