#include <iostream>
#include <memory>
#include <vector>
#include <regex>
#include <cmath>
#include <tuple>
#include <complex>
#include <functional>

// use of regex

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

struct ast {
    std::string sym;
    std::complex<double> val;

    std::shared_ptr<ast> t1, t2;
    ast (const std::string &label, std::shared_ptr<ast> t1 = nullptr, std::shared_ptr<ast> t2 = nullptr) : sym (label), val(0.0,0.0), t1 (t1), t2 (t2) {}
    ast (const std::complex<double> &value, std::shared_ptr<ast> t1 = nullptr, std::shared_ptr<ast> t2 = nullptr) : sym (""), val(value), t1 (t1), t2 (t2) {}
};

using namespace std;
using value_t = std::complex<double>;
using func_t = std::function<value_t(value_t)>;
using node = std::shared_ptr<ast>;

const std::regex oper ("^[-+*/^]$");
const std::regex trigo ("^sin|cos|tan|cot|log$");
const std::regex number ("^-?\\d+(.\\d+)?|\\(-?\\d+(.\\d+)?,-?\\d+(.\\d+)?\\)$");

////////////////////////////////////////////////////////////////////////
void show (const std::vector<std::string> &vs) {

    for (size_t i = 0; i < vs.size(); i++) {
        std::cout << "[" << vs[i] << "]";
    }

    std::cout << endl;
}
void showtree (const node &curr, bool isLeft = false, const std::string &prefix = "") {
    if (curr != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "├─" : "└─" );

        if (curr->sym == "") {
            std::cout << "[" << curr->val << "]" << std::endl;
        } else {

            std::cout << "[" << curr->sym << "]" << std::endl;
        }

        showtree(curr->t1, true, prefix + (isLeft ? "│  " : "   "));
        showtree(curr->t2, false, prefix + (isLeft ? "│  " : "   "));
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

node getstack (vector<node> &stack) {
    node val = stack.back();
    stack.pop_back();
    return val;
}
bool precedence (const vector<node> &stack, const string &cell) {
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

node add (node a, node b) {

    if (a->sym == "" && a->val == 0.0) return b;
    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val + b->val);

    return make_shared<ast> ("+",a,b);
}
node sub (node a, node b) {

    if (b->sym == "" && b->val == 0.0) return a;
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val - b->val);
    return make_shared<ast> ("-",a,b);
}
node exp (node a, node b) {

    if (a->val == 1.0 || b->val == 1.0) return a;
    if (b->sym == "" && b->val == 0.0) return make_shared<ast>(1.0);
    if (a->sym == "" && b->sym == "") return make_shared<ast>(pow(a->val , b->val));

    return make_shared<ast>("^",a,b);
}
node div (node a, node b) {

    if (a->sym == "" && a->val == 0.0) return make_shared<ast>(0.0);
    if (b->sym == "" && b->val == 1.0) return a;
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val / b->val);

    return make_shared<ast> ("/",a,b);
}
node mul (node a, node b) {

    if (a->val == 1.0) return b;
    if (b->val == 1.0) return a;
    if ((a->sym == "" && a->val == 0.0) || (b->sym == "" && b->val == 0.0)) return make_shared<ast> (0.0);
    if (a->sym == "" && b->sym == "") return make_shared<ast>(a->val * b->val);

    return make_shared<ast> ("*",a,b);
}

node parse (const string &input) {

    vector<string> code = tokenize(input);
    vector<string>::iterator it = code.begin();
    vector<node> tree, stack;

    while (it < code.end()) {
        string cell = *it;

        if (cell == "x") {
            tree.push_back( std::make_shared<ast>(cell));
        } else if (cell == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (regex_match(cell, number)) {
            tree.push_back( std::make_shared<ast>(stoc(cell)));
        } else if (regex_match(cell, oper)) {
            while (!stack.empty() && precedence(stack,cell)) {
                node next = getstack(stack);
                next->t2 = getstack(tree), next->t1 = getstack(tree);
                tree.push_back(next);
            }
            stack.push_back(std::make_shared<ast>(cell));
        } else if (regex_match(cell, trigo)) {
            it++;
            node next = std::make_shared<ast>(cell);
            next->t1 = parse(parenthesis(it));
            tree.push_back(next);
        } else {
            std::cout << "invalid identifier\n";
        }

        it++;
    }

    while(!stack.empty()) {
        node next = getstack(stack);
        next->t2 = getstack(tree), next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}
node derivate (const node &curr) {

    string term = curr->sym;
    node t1 = curr->t1, t2 = curr->t2;
    //std::cout << "[" << evaluate(inner) << "](" << "*" << ")[" << evaluate(outer) << "]\n";

    if (term == "x") {
        return make_shared<ast>(1.0);
    } else if (term == "") {
        return make_shared<ast>(0.0);
    } else if (term == "+") {
        return add(derivate(t1), derivate(t2));
    } else if (term == "-") {
        return sub(derivate(t1), derivate(t2));
    } else if (term == "*") {
        return add(mul(t1,derivate(t2)), mul(derivate(t1),t2));
    } else if (term == "/") {
        node num = sub(mul(derivate(t1),t2),mul(t1,derivate(t2)));
        node den = exp(t2,make_shared<ast>(2.0));
        return div(num, den) ;
    } else if (term == "^") {
        node outer = exp(t1, t2);
        node inner = add( mul( derivate(t1), div(t2,t1) ), mul(derivate(t2), make_shared<ast>("log", t1)));
        return mul(inner,outer);
    } else if (term == "cos") {
        return sub(make_shared<ast>(0.0), mul(derivate(t1), make_shared<ast>("sin", t1)));
    } else if (term == "sin") {
        return mul(derivate(t1), make_shared<ast>("cos", t1)) ;
    } else if (term == "tan") { // dx = 1 / (cos(x))^2
        return div(derivate(t1), exp(make_shared<ast>("cos", t1), make_shared<ast>(2.0)) );
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {
        return sub(make_shared<ast>(0.0), div(derivate(t1), make_shared<ast>("^", make_shared<ast>("sin", t1), make_shared<ast>(2.0)))) ;
    }

    return nullptr;
}
complex<double> evaluate (const node &ast, const complex<double> &value) {

    if (ast == nullptr) return 0;

    string term = ast->sym;
    complex<double> a = evaluate(ast->t1, value), b = evaluate(ast->t2, value);
    // std::cout << "[" << a <<  "]" << term << "[" << b << "]\n";
    if (term == "x") {
        return value;
    } else if (term == "") {
        return ast->val;
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
        std::cout << "[" << term << "]"  <<  " : Invalid operator\n";
    }

    return 0;
}

string gethash(node curr) {

    if (curr == nullptr) return "";
    string hash;

    if (curr->sym == "") {
        hash = "1*0";
    } else if (regex_match(curr->sym, oper)) {
        hash += curr->t1->sym == "" ? '1' : 'x';
        hash += curr->sym;
        hash += curr->t2->sym == "" ? '1' : 'x';
    } else {
        hash = "x*0";
    }

    return hash;
}
node simplify (node curr) {

    if (curr != nullptr) {
        curr->t1 = simplify(curr->t1), curr->t2 = simplify(curr->t2);

        string hash = gethash(curr->t1) + curr->sym + gethash(curr->t2);
        node a = curr->t1, b = curr->t2;
        // cout << "[" << hash << "]\n";
        if (hash == "1*0*x*1") { // (5) * (x * 3)
            return mul(mul(a, b->t2), b->t1);
        } else if (hash == "1*0*1*x") { // (5) * (3 * x)
            return mul(mul(a, b->t1), b->t2);
        } else if (hash == "1*0*1/x") { // (5) * (4 / x)
            return div(mul(a, b->t1), b->t2);
        } else if (hash == "1/x*x/1") { // (5/x) * (x/2)
            return mul(div(a->t1,b->t2), div(a->t2,b->t1));
        } else if (hash == "x/1*1/x") { // (x/5) * (2/x)
            return mul(div(a->t1,b->t2), div(a->t2,b->t1));
        } else if (hash == "1/x*x*1") { // (5/x) * (x*4)
            return mul(mul(a->t1,b->t2), div(a->t2,b->t1));
        } else if (hash == "1/x*x^1") { // (5/x) * (x^3)
            return mul(a->t1, exp( b->t1, sub(b->t2, make_shared<ast>(1.0))));
        } else if (hash == "1*x*1*x") { // (5*x) * (4*x)
            return mul(mul(a->t1,b->t1), mul(a->t2,b->t2));
        } else if (hash == "1*x*x*1") { // (5*x) * (x*4)
            return mul(mul(a->t1,b->t2), mul(a->t2,b->t1));
        } else if (hash == "1*x*1/x") { // (3*x) * (2/x)
            return mul(mul(a->t1,b->t1), div(a->t2,b->t2)) ;
        } else if (hash == "x*1*1/x") {
            return mul(mul(a->t2,b->t1), div(a->t1,b->t2));
        } else  if (hash == "x^1*1/x") {
            if (a->t1->sym == b->t2->sym) {
                return mul(exp(a->t1, sub(a->t2,make_shared<ast>(1.0))), b->t1);
            }
        } else if (hash == "x^1/x*0") {
            if (a->t1->sym == b->sym) {
                return exp(a->t1, sub(a->t2,make_shared<ast>(1.0)));
            }
        } else if (hash == "x^x/x*0") {
            if (a->t1->sym == b->sym) {
                return exp(a->t1, sub(a->t2, make_shared<ast>(1.0)));
            }
        } else if (hash == "x*0/x^x") {
            if (a->sym == b->t1->sym) {
                return exp(a, sub(make_shared<ast>(1.0), a) );
            }
        }
    }

    return curr;
}
tuple<func_t,func_t,func_t> differentiate (const string &expression) {

    node pass0 = simplify(parse(expression));
    node pass1 = simplify(derivate(pass0));
    node pass2 = simplify(derivate(pass1));

    return {
        [pass0](value_t x) { return evaluate(pass0, x); },
            [pass1](value_t x) { return evaluate(pass1, x); },
            [pass2](value_t x) { return evaluate(pass2, x); },
    };
}

int main () {


    complex<double> x = {3.82,4.56};
    auto [dfx,dx1,dx2] = differentiate( "92.6/x^2.1*x^67.3+x");

    cout << "\nresult : " << dfx(x);

    string fx = "92.6/x^2.1*x^67.3+x";

    node pass0 = parse(fx);
    node pass1 = simplify(derivate(pass0));
    // node *pass2 = derivate(pass1);

    //showtree(pass1);
    //showflat(pass0);
    cout << "\nresult : " << evaluate(pass0,x) << "\n";


    test();
    std::cout << "\nend\n";
}
