#include <iostream>
#include <iomanip>
#include <vector>
#include <regex>
#include <cmath>
#include <tuple>
#include <complex>
#include <functional>

#include "tests.hpp"

using namespace std;
using value_t = complex<double>;
using func_t = function<value_t(value_t)>;

const regex oper ("^[-+*/^]$");
const regex trig ("^sin|cos|tan|cot|log$");
const regex number ("^-?\\d+(.\\d+)?|\\(-?\\d+(.\\d+)?,-?\\d+(.\\d+)?\\)$");

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
// sin : sin x => x'.cos x
// cos : cos x => -x'.sin x
// tan : tan x => x' / (cos(x))^2
// cot = cot x = -x' / (sin(x))^2

struct node {
    string sym;
    complex<double> val;
    node *t1, *t2;


    node (const string &label, node *t1 = nullptr, node *t2 = nullptr) : sym (label), val(0.0,0.0), t1 (t1), t2 (t2) {}
    node (const complex<double> &value, node *t1 = nullptr, node *t2 = nullptr) : sym (""), val(value), t1 (t1), t2 (t2) {}
};
////////////////////////////////////////////////////////////////////////
void show (const std::vector<std::string> &vs) {

    for (size_t i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }

    cout << endl;
}
void showtree (const node *node, bool isLeft = false, const string &prefix = "") {
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
///////////////////////////////////////////////////////////////////////
complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
vector<string> tokenize (const string &input) {
    const regex tokn ("([0-9]+(\\.[0-9]+)?)|x|[-+*/^()]|(sin|cos|tan|cot|log)");
    const regex oper ("^[-+*/^]$");
    sregex_token_iterator iter (input.begin (), input.end (), tokn);
    vector<string> temp (iter, sregex_token_iterator ()), code;

    for (size_t i = 0; i < temp.size(); i++) {
        if (temp[i] == "-" && (i == 0 || regex_match(temp[i-1], oper))) {
            code.push_back("-" + temp[i + 1]);
            i += 2;
        }
        code.push_back(temp[i]);
    }

    return temp;
}

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
node *getstack (vector<node*> &stack) {
    node *val = stack.back();
    stack.pop_back();
    return val;
}
bool precedence (const vector<node*> &stack, const string &cell) {
    if (stack.empty()) return false;
    if (cell == "^") return order(stack.back()->sym) > order(cell);
    return order(stack.back()->sym) >= order(cell);
}
string parenthesis (vector<string>::iterator &it) {

    it += 1;
    int pile = 1;
    string sub ;

    while (true) {
        pile += (*it == "(") - (*it == ")");
        if (pile == 0) break;
        sub += *it++;
    }

    return sub;
}

node *add(node *a, node *b) {
    // if (a->sym == b->sym && !is_operator(a->sym)) return new node ("*",new node("2"), a);
    if (a->sym == "" && a->val == 0.0) return b;
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val + b->val);

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    //if (a->sym == b->sym) return new node("0");
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val - b->val);
    return new node ("-",a,b);
}
node *exp(node *a, node *b) {

    if (a->val == 1.0 || b->val == 1.0) return a;
    if (b->sym == "" && b->val == 0.0) return new node(1.0);
    if (a->sym == "" && b->sym == "") return new node(pow(a->val , b->val));

    return new node("^",a,b);
}
node *div(node *a, node *b) {

    //if (a->sym == b->sym) return new node (1.0);
    if (a->sym == "" && a->val == 0.0) return new node(0.0);
    if (b->sym == "" && b->val == 1.0) return a;
    if (a->sym == "" && b->sym == "") return new node(a->val / b->val);

    return new node ("/",a,b);
}
node *mul(node *a, node *b) {

    if (a->val == 1.0) return b;
    if (b->val == 1.0) return a;
    if ((a->sym == "" && a->val == 0.0) || (b->sym == "" && b->val == 0.0)) return new node (0.0);
    if (a->sym == "" && b->sym == "") return new node(a->val * b->val);
    return new node ("*",a,b);
}


node *parse (const string &input) {

    vector<string> code = tokenize(input);
    vector<string>::iterator it = code.begin();
    vector<node*> tree, stack;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( new node(cell));
        } else if (cell == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (regex_match(cell, number)) {
            tree.push_back( new node(stoc(cell)));
        } else if (regex_match(cell, oper)) {
            while (!stack.empty() && precedence(stack,cell)) {
                node *next = getstack(stack);
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }
            stack.push_back(new node(cell));
        } else if (regex_match(cell, trig)) {
            it++;
            node *next = new node(cell);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            cout << "invalid identifier\n";
        }

        it++;
    }

    while(!stack.empty()) {
        node *next = getstack(stack);
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}
node *derivate (const node *curr) {

    string term = curr->sym;
    node *t1 = curr->t1, *t2 = curr->t2;
    //cout << "[" << evaluate(inner) << "](" << "*" << ")[" << evaluate(outer) << "]\n";

    if (term == "x") {
        return new node(1.0);
    } else if (term == "") {
        return new node(0.0);
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
        if (t1->sym == "x" && regex_match(t2->sym, number)) {
            return mul(t2, exp( t1, sub(t2, new node(1.0)) ) ) ;
        }
        node *outer = exp(t1, t2);
        node *inner = add( mul( derivate(t1), div(t2,t1) ), mul(derivate(t2), new node("log", t1)));
        return mul(inner,outer);
    } else if (term == "cos") {
        return sub(new node(0.0), mul(derivate(t1), new node("sin", t1)));
    } else if (term == "sin") {
        return mul(derivate(t1), new node("cos", t1)) ;
    } else if (term == "tan") { // dx = 1 / (cos(x))^2
        return div(derivate(t1), exp(new node("cos", t1), new node(2.0)) );
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {
        return sub(new node(0.0), div(derivate(t1), new node("^", new node("sin", t1), new node(2.0)))) ;
    }

    return nullptr;
}
complex<double> evaluate (const node *node, const complex<double> &value) {

    if (node == nullptr) return 0;

    string term = node->sym;
    complex<double> a = evaluate(node->t1, value), b = evaluate(node->t2, value);
    // cout << "[" << a <<  "]" << term << "[" << b << "]\n";
    if (term == "x") {
        return value;
    } else if (term == "") {
        return node->val;
    } else if (regex_match(term,oper)) {
        switch (term[0]) {
            case '+' : return a + b; break;
            case '-' : return a - b; break;
            case '*' : return a * b; break;
            case '/' : return a / b; break;
            case '^' : return pow(a, b); break;
        }
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
        cout << "[" << term << "]"  <<  " : Invalid operator\n";
    }

    return 0;
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

    complex<double> x(-3.25,2.95);
    string expr = "cot(x/45.3^x*x^32.3+x^85.1/x^20.9/x-5^45.3*86.2^67.6/x^x)^x";

    node *pass0 = parse(expr);
    // node *pass1 = derivate(pass0);
    // node *pass2 = derivate(pass1);

    // showtree(pass0);
    cout << evaluate(pass0,x) << "\n";
    // showtree(pass0);

    // cout << evaluate(pass2, x) << "\n";

    tests();


    cout << "\nend\n";
}
