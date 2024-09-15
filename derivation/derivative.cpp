#include <iostream>
#include <vector>
#include <complex>
#include <functional>

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

using namespace std;

using value_t = complex<double>;
using func_t = function<value_t(value_t)>;
const complex<double> zero = {0.0,0.0}, one = {1.0,0.0}, two = {2.0,0.0};

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

        if (node->sym == "") {
            cout << "[" << node->val << "]" << endl;
        } else {

            cout << "[" << node->sym << "]" << endl;
        }

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

node *parse (const string &input) {

    vector<string> code = tokenize(input);
    vector<string>::iterator it = code.begin();
    vector<node*> tree, oper;

    while (it < code.end()) {
        string curr = *it;

        if (curr == "x") {
            tree.push_back( new node(curr));
        } else if (curr == "(") {
            tree.push_back(parse(parenthesis(it)));
        } else if (is_number(curr)) {
            tree.push_back( new node(stoc(curr)));
        } else if (is_operator(curr)) {

            while (!oper.empty() && precedence(oper,curr)) {
                node *next = getstack(oper);
                next->t2 = getstack(tree);
                next->t1 = getstack(tree);
                tree.push_back(next);
            }

            oper.push_back(new node(curr));

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
        node *next = getstack(oper);
        next->t2 = getstack(tree);
        next->t1 = getstack(tree);
        tree.push_back(next);
    }

    return tree.back();
}
complex<double> evaluate (const node *node, const complex<double> &value) {

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
        // cout << "[" << a <<  "]" << term << "[" << b << "] => ";
        // cout << xz << "\n";
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
        return one / tan(a);
    } else {
        cout << "Invalid operator\n";


    }

    return 0;
}
node *derivate (const node *curr) {

    string term = curr->sym;
    node *t1 = curr->t1, *t2 = curr->t2;

    //cout << "[" << term << "|" << curr->val << "]" << "\n" << flush;
    if (term == "x") {
        return new node(one);
    } else if (term == "") {
        return new node(zero);
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
        //        if (t1->sym == "x" && t2->sym == "") {
        //            return mul(t2, exp( t1, sub(t2, new node(one)) ) ) ;
        //        }
        node *outer = exp(t1, t2);
        node *inner = add( mul( derivate(t1), div(t2,t1) ), mul( derivate(t2), new node("log", t1) ));
        return mul(inner,outer);
    } else if (term == "cos") {
        return sub(new node(zero), mul(derivate(t1), new node("sin", t1)));
    } else if (term == "sin") {
        return mul(derivate(t1), new node("cos", t1)) ;
    } else if (term == "tan") { // dx = 1 / (cos(x))^2
        return div(derivate(t1), exp(new node("cos", t1), new node(two)));
    } else if (term == "log") { // dx = x' / x
        return div(derivate(t1),t1);
    } else if (term == "cot") {
        return sub(new node(zero), div(derivate(t1), new node("^", new node("sin", t1), new node(two)))) ;
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

    //The function failed! f(x) = cot(x/45.3^x*x^32.3+x^85.1/x^20.9/x-5^45.3*86.2^67.6/x^x)^x, x = (-3.25,2.95)
    //Expected: equal to (39.3817,-95.0759) (+/- (0.015625,0.03125))
    //Actual: (nan,nan)


    //The second derivative failed! f(x) = tan(x-25.8-x*64.5), x = (2.32,9.61)
    //Expected: equal to (0,0) (+/- (0.001,0.001))
    //Actual: (-nan,-nan)

    // The second derivative failed! f(x) = tan(8.1+81.5^x+87.5*x)^94.1, x = (-4.87,-7.25)
    // Expected: equal to (0,0) (+/- (0.001,0.001))
    // Actual: (-nan,-nan)

    //The function failed! f(x) = cot(39.6^11.5*42.3/x+93.7^73.3), x = (-8.34,-4.4)
    //Expected: equal to (0,-1) (+/- (0.001,0.001))
    //Actual: (-nan,-nan)

    complex<double> x(-3.25,2.95);
    string expr = "cot(x/45.3^x*x^32.3+x^85.1/x^20.9/x-5^45.3*86.2^67.6/x^x)^x";

    node *pass0 = parse(expr);
    // node *pass1 = derivate(pass0);
    // node *pass2 = derivate(pass1);

    // showtree(pass0);
    cout << evaluate(pass0,x) << "\n";


    //tests();
    cout << "\nend\n";
}
