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
    node *t1, *t2;

    node (const string &label = "", node *t1 = nullptr, node *t2 = nullptr) : sym (label), t1 (t1), t2 (t2) {}
};
////////////////////////////////////////////////////////////////////////
void show (const std::vector<std::string> &vs) {

    for (size_t i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }

    cout << endl;
}
void showtree(const node *node, bool isLeft = false, const string &prefix = "") {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├─" : "└─" );
        cout << "[" << node->sym << "]" << endl;

        showtree(node->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(node->t2, false, prefix + (isLeft ? "│  " : "   "));
    }
}
///////////////////////////////////////////////////////////////////////
vector<string> tokenize(const string &input) {
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
complex<double> stoc (const string &input) {
    istringstream iss(input);
    complex<double> zx;
    iss >> zx;
    return zx;
}
string ctos(const complex<double> &zx) {
    ostringstream oss;
    oss << fixed << setprecision(30) << zx; // zx.imag() == 0 ? oss << fixed << zx.real() : oss << fixed << zx;
    return oss.str();
}

int order (const string &src) {
    if (src == "+" || src == "-") return 1;
    if (src == "*" || src == "/") return 2;
    if (src == "^") return 3;
    return 0;
}
bool precedence(vector<string> &stack, string cell) {
    if (stack.empty()) return false;
    // if (cell == "^") return order(stack.back()) > order(cell);
    return order(stack.back()) >= order(cell);
}
template<class T> T getstack (vector<T> &stack) {
    T val = stack.back();
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

    return sub;
}

bool operable(node *a, node *b) {
    return regex_match(a->sym, number) && regex_match(b->sym, number);
}
string operate (const string &t1, const string &oper, const string &t2) {

    switch (oper[0]) {
        case '+' : return ctos (stoc(t1) + stoc(t2)) ; break;
        case '-' : return ctos (stoc(t1) - stoc(t2)) ; break;
        case '*' : return ctos (stoc(t1) * stoc(t2)) ; break;
        case '/' : return ctos (stoc(t1) / stoc(t2)) ; break;
        case '^' : return ctos (pow(stoc(t1), stoc(t2))) ; break;
    }

    return t1 + oper + t2;
}
node *div(node *a, node *b) {

    if (a->sym == "0") return new node ("0");
    if (b->sym == "1") return a;
    if (a->sym == b->sym && !regex_match(a->sym, oper)) return new node ("1");
    if (operable(a,b)) return new node(ctos(stoc(a->sym) / stoc(b->sym)));

    return new node ("/",a,b);
}
node *add(node *a, node *b) {

    if (a->sym == "0") return b;
    if (b->sym == "0") return a;
    if (a->sym == b->sym && !regex_match(a->sym, oper)) return new node ("*",new node("2"), a);
    if (operable(a,b)) return new node(ctos(stoc(a->sym) + stoc(b->sym)));

    return new node ("+",a,b);
}
node *sub(node *a, node *b) {

    if (b->sym == "0") return a;
    if (a->sym == b->sym && !regex_match(a->sym, oper)) return new node("0");
    if (operable(a,b)) return new node(ctos(stoc(a->sym) - stoc(b->sym)));

    return new node ("-",a,b);
}
node *mul(node *a, node *b) {

    if (a->sym == "1") return b;
    if (b->sym == "1") return a;
    if (a->sym == b->sym && !regex_match(a->sym, oper)) return new node ("^",a, new node("2"));
    if (a->sym == "0" || b->sym == "0") return new node ("0");
    if (operable(a,b)) return new node(ctos(stoc(a->sym) * stoc(b->sym)));

    return new node ("*",a,b);
}
node *exp(node *a, node *b) {
    // frexp
    if (a->sym == "1" || b->sym == "1") return a;
    if (b->sym == "0") return new node("1");
    if (a->sym == "0") return new node("0");

    if (operable(a,b)) {
        return new node(ctos(pow(stoc(a->sym), stoc(b->sym))));
    }

    return new node("^",a,b);
}

node *parse (const string &input) {

    vector<string> code = tokenize(input), stack;
    vector<string>::iterator it = code.begin();
    vector<node*> tree;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( new node(cell));
        } else if (cell == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (regex_match(cell, number)) {
            tree.push_back( new node(cell));
        } else if (regex_match(cell, oper)) {
            while (precedence(stack,cell)) {
                node *next = new node(getstack(stack));
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            stack.push_back(cell);
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

        node *next = new node(getstack(stack));
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}
string evaluate (node *node, string value = "") {

    if (node == nullptr) return "";

    string term = node->sym;
    string a = evaluate(node->t1, value), b = evaluate(node->t2, value);

    if (term == "x") {
        return value == "" ? term : value;
    } else if (regex_match(term,number)) {
        return term;
    } else if (regex_match(term,oper)) {
        if (regex_match(a, number) && regex_match(b, number)) {
            // cout << "[" << a <<  "]" << term << "[" << b << "] => " << operate(a,term,b);
            // cout <<"\n";
            return operate(a,term,b);
        }
    } else if (regex_match(a, number)) {
        value_t val = stoc(a);

        if (term == "cos") {
            return ctos(cos(val));
        } else if (term == "sin") {
            return ctos(sin(val));
        } else if (term == "tan") {
            return ctos(tan(val));
        } else if (term == "log") {
            return ctos(log(val));
        } else if (term == "cot") { //cot(x) = cos(x)/sin(x) or cot(x) = 1 / tan(x)
            return ctos( cos(val) / sin(val) );
        }
    }

    return a + term + b;
}
complex<double> evaluate2 (node *node, complex<double> value) {

    if (node == nullptr) return 0;

    string term = node->sym;
    complex<double> a = evaluate2(node->t1, value), b = evaluate2(node->t2, value);
    // cout << "[" << a <<  "]" << term << "[" << b << "]\n";
    if (term == "x") {
        return value;
    } else if (regex_match(term,number)) {
        return stoc(term);
    } else if (regex_match(term,oper)) {
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
node *derivate(node *curr) {

    string term = curr->sym;
    node *t1 = curr->t1, *t2 = curr->t2;
    //cout << "[" << evaluate(inner) << "](" << "*" << ")[" << evaluate(outer) << "]\n";

    if (term == "x") {
        return new node("1");
    } else if (regex_match(term, number)) {
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
        if (t1->sym == "x" && regex_match(t2->sym, number)) {
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
tuple<func_t,func_t,func_t> differentiate(const string &expression) {

    node *pass0 = parse(expression);
    node *pass1 = derivate(pass0);
    node *pass2 = derivate(pass1);

    return {
        [pass0](value_t x) { return stoc(evaluate(pass0, ctos(x))); },
        [pass1](value_t x) { return stoc(evaluate(pass1, ctos(x))); },
        [pass2](value_t x) { return stoc(evaluate(pass2, ctos(x))); },
    };
}


int main () {

  complex<double> x (3.41,-8.97);
  string expr = "x^x-x/x^91.1-22.9^x";

  node *pass0 = parse(expr);
  node *pass1 = derivate(pass0);
  node *pass2 = derivate(pass1);

  // showtree(pass1);
  cout << evaluate(pass2, ctos(x)) << "\n";
  cout << evaluate2(pass2, x) << "\n";
/*

The first derivative failed! f(x) = "x^73.4+70.1^73.8*x^58.9", x = "(-0.2,-0.5)"

Expected: equal to (2.61549e+122,3.07174e+121) (+/- (8.06953e+118,1.00869e+118))
Actual: (0,0)



*/



    cout << "\nend\n";
}
